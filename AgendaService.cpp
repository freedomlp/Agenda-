#include "AgendaService.hpp"
#include <iostream>
#include "Date.hpp"
using namespace std;

AgendaService::AgendaService()
{
    startAgenda();
}

AgendaService::~AgendaService()
{
    quitAgenda();
}

bool AgendaService::userLogIn(const std::string &userName, const std::string &password)
{
    list<User> temp = listAllUsers();
    for(auto it = temp.begin(); it != temp.end(); ++it)
    {
        if(((*it).getName() == userName) && ((*it).getPassword() == password))
            return true;
    }
    return false;
}

bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                    const std::string &email, const std::string &phone)
{
    list<User> temp = listAllUsers();
    for(auto it = temp.begin(); it != temp.end(); ++it)
    {
        if((*it).getName() == userName)
            return false;
    }
    User user(userName,password,email,phone);
    m_storage->createUser(user);
    return true;
}

bool AgendaService::deleteUser(const std::string &userName, const std::string &password)
{
    if(!userLogIn(userName, password))
        return false;

    int flag = m_storage->deleteUser([&] (const User &a) {return (a.getName() == userName && a.getPassword() == password);});

    if(flag == 0)
        return false;
    else
    {
        m_storage->deleteMeeting([&] (const Meeting &a) {return a.getSponsor() == userName;});

        m_storage->updateMeeting([&] (const Meeting &a) {return a.isParticipator(userName);},
                                [&] (Meeting &b) {b.removeParticipator(userName);});
        
        m_storage->deleteMeeting([&] (const Meeting &a) {return a.getParticipator().size() == 0;});
    }
    return true;
}

std::list<User> AgendaService::listAllUsers(void) const
{
    list<User> output;
    output = m_storage->queryUser([&] (const User &a) {return true;});
    return output;
}

bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                     const std::string &startDate, const std::string &endDate,
                     const std::vector<std::string> &participator)
{
    if(participator.size() == 0)
        return false;

    for(int i = 0; i < participator.size(); ++i)
    {
        if(participator[i] == userName)
            return false;
    }

    Date start(startDate);
    Date end(endDate);

    if(start.getYear() == 0 && start.getMonth() == 0 && start.getDay() == 0 && start.getHour() == 0 && start.getMinute() == 0)
        return false;

    if(end.getYear() == 0 && end.getMonth() == 0 && end.getDay() == 0 && end.getHour() == 0 && end.getMinute() == 0)
        return false;

    if(!(Date::isValid(start) && Date::isValid(end)))
        return false;

    if(start >= end)
        return false;

    list<Meeting> meetings = m_storage->queryMeeting([&] (const Meeting &a) {return true;});
    for(auto it = meetings.begin(); it != meetings.end(); ++it)
    {
        if((*it).getTitle() == title)
            return false;
    }

    vector<string> peopleInMeeting = participator;
    peopleInMeeting.push_back(userName);
    
    list<User> u = m_storage->queryUser([&] (const User &a) {
        for(int i = 0; i < peopleInMeeting.size(); ++i)
        {
            if(a.getName() == peopleInMeeting[i])
                return true;
        }
        return false;});
    if(u.size() != peopleInMeeting.size())
        return false;

    for(int i = 0; i < peopleInMeeting.size(); ++i)
    {
        list<Meeting> have_meetings = listAllMeetings(peopleInMeeting[i]);
        for(auto it = have_meetings.begin(); it != have_meetings.end(); ++it)
        {
            if(!(start >= (*it).getEndDate() || end <= (*it).getStartDate()))
                return false;
        }
    }

    Meeting new_m(userName, participator, startDate, endDate, title);
    m_storage->createMeeting(new_m);
    return true;
}

bool AgendaService::addMeetingParticipator(const std::string &userName,
                              const std::string &title,
                              const std::string &participator)
{
    list<User> u = m_storage->queryUser([&] (const User &a) {return a.getName() == participator;});
    if(u.size() != 1)
        return false;

    int num = m_storage->updateMeeting([&] (const Meeting &a) {
        if(a.getSponsor() == userName && a.getTitle() == title)
        {
            if(a.getSponsor() == participator || a.isParticipator(participator))
                return false;

            list<Meeting> have_participate = listAllMeetings(participator);
            for(auto it = have_participate.begin(); it != have_participate.end(); ++it)
            {
                if(!(a.getStartDate() >= (*it).getEndDate() || a.getEndDate() <= (*it).getStartDate()))
                return false;
            }
            return true;
        }
        return false;
    }, [&] (Meeting &b) {b.addParticipator(participator);});

    if(num == 0)
        return false;

    return true;
}

bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                 const std::string &title,
                                 const std::string &participator)
{
    int num = m_storage->updateMeeting([&] (const Meeting &a) {return (a.getSponsor() == userName
    && a.getTitle() == title && a.isParticipator(participator));}, [&] (Meeting &b) {b.removeParticipator(participator);});
    if(num == 0)
        return false;

    m_storage->deleteMeeting([&] (const Meeting &a) {return a.getParticipator().size() == 0;});

    return true;
}

bool AgendaService::quitMeeting(const std::string &userName, const std::string &title)
{
    int flag = m_storage->updateMeeting([&] (const Meeting &a) {return (a.getTitle() == title && a.isParticipator(userName));}
    , [&] (Meeting &b) {b.removeParticipator(userName);});

    m_storage->deleteMeeting([&] (const Meeting &a) {return a.getParticipator().size() == 0;});

    if(flag == 0)
        return false;
    return true;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &title) const
{
    list<Meeting> meetings;
    meetings = m_storage->queryMeeting([&] (const Meeting &a) {
        return ((a.getSponsor() == userName || a.isParticipator(userName)) && a.getTitle() == title);});
    
    return meetings;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                  const std::string &startDate,
                                  const std::string &endDate) const
{
    list<Meeting> meetings;

    Date start(startDate);
    Date end(endDate);
    if(start.getYear() == 0 && start.getMonth() == 0 && start.getDay() == 0 && start.getHour() == 0 && start.getMinute() == 0)
        return meetings;

    if(end.getYear() == 0 && end.getMonth() == 0 && end.getDay() == 0 && end.getHour() == 0 && end.getMinute() == 0)
        return meetings;

    if(!(Date::isValid(start) && Date::isValid(end)))
        return meetings;

    if(start > end)
        return meetings;

    meetings = m_storage->queryMeeting([&] (const Meeting &a)
    {
        if(!(start > a.getEndDate() || end < a.getStartDate()))
                return true;

        return false;
    });
    return meetings;
}

std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const
{
    list<Meeting> meetings;
    meetings = m_storage->queryMeeting([&] (const Meeting &a) {return (a.getSponsor() == userName || a.isParticipator(userName));});

    return meetings;
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const
{
    list<Meeting> meetings;
    meetings = m_storage->queryMeeting([&] (const Meeting &a) {return a.getSponsor() == userName;});

    return meetings;
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(
      const std::string &userName) const
{
    list<Meeting> meetings;
    meetings = m_storage->queryMeeting([&] (const Meeting &a) {return a.isParticipator(userName);});

    return meetings;
}

bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title)
{
    int num = m_storage->deleteMeeting([&] (const Meeting &a) {return (a.getTitle() == title && a.getSponsor() == userName);});
    if(num == 0)
        return false;

    return true;
}

bool AgendaService::deleteAllMeetings(const std::string &userName)
{
    int num = m_storage->deleteMeeting([&] (const Meeting &a) {return a.getSponsor() == userName;});
    if(num == 0)
        return false;

    return true;
}

void AgendaService::startAgenda(void)
{
    m_storage = Storage::getInstance();
}


void AgendaService::quitAgenda(void)
{
    m_storage->sync();
}






#include "Storage.hpp"
#include "Path.hpp"
#include "Date.hpp"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

std::shared_ptr<Storage> Storage:: m_instance = NULL;

Storage::Storage()
{
    readFromFile();
    m_dirty = false;
}

bool Storage::readFromFile(void)
{
    ifstream fusers(Path::userPath);
    ifstream fmeetings(Path::meetingPath);
    if (!(fusers.is_open() && fmeetings.is_open()))
        return false;

    string user;
    while (getline(fusers, user))
    {
        vector<string> message;
        for (int i = 0; i < user.length(); ++i)
        {
            if (user[i] == '"')
            {
                int j = i + 1;
                for (j; user[j] != '"'; ++j);
                message.push_back(user.substr(i + 1, j - i - 1));
                i = j;
            }
        }
        User temp(message[0], message[1], message[2], message[3]);
        m_userList.push_back(temp);
    }
    fusers.close();

    string meeting;
    while (getline(fmeetings, meeting))
    {
        vector<string> message;
        for (int i = 0; i < meeting.length(); ++i)
        {
            if (meeting[i] == '"')
            {
                int j = i + 1;
                for (j; meeting[j] != '"'; ++j);
                message.push_back(meeting.substr(i + 1, j - i - 1));
                i = j;
            }
        }

        vector<string> participators;
        string participator;
        for (int i = 0; i < message[1].length(); ++i)
        {
            if (message[1][i] != '&')
                participator.push_back(message[1][i]);
            else
            {
                participators.push_back(participator);
                participator.clear();
            }
        }
        participators.push_back(participator);
        Date start(message[2]);
        Date end(message[3]);
        Meeting temp(message[0], participators, start, end, message[4]);
        m_meetingList.push_back(temp);
    }
    fmeetings.close();
    return true;
}

bool Storage::writeToFile(void)
{
    ofstream fusers(Path::userPath);
    ofstream fmeetings(Path::meetingPath);
    if (!(fusers.is_open() && fmeetings.is_open()))
        return false;

    for (auto it = m_userList.begin(); it != m_userList.end(); ++it)
    {
        fusers << '"' << (*it).getName() << '"' << ',' << '"' << (*it).getPassword() << '"' << ','
        << '"' << (*it).getEmail() << '"' << ',' << '"' << (*it).getPhone() << '"' << endl;
    }
    fusers.close();

    for(auto it = m_meetingList.begin(); it != m_meetingList.end(); ++it)
    {
        vector<string> participators = (*it).getParticipator();
        fmeetings << '"' << (*it).getSponsor() << '"' << ',';

        fmeetings << '"';
        for(int i = 0; i < participators.size(); ++i)
        {
            fmeetings << participators[i];
            if(i != (participators.size() - 1))
                fmeetings << '&';
            else
                fmeetings << '"' << ',';
        }

        Date start = (*it).getStartDate();
        Date end = (*it).getEndDate();
        fmeetings << '"' << Date::dateToString(start) << '"' << ','
        << '"' << Date::dateToString(end) << '"' << ',' << '"' << (*it).getTitle() << '"' << endl;
    }
    fmeetings.close();
    return true;
}

std::shared_ptr<Storage> Storage::getInstance(void)
{
    if(m_instance == NULL)
        m_instance = std::shared_ptr<Storage>(new Storage);
    return m_instance;
}

Storage:: ~Storage()
{
    sync();
}

void Storage::createUser(const User &t_user)
{
    m_userList.push_back(t_user);
    m_dirty = true;
}

std::list<User> Storage::queryUser(std::function<bool(const User &)> filter) const
{
    list<User> found_list;
    for(auto it = m_userList.begin(); it != m_userList.end(); ++it)
    {
        if(filter(*it))
            found_list.push_back(*it);
    }
    return found_list;
}

int Storage::updateUser(std::function<bool(const User &)> filter,
                 std::function<void(User &)> switcher)
{
    int update_num = 0;
    for(auto it = m_userList.begin(); it != m_userList.end(); ++it)
    {
        if(filter(*it))
        {
            switcher(*it);
            ++ update_num;
        }
    }
    m_dirty = true;
    return update_num;
}

int Storage::deleteUser(std::function<bool(const User &)> filter)
{
    int delete_num = 0;
    for(auto it = m_userList.begin(); it != m_userList.end();)
    {
        if(filter(*it))
        {
            it = m_userList.erase(it);
            ++ delete_num;
        }
        else
            it++;
    }
    m_dirty = true;
    return delete_num;
}

void Storage::createMeeting(const Meeting &t_meeting)
{
    m_meetingList.push_back(t_meeting);
    m_dirty = true;
}

std::list<Meeting> Storage::queryMeeting(
      std::function<bool(const Meeting &)> filter) const
{
    list<Meeting> found_list;
    for(auto it = m_meetingList.begin(); it != m_meetingList.end(); ++it)
    {
        if(filter(*it))
            found_list.push_back(*it);
    }
    return found_list;
}

int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                    std::function<void(Meeting &)> switcher)
{
    int update_num = 0;
    for(auto it = m_meetingList.begin(); it != m_meetingList.end(); ++it)
    {
        if(filter(*it))
        {
            switcher(*it);
            ++ update_num;
        }
    }
    m_dirty = true;
    return update_num;
}

int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter)
{
    int delete_num = 0;
    for(auto it = m_meetingList.begin(); it != m_meetingList.end();)
    {
        if(filter(*it))
        {
            it = m_meetingList.erase(it);
            ++ delete_num;
        }
        else
            it++;
    }
    m_dirty = true;
    return delete_num;
}

bool Storage::sync(void)
{
    if(m_dirty = false)
        return false;
    else
    {
        bool flag = writeToFile();
        if(flag = false)
            return false;
        m_dirty = false;
    }
    return true;
}

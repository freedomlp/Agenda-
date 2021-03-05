#include "AgendaUI.hpp"
#include "Storage.cpp"
#include "User.cpp"
#include "Date.cpp"
#include "Meeting.cpp"
#include "AgendaService.cpp"
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

AgendaUI::AgendaUI()
{
    startAgenda();
}

void AgendaUI::OperationLoop(void)
{
    string op;
    do
    {
        cout << "-------------------------Agenda---------------------------" << endl;
        cout << "action:" << endl;
        cout << "l     -log in Agenda by user name and password" << endl;
        cout << "r     -register an Agenda account" << endl;
        cout << "q     -quit Agenda" << endl;
        cout << "----------------------------------------------------------" << endl
             << endl;
        cout << "Agenda :~$";

        cin >> op;
        if(op == "l")
            userLogIn();

        if(op == "r")
            userRegister();

        if(op == "q")
            quitAgenda();

    } while (op != "q");
}

void AgendaUI::startAgenda(void)
{
    m_agendaService.startAgenda();
}

std::string AgendaUI::getOperation()
{
    cout << "-------------------------Agenda---------------------------" << endl;
    cout << "Action:" << endl;
    cout << "o     -log out Agenda" << endl;
    cout << "dc    -delete Agenda user" << endl;
    cout << "lu    -list all agenda user" << endl;
    cout << "cm    -create a meeting" << endl;
    cout << "amp   -add meeting participator" << endl;
    cout << "rmp   -remove meeting participator" << endl;
    cout << "rqm   -request to quit meeting" << endl;
    cout << "la    -list all meeings" << endl;
    cout << "las   -list all sponsor meetings" << endl;
    cout << "lap   -list all participator meetings" << endl;
    cout << "qm    -query meeting by title" << endl;
    cout << "qt    -query meeting by time interval" << endl;
    cout << "dm    -delete meeting by title" << endl;
    cout << "da    -delete all meetings" << endl;
    cout << "----------------------------------------------------------" << endl << endl;
    cout << "Agenda@" << m_userName << " :~# ";

    string op;
    cin >> op;
    return op;
}

bool AgendaUI::executeOperation(std::string t_operation)
{
    if(t_operation == "o")
    {
        userLogOut();
        return false;
    }

    if(t_operation == "dc")
    {
        deleteUser();
        return false;
    }

    if(t_operation == "lu")
    {
        listAllUsers();
        return true;
    }

    if(t_operation == "cm")
    {
        createMeeting();
        return true;
    }

    if(t_operation == "amp")
    {
        addMeetingParticipator();
        return true;
    }

    if(t_operation == "rmp")
    {
        removeMeetingParticipator();
        return true;
    }


    if(t_operation == "rqm")
    {
        quitMeeting();
        return true;
    }

    if(t_operation == "la")
    {
        listAllMeetings();
        return true;
    }

    if(t_operation == "las")
    {
        listAllSponsorMeetings();
        return true;
    }

    if(t_operation == "lap")
    {
        listAllParticipateMeetings();
        return true;
    }

    if(t_operation == "qm")
    {
        queryMeetingByTitle();
        return true;
    }

    if(t_operation == "qt")
    {
        queryMeetingByTimeInterval();
        return true;
    }

    if(t_operation == "dm")
    {
        deleteMeetingByTitle();
        return true;
    }

    if(t_operation == "da")
    {
        deleteAllMeetings();
        return true;
    }
    return false;
}

void AgendaUI::userLogIn(void)
{
    cout << endl;
    cout << "[log in] [username] [password]" << endl;
    cout << "[log in] " ;
    string username, password;
    cin >> username >> password;
    if(m_agendaService.userLogIn(username, password))
    {
        cout << "[log in] succeed!" << endl;
        m_userName = username;
        m_userPassword = password;
        while(executeOperation(getOperation()));
    }
    else
        cout << "[log in] Password error or user dosen't exist" << endl;
}

void AgendaUI::userRegister(void)
{
    cout << endl;
    cout << "[register] [username] [password] [email] [phone]" << endl;
    cout << "[register] " ;
    string username, password, email, phone;
    cin >> username >> password >> email >> phone;
    if(m_agendaService.userRegister(username, password, email, phone))
        cout << "[register] succeed" << endl;
    else
        cout << "[register] This user name has been registerd!" << endl;
}

void AgendaUI::userLogOut(void)
{
    m_userName = "";
    m_userPassword = "";
}

void AgendaUI::quitAgenda(void)
{
    m_agendaService.quitAgenda();
}

void AgendaUI::deleteUser(void)
{
    if(m_agendaService.deleteUser(m_userPassword, m_userPassword))
    {
        cout << "[delete agenda account] succeed!" << endl;
    }
    userLogOut();
}

void AgendaUI::listAllUsers(void)
{
    list<User> user = m_agendaService.listAllUsers();
    cout << endl;
    cout << "[list all users]" << endl << endl;
    cout << left << setw(15) << "name" << left << setw(25) << "email" << "phone" << endl;
    for(auto it = user.begin(); it != user.end(); ++it)
    {
        cout << left << setw(15) << (*it).getName() << left << setw(25) << (*it).getEmail() << (*it).getPhone() << endl;
    }
}

void AgendaUI::createMeeting(void)
{
    cout << endl;
    cout << "[create meeting] [the number of participators]" << endl;
    int num;
    cout << "[create meeting] ";
    cin >> num;
    vector<string> parts;
    for(int i = 1; i <= num; ++i)
    {
        cout << "[create meeting] " << "[please enter the participator " << i << "]" << endl;
        cout << "[create meeting] ";
        string part;
        cin >> part;
        parts.push_back(part);
    }
    cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mmdd/hh:mm)]" << endl;
    cout << "[create meeting] ";
    string title, start, end;
    cin >> title >> start >> end;

    if(m_agendaService.createMeeting(m_userName, title, start, end, parts))
        cout << "[create meeting] succeed!" << endl;
    else
        cout << "[create meeting] error!" << endl;
}

void AgendaUI::addMeetingParticipator(void)
{
    cout << endl;
    cout << "[add participator] [meeting title] [participator username]" << endl;
    cout << "[add participator] ";
    string title, participator;
    cin >> title >> participator;
    if(m_agendaService.addMeetingParticipator(m_userName, title, participator))
        cout << "[add participator] succeed!" << endl;
    else
        cout << "[add participator] error!" << endl;
}

void AgendaUI::removeMeetingParticipator(void)
{
    cout << endl;
    cout << "[remove participator] [meeting title] [participator username]" << endl;
    cout << "[remove participator] ";
    string title, participator;
    cin >> title >> participator;
    if(m_agendaService.removeMeetingParticipator(m_userName, title, participator))
        cout << "[remove participator] succeed!" << endl;
    else
        cout << "[remove participator] error!" << endl;
}

void AgendaUI::quitMeeting(void)
{
    cout << endl;
    cout << "[quit meeting] [meeting title]" << endl;
    cout << "[quit meeting] ";
    string title;
    cin >> title;
    if(m_agendaService.quitMeeting(m_userName, title))
        cout << "[quit meeting] succeed!" << endl;
    else
        cout << "[quit meeting] error!" << endl;
}

void AgendaUI::listAllMeetings(void)
{
    cout << endl;
    cout << "[list all meetings]" << endl;
    list<Meeting> meetings = m_agendaService.listAllMeetings(m_userName);
    printMeetings(meetings);
}

void AgendaUI::listAllSponsorMeetings(void)
{
    cout << endl;
    cout << "[list all sponsor meetings]" << endl;
    list<Meeting> meetings = m_agendaService.listAllSponsorMeetings(m_userName);
    printMeetings(meetings);
}

void AgendaUI::listAllParticipateMeetings(void)
{
    cout << endl;
    cout << "[list all participator meetings]" << endl;
    list<Meeting> meetings = m_agendaService.listAllParticipateMeetings(m_userName);
    printMeetings(meetings);
}

void AgendaUI::queryMeetingByTitle(void)
{
    cout << endl;
    cout << "[query meeting] [title]" << endl;
    cout << "[query meeting] ";
    string title;
    cin >> title;
    list<Meeting> meetings = m_agendaService.meetingQuery(m_userName, title);
    printMeetings(meetings);
}

void AgendaUI::queryMeetingByTimeInterval(void)
{
    cout << endl;
    cout << "[query meeting] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl;
    cout << "[query meeting] ";

    string start, end;
    cin >> start >> end;
    cout << "[query meeting]" << endl;

    list<Meeting> meetings = m_agendaService.meetingQuery(m_userName, start, end);
    printMeetings(meetings);
}

void AgendaUI::deleteMeetingByTitle(void)
{
    cout << "[delete meeting] [title]" << endl;
    cout << "[delete meeting] ";
    string title;
    if(m_agendaService.deleteMeeting(m_userName, title))
        cout << "[delete meeting] succeed!" << endl;
    else
        cout << "[delete meeting] error!" << endl;
}

void AgendaUI::deleteAllMeetings(void)
{
    if(m_agendaService.deleteAllMeetings(m_userName))
        cout << "[delete all meeting] succeed!" << endl;
}

void AgendaUI::printMeetings(std::list<Meeting> t_meetings)
{
    cout << endl;
    cout << left << setw(15) << "title" << left << setw(12) << "sponsor" 
    << left << setw(20) << "start date" << left << setw(20) << "end date" << left << setw(15) << "participators" << endl;
    for(auto it = t_meetings.begin(); it != t_meetings.end(); ++it)
    {
        cout << left << setw(15) << (*it).getTitle() << left << setw(12) << (*it).getSponsor() 
         << left << setw(20) << Date::dateToString((*it).getStartDate()) 
         << left << setw(20) <<  Date::dateToString((*it).getEndDate());

        vector<string> participator = (*it).getParticipator();
        for (int i = 0; i < participator.size(); ++i)
        {
            cout << participator[i];
            if(i != participator.size() - 1)
                cout << ',';
        }
        cout << endl;
    }
}

int main(){
	AgendaUI u;
	u.OperationLoop();
}


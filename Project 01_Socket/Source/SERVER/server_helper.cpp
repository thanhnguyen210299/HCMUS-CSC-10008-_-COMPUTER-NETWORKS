#include "server.h"
#include "ui_server.h"

string server::fetchMessage(string message, int t)
{
    int j=0;
    for (j=0; j<message.length(); j++)
    {
        if (message[j]=='[') t--;
        if (t==0) break;
    }
    j++;
    string res="";
    while (message[j]!=']' && j<message.length())
        res=res+message[j++];
    return res;
}

void server::load_Account_File()
{
    ifstream f;
    f.open("ClientAccount.txt");
    int i=0;
    User temp;
    string str;
    if (f.fail())
        return;
    while (!f.eof())
    {
        getline(f, str);
        if (i % 2 == 0)
            temp.username = str;
        else
        {
            temp.password = str;
            temp.online = false;
            Clients.push_back(temp);
        }
        i++;
    }
    f.close();
}

bool server::Check_Username(string user)
{
    for (int i = 0; i < Clients.size(); i++)
    {
        if (user == Clients[i].username)
            return 1;
    }
    return 0;
}

bool server::Check_Password(string name, string pass)
{
    for (int i = 0; i < Clients.size(); i++)
    {
        if (name == Clients[i].username && pass == Clients[i].password)
            return 1;
    }
    return 0;
}

void server::Add_User(string name, string pass, QTcpSocket *&client)
{
    User a;
    a.username = name;
    a.password = pass;
    a.socket = client;
    a.online = false;
    Clients.push_back(a);
}

void server::Update_User(string name, QTcpSocket *&client)
{
    for (int i = 0; i < Clients.size(); i++)
        if (Clients[i].username == name)
        {
            Clients[i].socket = client;
            Clients[i].online = true;
        }
}

void server::Delete_Group_Chat(string name)
{
    ui->activityText->append(QString::fromStdString("Delete group " + name));
    int len = g_chat.size();
    int pos;
    for (pos = 0; pos < g_chat.size(); pos++)
        if (g_chat[pos].group_name == name)
            break;
    for (int i = pos; i < g_chat.size() - 1; i++)
        g_chat[i].group_name == g_chat[i + 1].group_name;
    g_chat.resize(len - 1);
    Print_Group_List();
}

void server::Delete_User_From_Group(string groupname, string user)
{
    for (int i = 0; i < g_chat.size(); i++)
    {
        // find group
        if (g_chat[i].group_name == groupname)
        {
            int pos;
            // find user
            for (pos = 0; pos < g_chat[i].number; pos++)
                if (g_chat[i].users[pos].username == user)
                    break;

            // delete user
            for (int j = pos; j < g_chat[i].number - 1; j++)
                g_chat[i].users[j].username == g_chat[i].users[j + 1].username;
            g_chat[i].number--;
            g_chat[i].users.resize(g_chat[i].number);

            // delete group if number = 0
            if (g_chat[i].number == 0)
                Delete_Group_Chat(groupname);
             return;
        }
    }
}

bool server::is_Existed_Group(string name)
{
    for (int i = 0; i < g_chat.size(); i++)
        if (g_chat[i].group_name == name)
            return true;
    return false;
}

void server::Add_Group_Chat(string name)
{
    GroupChat temp;
    temp.group_name = name;
    temp.number = 0;
    g_chat.push_back(temp);
}

bool server::isUserOnline(string name)
{
    for (int i=0; i< Clients.size(); i++)
        if (Clients[i].username == name && Clients[i].online)
            return true;
    return false;
}

bool server::Add_User_To_Group(string group_name, string username)
{
    for (int i = 0; i < g_chat.size(); i++)
        {
            // find group
            if (g_chat[i].group_name == group_name)
            {
                // find user
                for (int j = 0; j < g_chat[i].number; j++)
                    if (g_chat[i].users[j].username == username)
                        return false;

                // add user
                User temp;
                temp.username = username;
                g_chat[i].users.push_back(temp);
                g_chat[i].number++;
                return true;
            }
        }
}

void server::Add_Private_Chat(string sender, string receiver)
{
    PrivateChat temp;
    temp.client1 = sender;
    temp.client2 = receiver;
    p_chat.push_back(temp);
}

bool server::is_Existed_Private(string sender, string receiver)
{
    for (int i = 0; i < p_chat.size(); i++)
        if ((p_chat[i].client1 == sender && p_chat[i].client2 == receiver) || (p_chat[i].client1 == receiver && p_chat[i].client2 == sender))
            return true;
    return false;
}

bool server::is_Exist_User(string group_name, string username)
{
    for (int i = 0; i < g_chat.size(); i++)
        {
            // find group
            if (g_chat[i].group_name == group_name)
            {
                // find user
                for (int j = 0; j < g_chat[i].number; j++)
                    if (g_chat[i].users[j].username == username)
                        return true;
                return false;
            }
        }
}

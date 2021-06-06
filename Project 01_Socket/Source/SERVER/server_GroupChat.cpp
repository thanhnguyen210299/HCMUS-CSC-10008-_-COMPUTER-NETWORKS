#include "server.h"
#include "ui_server.h"

void server::Create_Group_Chat(QTcpSocket *&client, string message)
{

    /*
        client send to server: [CREATE_GROUP_CHAT][groupname]
        Server reply         : [CREATE_GROUP_CHAT][groupname][SUCCESS] hay [CREATE_GROUP_CHAT][groupname][FAIL]
    */

    // catch group name
    string name=fetchMessage(message,2);

    // check existed group chat
    if (!is_Existed_Group(name))
    {
        // Add Group Chat to List
        Add_Group_Chat(name);

        // Add User to list of group
        Add_User_To_Group(name, client->objectName().toStdString());

        SendMessageToClient(client,"[CREATE_GROUP_CHAT]["+name+"][SUCCESS]");
        ui->activityText->append(QString::fromStdString("New group chat "+name));
        ui->groupListText->append(QString::fromStdString(name));

        Solve_Send_User_List(name, client->objectName().toStdString());
    }
    else
        SendMessageToClient(client,"[CREATE_GROUP_CHAT]["+name+"][FAIL]");
}

void server::Solve_Group_Chat(string message)
{
    /*
        + Client gửi gói tin đến server:         [GROUP_CHAT][groupname][sender][content]
        + Server gửi gói tin cho các user khác:  [GROUP_CHAT][groupname][sender][content]
    */

    string groupname = fetchMessage(message,2);
    string content=fetchMessage(message,4);
    string sender=fetchMessage(message,3);

    ui->activityText->append(QString::fromStdString(sender + " send to group " + groupname + ": '" + content + "'"));

    for (int i = 0; i < g_chat.size(); i++)
        // find group
        if (g_chat[i].group_name == groupname)
        {
            // send message to clients
            for (int j = 0; j < g_chat[i].number; j++)
                SendMessageToClient(g_chat[i].users[j].username, message);
            return;
        }
}

void server::Solve_Send_User_List(string groupname, string username)
{
    // send to new member user list in group
    string user_list = "[USER_LIST]["+groupname+"]";
    /* message: [USER_LIST][groupname][user1][user2]...[usern]*/
    for (int i=0; i<g_chat.size(); i++)
    {
        if (g_chat[i].group_name == groupname)
        {
            for (int j=0; j< g_chat[i].users.size(); j++)
                user_list = user_list + "["+g_chat[i].users[j].username+"]";

            SendMessageToClient(username, user_list);
            return;
        }
    }
}

string server::Create_User_List_Delete(string groupname, string name)
{
    // send to new member user list in group
    string user_list = "[USER_LIST]["+groupname+"]";
    /* message: [USER_LIST][groupname][user1][user2]...[usern]*/
    for (int i=0; i<g_chat.size(); i++)
    {
        if (g_chat[i].group_name == groupname)
        {
            for (int j=0; j< g_chat[i].users.size(); j++)
                if (g_chat[i].users[j].username != name)
                    user_list = user_list + "["+g_chat[i].users[j].username+"]";
            break;
        }
    }
    return user_list;
}


void server::Solve_Add_User(string message)
{
    /*
    + Client gửi gói tin dến server: [ADD_USER][groupname][sender][new_user]
    + Server gửi gói tin cho
    ~ sender:              [ADD_USER][groupname][new_user][SUCCESS] hay [ADD_USER][groupname][new_user][FAIL]
    ~ new_user:            [CREATE_GROUP_CHAT][groupname][SUCCESS]
    ~ các user còn lại:    [GROUP_CHAT_NEW_MEMBER][groupname][new_user]
    */

    string groupname=fetchMessage(message,2);
    string username=fetchMessage(message,4);
    string sender=fetchMessage(message,3);

    // check User online & User not in group
    if (isUserOnline(username))
        if (Add_User_To_Group(groupname, username))
        {
            ui->activityText->append(QString::fromStdString("Add " + username + " in to group " + groupname));

            // send message to sender
            string msg = "[ADD_USER]["+groupname+"]["+username+"][SUCCESS]";
            SendMessageToClient(sender, msg);

            // send message to new member
            msg="[CREATE_GROUP_CHAT]["+groupname+"][SUCCESS]";
            SendMessageToClient(username, msg);

            Solve_Send_User_List(groupname, username);

            // send message to other user in group
            msg="[GROUP_CHAT_NEW_MEMBER]["+groupname+"]["+username+"]";

            for (int i = 0; i < g_chat.size(); i++)
            {
                // find group
                if (g_chat[i].group_name == groupname)
                {
                    // if user != sender & user != new member
                    for (int j = 0; j < g_chat[i].number; j++)
                        if (g_chat[i].users[j].username != username && g_chat[i].users[j].username != sender)
                            SendMessageToClient(g_chat[i].users[j].username, msg);
                    return;
                }
            }
        }
    string msg = "[ADD_USER]["+groupname+"]["+username+"][FAIL]";
    SendMessageToClient(sender, msg);
}

void server::End_Group_Chat(string message)
{
    /*
        + Client gửi gói tin đến server:         [END_GROUP_CHAT][groupname][sender]
        + Server gửi gói tin cho các user khác:  [GROUP_CHAT_END_USER][groupname][sender]
                                                 [END_GROUP_CHAT][groupname]
    */

    string groupname = fetchMessage(message,2);
    string sender=fetchMessage(message,3);

    SendMessageToClient(sender, "[END_GROUP_CHAT]["+ groupname + "]");

    if (!is_Exist_User(groupname, sender))
        return;

    ui->activityText->append(QString::fromStdString(sender + " is leave group " + groupname));
    string mess_send = "[GROUP_CHAT_END_USER][" + groupname + "][" + sender + "]";
    mess_send = mess_send + "`" + Create_User_List_Delete(groupname, sender);
    for (int i = 0; i < g_chat.size(); i++)
    {
        // find group
        if (g_chat[i].group_name == groupname)
        {
            // if user != sender
            for (int j = 0; j < g_chat[i].number; j++)
                if (g_chat[i].users[j].username != sender)
                {
                    SendMessageToClient(g_chat[i].users[j].username, mess_send);
                }
            break;
        }
    }

    // Xoa user khoi group
    Delete_User_From_Group(groupname, sender);
}

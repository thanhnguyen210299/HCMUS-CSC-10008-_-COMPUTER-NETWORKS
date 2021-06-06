#include "client.h"
#include "ui_client.h"

void client::Solve_Create_Group_Chat(string message)
{
    string status=fetchMessage(message,3);
    if (status=="SUCCESS")
    {
        string group_name=fetchMessage(message,2);
        group_chat *newGroup = new group_chat();

        groups.push_back(newGroup);
        groups[groups.size()-1]->setName(group_name);
        groups[groups.size()-1]->setClientName(username);
        groups[groups.size()-1]->Show();

        ui->groupChatButton->show();
        ui->privateChatButton->show();
        ui->type->hide();
        ui->name->hide();
        ui->ok->hide();
        ui->backButton->hide();
        selection=NOT_SELECTED;
    }
    if (status == "FAIL")
    {
        QMessageBox::warning(this, "Warning", "Group name is existed", QMessageBox::Ok);
        ui->name->setText("");
    }
}

void client::Solve_Group_Chat(string message)
{
    //[GROUP_CHAT][groupname][sender][content]
    string groupname=fetchMessage(message,2);
    string sender=fetchMessage(message, 3);
    string msg=fetchMessage(message,3)+": "+fetchMessage(message,4);
    for (int i=0; i<groups.size(); i++)
        if (groups[i]->getName()==groupname)
        {
            if (groups[i]->getClientName() == sender)
            {
                groups[i]->print("You: " + fetchMessage(message, 4));
            }
            else
                groups[i]->print(msg);
            break;
        }
}

void client::Solve_Add_User(string message)
{
    //[ADD_USER][groupname][new_member][SUCCESS]
    string groupname=fetchMessage(message,2);
    string new_member = fetchMessage(message, 3);
    string status = fetchMessage(message,4);

    for (int i=0; i<groups.size(); i++)
        if (groups[i]->getName() == groupname)
        {
            if (status == "SUCCESS")
            {
                groups[i]->print_Message_Add_Success();
                groups[i]->print_User(new_member);
                groups[i]->print_mess("Add " + new_member);
            }
            else
            {
                groups[i]->print_Message_Add_Fail();
            }
            return;
        }
}

void client::Solve_Group_NewMember(string message)
{
    /*[GROUP_CHAT_NEW_USER][groupname][new_user]*/
    string groupname = fetchMessage(message, 2);
    string new_user = fetchMessage(message, 3);
    for (int i=0; i<groups.size(); i++)
        if (groups[i]->getName()==groupname)
        {
            groups[i]->print_mess("Add " + new_user);
            groups[i]->print_User(new_user);
            break;
        }
}

void client::Solve_Group_EndMember(string message)
{
    /*[GROUP_CHAT_END_USER][groupname][sender]*/
    string groupname = fetchMessage(message, 2);
    string sender = fetchMessage(message, 3);
    for (int i=0; i<groups.size(); i++)
        if (groups[i]->getName()==groupname)
        {
            groups[i]->print_mess(sender + " is leave group");
            break;
        }
}

void client::PrintGroupUserList(string message)
{
    // [USER_LIST][groupname][user1][user2]...
    string groupname= fetchMessage(message, 2);
    for (int i= 0; i < groups.size(); i++)
    {
        if (groups[i]->getName() == groupname)
        {
            groups[i]->print_User_List(message);
            break;
        }
    }
}

void client::End_Group_Chat(string message)
{
    // [END_GROUP_CHAT][groupname]
    string groupname = fetchMessage(message, 2);
    for (int i= 0; i < groups.size(); i++)
    {
        if (groups[i]->getName() == groupname)
        {
            groups[i]->setName("");
            QMessageBox::warning(this, "Warning", QString::fromStdString("End group chat " + groupname), QMessageBox::Ok);
            break;
        }
    }
}

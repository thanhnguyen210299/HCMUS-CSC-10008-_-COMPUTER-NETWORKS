#include "server.h"
#include "ui_server.h"

void server::Print_Users_List()
{
    ui->userListText->setText("");
    for (int i = 0; i < Clients.size(); i++)
        if (Clients[i].online)
            ui->userListText->append(QString::fromStdString(Clients[i].username));
}

void server::Print_Group_List()
{
    ui->groupListText->setText("");
    for (int i = 0; i < g_chat.size(); i++)
        ui->groupListText->append(QString::fromStdString(g_chat[i].group_name));
}

void server::Print_Private_List()
{
    ui->privateListText->setText("");
    for (int i=0; i<p_chat.size(); i++)
        ui->privateListText->append(QString::fromStdString(p_chat[i].client1 + " & " + p_chat[i].client2));
}

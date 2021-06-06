#include "client.h"
#include "ui_client.h"

void client::Solve_Create_Private_Chat(string message)
{
    //message: [CREATE_PRIVATE_CHAT][sender][receiver][SUCCESS] / [FAIL]
    string status=fetchMessage(message, 4);

    if (status=="FAIL")
    {
        QMessageBox::warning(this, "Warning", "Invalid username!", QMessageBox::Ok);
        ui->name->setText("");
    }
    else
    {
        string receiver=fetchMessage(message,3);
        string sender = fetchMessage(message, 2);
        private_chat *newRoom = new private_chat();

        private_rooms.push_back(newRoom);
        private_rooms[private_rooms.size()-1]->setPartner(receiver);
        private_rooms[private_rooms.size()-1]->setClientName(sender);
        private_rooms[private_rooms.size()-1]->Show();

        ui->groupChatButton->show();
        ui->privateChatButton->show();
        ui->type->hide();
        ui->name->hide();
        ui->ok->hide();
        ui->backButton->hide();
        selection=NOT_SELECTED;
    }
}

void client::Solve_Private_Chat(string message)
{
    //message: [PRIVATE_CHAT][sender][content]
    string sender=fetchMessage(message,2);
    string content=fetchMessage(message,3);
    for (int i=0; i<private_rooms.size(); i++)
        if (private_rooms[i]->getPartner()==sender)
        {
            private_rooms[i]->print(sender+": "+content);
            break;
        }
}

void client::End_Private_Chat(string message)
{
    //message: [END_PRIVATE_CHAT][sender]
    string sender=fetchMessage(message,2);
    string mess_send = "End private chat with "+sender;
    for (int i=0; i<private_rooms.size(); i++)
        if (private_rooms[i]->getPartner()==sender)
        {
            private_rooms[i]->close();
            QMessageBox::warning(this,"Warning", QString::fromStdString(mess_send), QMessageBox::Ok);

            private_rooms[i]->setPartner("");
        }
}

#include "server.h"
#include "ui_server.h"

void server::Create_Private_Chat(QTcpSocket *&client, string message)
{
    /*
        + client -> server: [CREATE_PRIVATE_CHAT][sender][receiver]
        + server:
                 -> sender: [CREATE_PRIVATE_CHAT][sender][receiver][SUCCESS] / [FAIL]
                 -> receiver: [CREATE_PRIVATE_CHAT][receiver][sender][SUCCESS]
    */

    // catch receiver
    string receiver=fetchMessage(message,3);
    string sender=fetchMessage(message, 2);

    // check if user is online
    if (sender!= receiver && isUserOnline(receiver) && !is_Existed_Private(sender, receiver))
    {
        ui->activityText->append(QString::fromStdString("Private chat between "+sender+" and "+receiver+" started."));

        Add_Private_Chat(sender, receiver);

        SendMessageToClient(sender, "[CREATE_PRIVATE_CHAT]["+sender+"]["+receiver+"][SUCCESS]");
        SendMessageToClient(receiver, "[CREATE_PRIVATE_CHAT]["+receiver+"]["+sender+"][SUCCESS]");

        ui->privateListText->append(QString::fromStdString(sender + " & " + receiver));
    }
    else
        SendMessageToClient(client,"[CREATE_PRIVATE_CHAT]["+sender+"]["+receiver+"][FAIL]");
}

void server::Private_Chat(string message)
{
    /*
        + client -> server:  [PRIVATE_CHAT][sender][receiver][content]
        + server -> client:  [PRIVATE_CHAT][sender][content]
    */

    string sender=fetchMessage(message,2);
    string receiver=fetchMessage(message,3);
    string content=fetchMessage(message,4);

    ui->activityText->append(QString::fromStdString(sender + " sent to " + receiver + ": '" + content + "'"));
    SendMessageToClient(receiver,"[PRIVATE_CHAT]["+sender+"]["+content+"]");
}

void server::End_Private_Chat(string message)
{
    /*
        + client -> server:  [END_PRIVATE_CHAT][sender][receiver]
        + server -> client:  [END_PRIVATE_CHAT][sender]
    */

    string sender=fetchMessage(message,2);
    string receiver=fetchMessage(message,3);
    SendMessageToClient(receiver,"[END_PRIVATE_CHAT]["+sender+"]");

    // delete private_chat
    for (int i=0; i<p_chat.size(); i++)
    {
        if (p_chat[i].client1==sender && p_chat[i].client2==receiver || p_chat[i].client1==receiver && p_chat[i].client2==sender)
        {
            ui->activityText->append(QString::fromStdString("End private chat between "+sender+" and "+receiver));
            p_chat[i]=p_chat[p_chat.size()-1];
            p_chat.pop_back();
            break;
        }
    }

    Print_Private_List();
}

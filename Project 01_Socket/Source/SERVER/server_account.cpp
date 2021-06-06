#include "server.h"
#include "ui_server.h"

void server::Solve_Login(QTcpSocket *&client, string message)
{
    /*
        - client gửi gói tin cho server: [LOGIN][username][password]
        - server gửi gói tin cho client: [LOGIN][PASS_WRONG]
                                         [LOGIN][SUCCESS]
                                         [LOGIN][NOT_EXIST]
    */

    string username = fetchMessage(message, 2);
    string password = fetchMessage(message, 3);
    string mess_send;

    if (!Check_Username(username))
        mess_send = "[LOGIN][NOT_EXIST]";
    else
    {
        if (Check_Password(username, password))
        {
            if (isUserOnline(username))
                mess_send = "[LOGIN][EXISTED]";
            else
            {

                ui->activityText->append(QString::fromStdString(username + " is connected!"));

                client->setObjectName(QString::fromStdString(username));
                mess_send = "[LOGIN][SUCCESS]";

                Update_User(username, client);
                Print_Users_List();

            }
        }
        else
            mess_send = "[LOGIN][PASS_WRONG]";
    }

   SendMessageToClient(client, mess_send);
}

void server::Solve_Sign_Up(QTcpSocket *&client, string message)
{
    /*
        - client gửi gói tin cho server: [SIGN_UP][username][password]
        - server gửi gói tin cho client: [SIGN_UP][SUCCESS]
                                         [SIGN_UP][FAIL]
    */

    string username = fetchMessage(message, 2);
    string password = fetchMessage(message, 3);
    string mess_send;

    if (Check_Username(username))
        mess_send = "[SIGN_UP][FAIL]";
    else
    {
        ui->activityText->append(QString::fromStdString(username + " is signed up!"));

        Add_User(username, password, client);

        mess_send = "[SIGN_UP][SUCCESS]";

        ofstream f;
        f.open("ClientAccount.txt", ios::app);
        f << username << endl;
        f << password << endl;
        f.close();
    }
    SendMessageToClient(client, mess_send);
}

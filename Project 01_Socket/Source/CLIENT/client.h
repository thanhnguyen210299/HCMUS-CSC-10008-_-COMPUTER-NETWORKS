#ifndef CLIENT_H
#define CLIENT_H

#include "lib.h"
#include "group_chat.h"
#include "private_chat.h"
#include "client_function.h"

#define NOT_SELECTED 0
#define GROUP_CHAT 1
#define PRIVATE_CHAT 2

namespace Ui {
class client;
}

class client : public QWidget
{
    Q_OBJECT


private:
    Ui::client *ui;
    QTcpSocket *Client;
    int selection;
    string username;
    vector<group_chat*> groups;
    vector<private_chat*> private_rooms;
    QFile *file;
    QByteArray* fileBuffer;
    bool fileTransfering;
    int fileSize;
    int currentSize=0;
    string partner,fileName;

public:
    explicit client(QWidget *parent = nullptr);
    ~client();
    void closeEvent(QCloseEvent *event);

    void ListenServer();
    void SendMessageToServer(string message);
    void Disconnect();

    // login + signup
    void Solve_Login(string message);
    void Solve_Signup(string message);

    // group chat
    void Solve_Create_Group_Chat(string message);
    void Solve_Group_Chat(string message);
    void Solve_Add_User(string message);

    void Solve_Group_NewMember(string message);
    void Solve_Group_EndMember(string message);
    void PrintGroupUserList(string message);
    void End_Group_Chat(string message);

    //private chat
    void Solve_Create_Private_Chat(string message);
    void Solve_Private_Chat(string message);
    void End_Private_Chat(string message);


private slots:
    void on_loginButton_clicked();
    void on_groupChatButton_clicked();
    void on_privateChatButton_clicked();
    void on_ok_clicked();
    void on_signupButton_clicked();
    void on_backButton_clicked();

};


#endif // CLIENT_H

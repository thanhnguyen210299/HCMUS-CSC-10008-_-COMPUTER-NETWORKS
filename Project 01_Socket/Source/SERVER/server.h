#ifndef SERVER_H
#define SERVER_H

#include "lib.h"

namespace Ui {
class server;
}

class server : public QWidget
{
    Q_OBJECT

private:
    Ui::server *ui;
    QTcpServer *Server;
    vector<User> Clients;
    vector<GroupChat> g_chat;
    vector<PrivateChat> p_chat;
    string from,to;
    bool fileTransfering;
    int fileSize = 0;
    int currentSize = 0;
public:

    explicit server(QWidget *parent = nullptr);
    ~server();
    void load_Account_File();

    void NewConnection();
    void ListenClient();
    void ClientDisconnect();
    void SendMessageToClient(QTcpSocket *&client, string message);
    void SendMessageToClient(string name, string message);

    // server_account.cpp
    void Solve_Login(QTcpSocket *&client, string message);
    void Solve_Sign_Up(QTcpSocket *&client, string message);

    // server_ui.cpp
    void Print_Users_List();
    void Print_Group_List();
    void Print_Private_List();

    // server_GroupChat.cpp
    void Create_Group_Chat(QTcpSocket *&client, string message);
    void Solve_Group_Chat(string message);
    void Solve_Add_User(string message);
    void End_Group_Chat(string message);
    void Solve_Send_User_List(string groupname, string username);
    string Create_User_List_Delete(string groupname, string name);

    // server_PrivateChat.cpp
    void Create_Private_Chat(QTcpSocket *&client, string message);
    void Private_Chat(string message);
    void End_Private_Chat(string message);

    // server_helper.cpp
    static string fetchMessage(string message, int t);
    // login + signup
    bool Check_Username(string user);
    bool Check_Password(string name, string pass);

    void Add_User(string name, string pass, QTcpSocket *&client);
    void Update_User(string name, QTcpSocket *&client);

    bool isUserOnline(string name);
    // group chat
    bool is_Existed_Group(string name);
    bool is_Exist_User(string group_name, string username);
    void Add_Group_Chat(string name);
    bool Add_User_To_Group(string group, string username);
    void Delete_Group_Chat(string name);
    void Delete_User_From_Group(string groupname, string user);

    //private chat
    void Add_Private_Chat(string sender, string receiver);
    bool is_Existed_Private(string sender, string receiver);

    bool is_Existed_Client(QTcpSocket *&client);
    void DisconnectClient(string client);

};

#endif // SERVER_H

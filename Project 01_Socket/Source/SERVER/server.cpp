#include "server.h"
#include "ui_server.h"

server::server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::server)
{
    ui->setupUi(this);
    fileTransfering=false;
    currentSize=0;

    //Create server
    Server=new QTcpServer();
    if(!Server->listen(QHostAddress::Any, SERVER_PORT))
    {
        //thong bao ko khoi tao dc server
        ui->activityText->setText("FAIL TO CREATE SERVER");
        return;

    }
    Server->setObjectName(QString::fromStdString("SERVER"));
    //khoi tao server thanh cong
    ui->activityText->setText("SERVER STARTED");
    load_Account_File();
    connect(Server,&QTcpServer::newConnection,this,&server::NewConnection);

}

server::~server()
{
    delete ui;
}

void server::NewConnection()
{
    QTcpSocket* newClient = Server->nextPendingConnection();
    QObject::connect(newClient,&QTcpSocket::readyRead,this,&server::ListenClient);
    QObject::connect(newClient,&QTcpSocket::disconnected,this,&server::ClientDisconnect);
}

void server::ListenClient()
{
    QTcpSocket* currentClient = static_cast<QTcpSocket*>(sender());
    QByteArray mess = currentClient->readAll();
    string message=mess.toStdString();

    if (fileTransfering)
    {
        for (int i = 0; i < Clients.size(); i++)
            if (Clients[i].username == to)
            {
                Clients[i].socket->write(mess);
                currentSize += mess.size();

                if (currentSize>=fileSize)
                {
                    fileTransfering=false;
                    currentSize=0;
                }
                break;
            }
        return;
    }

    string flag=fetchMessage(message,1);

    if (flag=="SEND_FILE")
    {
        string fileName=fetchMessage(message,4);
        string receiver=fetchMessage(message,3);
        string size=fetchMessage(message,5);
        fileSize=QString::fromStdString(size).toInt();
        currentSize=0;
        from = fetchMessage(message,2);
        to = receiver;

        ui->activityText->append(QString::fromStdString(from + " sent a file named " +fileName+" to " + to));
        SendMessageToClient(receiver,"[SERVER_SEND_FILE]["+from+"]["+fileName+"]["+size+"]");
        fileTransfering=true;
    }
    else
    {
        fileTransfering=false;
        if (flag == "LOGIN")
            Solve_Login(currentClient, message);
        else
            if (flag == "SIGN_UP")
                Solve_Sign_Up(currentClient, message);
        else
            if (flag == "CREATE_GROUP_CHAT")
                Create_Group_Chat(currentClient,message);
        else
            if (flag == "GROUP_CHAT")
                Solve_Group_Chat(message);
        else
            if (flag == "ADD_USER")
                Solve_Add_User(message);
        else
            if (flag == "END_GROUP_CHAT")
                End_Group_Chat(message);
        else
            if (flag=="CREATE_PRIVATE_CHAT")
                Create_Private_Chat(currentClient,message);
        else
            if (flag=="PRIVATE_CHAT")
                Private_Chat(message);
        else
            if (flag=="END_PRIVATE_CHAT")
                End_Private_Chat(message);
    }
}

void server::SendMessageToClient(QTcpSocket *&client, string message)
{
    message+= "`";
    QByteArray byteArray(message.c_str(), message.length());
    client->write(byteArray);
}

void server::SendMessageToClient(string name, string message)
{
    message+="`";
    for (int i = 0; i < Clients.size(); i++)
        if (Clients[i].username == name)
        {
            QByteArray byteArray(message.c_str(), message.length());
            Clients[i].socket->write(byteArray);
            break;
        }
}

bool server::is_Existed_Client(QTcpSocket *&client)
{
    for (int i=0; i < Clients.size(); i++)
        if (client->objectName().toStdString() == Clients[i].username && client == Clients[i].socket)
            return true;
    return false;
}

void server::DisconnectClient(string client)
{
    int len = Clients.size();
    int pos;
    for (pos = 0; pos < Clients.size(); pos++)
        if (Clients[pos].username == client)
            Clients[pos].online=false;
}

void server::ClientDisconnect()
{
    QTcpSocket* currentClient = static_cast<QTcpSocket*>(sender());

    if (!is_Existed_Client(currentClient))
            return;

    string name=currentClient->objectName().toStdString();
    string mess="";

    vector<string> private_user;
    for (int i = 0; i < p_chat.size(); i++)
    {
        if (p_chat[i].client1 == name)
            private_user.push_back(p_chat[i].client2);
        if (p_chat[i].client2 == name)
            private_user.push_back(p_chat[i].client1);
    }
    // delete
    for (int i = 0; i < private_user.size(); i++)
    {
        mess = "[END_PRIVATE_CHAT][" + name + "][" + private_user[i] + "]";
        End_Private_Chat(mess);
    }

    vector<string> group_user;
    for (int i = 0; i < g_chat.size(); i++)
    {
        for (int j = 0; j < g_chat[i].users.size(); j++)
        {
            if (name == g_chat[i].users[j].username)
            {
                group_user.push_back(g_chat[i].group_name);
                break;
            }
        }
    }
    //delete
    for (int i = 0; i < group_user.size(); i++)
    {
        mess = "[END_GROUP_CHAT][" + group_user[i] + "][" + name + "]";
        End_Group_Chat(mess);
    }

    ui->activityText->append(currentClient->objectName() + QString::fromStdString(" is disconected"));

    // delete client
    DisconnectClient(currentClient->objectName().toStdString());
    Print_Users_List();
}

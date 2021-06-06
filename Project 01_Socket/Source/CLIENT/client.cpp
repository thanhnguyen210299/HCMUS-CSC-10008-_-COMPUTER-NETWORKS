#include "client.h"
#include "ui_client.h"

client::client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
    ui->groupChatButton->hide();
    ui->privateChatButton->hide();
    ui->type->hide();
    ui->name->hide();
    ui->ok->hide();
    ui->backButton->hide();
    ui->accountText->hide();
    fileTransfering=false;
    currentSize=0;

    //Create client
    Client=new QTcpSocket();

    //Connect to server
    Client->abort();
    Client->connectToHost(SERVER_IP,SERVER_PORT);

    if (!Client->waitForConnected())
    {
        QMessageBox::warning(this, "Server", "Cannot connect with server", QMessageBox::Ok);
        ui->loginButton->setEnabled(false);
        ui->signupButton->setEnabled(false);
        Client->close();
    }

    //ket noi duoc
    QObject::connect(Client,&QTcpSocket::readyRead,this,&client::ListenServer);
    QObject::connect(Client,&QTcpSocket::disconnected,this,&client::Disconnect);


}

client::~client()
{
    delete ui;
    delete Client;
}

void client::closeEvent(QCloseEvent *event)
{
    for (int i = 0; i < groups.size(); i++)
        groups[i]->close();
    for (int i = 0; i <private_rooms.size(); i++)
        private_rooms[i]->close();
    Client->close();
    event->accept();
}

void client::Disconnect()
{
    if (!Client->isOpen())
        return;
    if (!Client->waitForConnected())
    {
        QMessageBox::warning(this, "Server", "Cannot connect with server", QMessageBox::Ok);

        for (int i = 0; i < groups.size(); i++)
            groups[i]->close();
        for (int i = 0; i <private_rooms.size(); i++)
            private_rooms[i]->close();

        this->setVisible(false);
    }
    Client->close();
}

void client::ListenServer()
{
    QByteArray mess = Client->readAll();
    QString msg = mess;
    if (!msg.isEmpty())
    {
        if (fileTransfering)
        {
            fileBuffer->append(mess);
            currentSize+=mess.size();
            if (currentSize>=fileSize)
            {
                file->write(*fileBuffer);
                file->close();
                fileBuffer->clear();

                fileTransfering=false;
                currentSize=0;

                for (int i =0;i< private_rooms.size(); i++)
                    if (private_rooms[i]->getPartner() == partner)
                    {
                        private_rooms[i]->print(username + " sent you a file named "+fileName);
                        break;
                    }
            }
            return;
        }

        fileTransfering=false;
        vector<string> a = split_package(msg.toStdString());
        for (int i=0; i<a.size(); i++)
        {
            string message = a[i];
            string flag=fetchMessage(message,1);

            if (flag == "LOGIN") Solve_Login(message);
            else
                if (flag == "SIGN_UP")
                    Solve_Signup(message);
            else
                if (flag=="CREATE_GROUP_CHAT")
                    Solve_Create_Group_Chat(message);
            else
                if (flag=="GROUP_CHAT")
                    Solve_Group_Chat(message);
            else
                if (flag=="ADD_USER")
                    Solve_Add_User(message);
            else
                if (flag == "GROUP_CHAT_NEW_MEMBER")
                    Solve_Group_NewMember(message);
            else
                if (flag == "GROUP_CHAT_END_USER")
                    Solve_Group_EndMember(message);
            else
                if (flag == "USER_LIST")
                    PrintGroupUserList(message);
            else
                if (flag == "END_GROUP_CHAT")
                    End_Group_Chat(message);
            else
                if (flag=="CREATE_PRIVATE_CHAT")
                    Solve_Create_Private_Chat(message);
            else
                if (flag=="PRIVATE_CHAT")
                    Solve_Private_Chat(message);
            else
                if (flag=="END_PRIVATE_CHAT")
                    End_Private_Chat(message);
            else
                if (flag=="SERVER_SEND_FILE")
                {
                    fileName=fetchMessage(message,3);
                    partner = fetchMessage(message, 2);

                    file = new QFile(QString::fromStdString(fileName));
                    file->open(QIODevice::WriteOnly);

                    QString size=QString::fromStdString(fetchMessage(message,4));
                    fileSize=size.toInt();
                    fileBuffer = new QByteArray();
                    fileTransfering=true;
                    currentSize=0;
                }
        }
    }
}

void client::SendMessageToServer(string message)
{
    QByteArray byteArray(message.c_str(), message.length());
    Client->write(byteArray);
}

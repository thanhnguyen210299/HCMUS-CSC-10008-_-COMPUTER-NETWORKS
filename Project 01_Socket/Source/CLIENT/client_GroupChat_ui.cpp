#include "group_chat.h"
#include "ui_group_chat.h"

group_chat::group_chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::group_chat)
{
    ui->setupUi(this);

    //Create client
    groupSocket=new QTcpSocket();

    //Connect to server
    groupSocket->abort();
    groupSocket->connectToHost(SERVER_IP,SERVER_PORT);
    groupSocket->setObjectName(QString::fromStdString(clientName));
}

group_chat::~group_chat()
{
    delete ui;
    delete groupSocket;
}

void group_chat::Show()
{
    ui->groupNameLine->setText(QString::fromStdString(name));
    ui->usernameLine->setText(QString::fromStdString(clientName));
    groupSocket->setObjectName(QString::fromStdString(clientName));
    show();
}

void group_chat::on_send_clicked()
{
    string content=ui->messageBox->toPlainText().toStdString();
    string message="[GROUP_CHAT]["+name+"]["+clientName+"]["+content+"]";
    groupSocket->setObjectName(QString::fromStdString(clientName));

    //gui goi tin cho server: [GROUP_CHAT][groupname][username][content]
    QByteArray byteArray(message.c_str(), message.length());
    groupSocket->write(byteArray);
    ui->messageBox->setText("");
}

void group_chat::on_addUser_clicked()
{
    //[ADD_USER][groupname][sender][new_user]
    string newUser=ui->newUser->text().toStdString();
    string message="[ADD_USER]["+name+"]["+clientName+"]["+newUser+"]";
    groupSocket->setObjectName(QString::fromStdString(clientName));

    QByteArray byteArray(message.c_str(), message.length());
    groupSocket->write(byteArray);
    ui->newUser->setText("");
}

void group_chat::on_privateChatButton_clicked()
{
    groupSocket->setObjectName(QString::fromStdString(clientName));
    string partner = ui->userList->currentItem()->text().toStdString();
    //ui->chatWindow->append(QString::fromStdString("Client Name: " + clientName));
    if (partner == clientName + " (You)")
        return;
    else
        ui->userList->currentItem()->setBackgroundColor(Qt::blue);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Private Chat", QString::fromStdString("Create Private Chat with " + partner + "?"), QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        //ui->chatWindow->append(QString::fromStdString("Client Name: ") + groupSocket->objectName());
        string message="[CREATE_PRIVATE_CHAT]["+ groupSocket->objectName().toStdString()+"]["+partner+"]";
        QByteArray byteArray(message.c_str(), message.length());
        groupSocket->write(byteArray);
    }
    ui->userList->currentItem()->setBackgroundColor(Qt::white);
}


void group_chat::closeEvent(QCloseEvent *event)
{
    groupSocket->setObjectName(QString::fromStdString(clientName));
    string message="[END_GROUP_CHAT]["+name+"]["+clientName+"]";
    QByteArray byteArray(message.c_str(), message.length());
    groupSocket->write(byteArray);
    event->accept();
}

void group_chat::print(string s)
{
    ui->chatWindow->setTextColor(Qt::black);
    ui->chatWindow->append(QString::fromStdString(s));
}

void group_chat::print_mess(string s)
{
    ui->chatWindow->setTextColor(Qt::blue);
    ui->chatWindow->append(QString::fromStdString(s));
}

void group_chat::print_Message_Add_Success()
{
    QMessageBox::information(this, "Message", "Add User Successful", QMessageBox::Ok);
}

void group_chat::print_Message_Add_Fail()
{
    QMessageBox::warning(this, "Warning", "Add User Fail", QMessageBox::Ok);
}

void group_chat::print_User_List(string message)
{
    /*message: [USER_LIST][groupname][user1][user2]..][usern]*/
    int j = 12;
    while (message[j] != ']')
        j++;
    j++;
    ui->userList->clear();
    string user = "";
    while (j < message.length())
    {
        if (message[j] == ']')
        {
            if (user == clientName)
                user = user + " (You)";
            ui->userList->addItem(QString::fromStdString(user));

            user="";
        }
        if (message[j] != '[' && message[j] != ']')
            user+=message[j];

        j++;
    }
}

void group_chat::print_User(string name)
{
    ui->userList->addItem(QString::fromStdString(name));
}


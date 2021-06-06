#include "private_chat.h"
#include "ui_private_chat.h"

private_chat::private_chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::private_chat)
{
    ui->setupUi(this);

    //Create client
    privateSocket=new QTcpSocket();

    //Connect to server
    privateSocket->abort();
    privateSocket->connectToHost(SERVER_IP,SERVER_PORT);
    privateSocket->setObjectName(QString::fromStdString(clientName));
}

private_chat::~private_chat()
{
    delete ui;
    delete privateSocket;
}

void private_chat::Show()
{
    ui->userText->setText(QString::fromStdString(clientName));
    ui->partnerText->setText(QString::fromStdString(partner));
    privateSocket->setObjectName(QString::fromStdString(clientName));
    show();
}

void private_chat::print(string s)
{
    ui->chatWindow->setTextColor(Qt::black);
    ui->chatWindow->append(QString::fromStdString(s));
}

void private_chat::on_send_clicked()
{
    string content=ui->messageBox->toPlainText().toStdString();
    //gui goi tin cho server: [PRIVATE_CHAT][sender][receiver][content]
    string message="[PRIVATE_CHAT]["+clientName+"]["+partner+"]["+content+"]";
    privateSocket->setObjectName(QString::fromStdString(clientName));

    QByteArray byteArray(message.c_str(), message.length());
    privateSocket->write(byteArray);
    ui->messageBox->setText("");

    ui->chatWindow->setTextColor(Qt::blue);
    ui->chatWindow->append(QString::fromStdString("You: "+content));
}

void private_chat::on_attach_clicked()
{
    string filePath =  QFileDialog::getOpenFileName(
              this,
              "Open Document",
              QDir::currentPath(),
              "All files (*.*)").toStdString();

    if (filePath=="") return;

    QFile file(QString::fromStdString(filePath));
    if (!file.open(QIODevice::ReadOnly)){
        QMessageBox msgBox;
        msgBox.setText("Cannot open file");
        msgBox.exec();
        return;
    }
    QFileInfo fileInfo(file);
    QString fileName = fileInfo.fileName();
    //string name=(fileName.right(fileName.length()-fileName.lastIndexOf('/')- 1)).toStdString();
    string name=fileName.toStdString();
    int fileSize = static_cast<int>(file.size());

 //   if(fileSize < 2 * 1048576 || fileSize > 5 * 1048576){
    if (fileSize>5*1048576)
       {
        QMessageBox msgBox;
        msgBox.setText("Valid file size is 2 - 5MB.");
        msgBox.exec();
        return ;
    }
    else{
        //message: [SEND_FILE][sender][receiver][filename]
        string FileSize = QString::number(fileSize).toStdString();
        string message="[SEND_FILE]["+clientName+"]["+partner+"]["+name+"]["+FileSize+"]";
        QByteArray byteArray(message.c_str(), message.length());
        privateSocket->write(byteArray);
        //ui->chatWindow->append(QString::number(QString::fromStdString(FileSize).toInt()));
        //send file content
        QByteArray fileContent = file.readAll();
        file.close();
        QMessageBox::information(this, "Information", "Sent file successful", QMessageBox::Ok);
        ui->chatWindow->append("You sent a file named "+fileName);
        qint64 x = 0;
        while (x < fileContent.size()) {
            qint64 y = privateSocket->write(fileContent);
            x += y;
        }
    }
}


void private_chat::closeEvent(QCloseEvent *event)
{
    privateSocket->setObjectName(QString::fromStdString(clientName));
    string message="[END_PRIVATE_CHAT]["+clientName+"]["+partner+"]";
    QByteArray byteArray(message.c_str(), message.length());
    privateSocket->write(byteArray);
    event->accept();
}

void private_chat::disable()
{
    ui->send->setEnabled(false);
    ui->attach->setEnabled(false);
    ui->messageBox->setEnabled(false);
}

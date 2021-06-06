#ifndef GROUP_CHAT_H
#define GROUP_CHAT_H

#include "lib.h"

namespace Ui {
class group_chat;
}

class group_chat : public QWidget
{
    Q_OBJECT

public:
    explicit group_chat(QWidget *parent = nullptr);
    ~group_chat();
    void setName(string s)
    {
        name=s;
    }
    void setClientName(string s)
    {
        clientName=s;
    }

    void Show();
    string getName() {return name;}
    string getClientName() {return clientName;}
    void print(string s);
    void print_mess(string s);

    void print_Message_Add_Success();
    void print_Message_Add_Fail();

    void print_User_List(string message);
    void print_User(string name);

private slots:
    void on_send_clicked();
    void on_addUser_clicked();
    void closeEvent(QCloseEvent *event);
    void on_privateChatButton_clicked();

private:
    Ui::group_chat *ui;
    string name, clientName;
    QTcpSocket *groupSocket;
};

#endif // GROUP_CHAT_H

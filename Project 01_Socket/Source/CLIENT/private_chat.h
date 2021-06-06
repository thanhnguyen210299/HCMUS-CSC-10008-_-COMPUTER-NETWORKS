#ifndef PRIVATE_CHAT_H
#define PRIVATE_CHAT_H

#include "lib.h"

namespace Ui {
class private_chat;
}

class private_chat : public QWidget
{
    Q_OBJECT

public:
    explicit private_chat(QWidget *parent = nullptr);
    ~private_chat();

    void setClientName(string name){ clientName = name;}
    void setPartner(string name){ partner = name;}
    string getClientName(){return clientName;}
    string getPartner(){return partner;}


    void Show();
    void print(string s);
    void disable();

private slots:
    void on_send_clicked();
    void on_attach_clicked();
    void closeEvent(QCloseEvent *event);

private:
    Ui::private_chat *ui;
    string clientName;
    string partner;
    QTcpSocket *privateSocket;
};

#endif // PRIVATE_CHAT_H

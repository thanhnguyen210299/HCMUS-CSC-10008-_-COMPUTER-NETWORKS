#include "client.h"
#include "ui_client.h"

void client::on_loginButton_clicked()
{
    //Gui username va password cho server
    Client->setObjectName(ui->usernameTextBox->text());
    string user = ui->usernameTextBox->text().toStdString();
    string pass = ui->passwordTextBox->text().toStdString();

    username = user;
    // Ma hoa password
    pass=decode(pass);

    SendMessageToServer("[LOGIN]["+user+"]["+pass+"]");
}

void client::on_signupButton_clicked()
{
    //Gui username va password cho server
    //ui->privateChatButton->show();
    string user = ui->usernameTextBox->text().toStdString();
    string pass = ui->passwordTextBox->text().toStdString();

    // Ma hoa password
    pass = decode(pass);

    SendMessageToServer("[SIGN_UP]["+user+"]["+pass+"]");

}

void client::on_groupChatButton_clicked()
{
    selection=GROUP_CHAT;
    ui->groupChatButton->hide();
    ui->privateChatButton->hide();
    ui->type->show();
    ui->type->setText("Enter group name:");
    ui->name->show();
    ui->name->setText("");
    ui->ok->show();
    ui->backButton->show();
}

void client::on_backButton_clicked()
{
    ui->type->hide();
    ui->name->hide();
    ui->name->setText("");
    ui->ok->hide();
    ui->backButton->hide();
    ui->groupChatButton->show();
    ui->privateChatButton->show();
}

void client::on_privateChatButton_clicked()
{
    selection=PRIVATE_CHAT;
    ui->groupChatButton->hide();
    ui->privateChatButton->hide();
    ui->type->show();
    ui->type->setText("Start private chat with:");
    ui->name->show();
    ui->name->setText("");
    ui->ok->show();
    ui->backButton->show();
}

void client::on_ok_clicked()
{
    if (selection==GROUP_CHAT)
        SendMessageToServer("[CREATE_GROUP_CHAT]["+ui->name->toPlainText().toStdString()+"]");
    else
        SendMessageToServer("[CREATE_PRIVATE_CHAT]["+username+"]["+ui->name->toPlainText().toStdString()+"]");
}

#include "client.h"
#include "ui_client.h"

void client::Solve_Login(string message)
{
    string status=fetchMessage(message,2);
    if (status=="SUCCESS")
    {
        //login thanh cong -> hien cua so Selection
        username = ui->usernameTextBox->text().toStdString();

        ui->groupChatButton->show();
        ui->privateChatButton->show();
        ui->accountText->show();
        ui->accountText->setText(QString::fromStdString("Username: " + username));

        ui->loginLabel->hide();
        ui->usernameTextBox->hide();
        ui->passwordTextBox->hide();
        ui->loginButton->hide();
        ui->signupButton->hide();
        ui->username->hide();
        ui->password->hide();
    }
    if (status == "NOT_EXIST")
    {
        QMessageBox::warning(this, "Warning", "Username is not existed", QMessageBox::Ok);
        ui->usernameTextBox->setText("");
        ui->passwordTextBox->setText("");
    }
    if (status == "PASS_WRONG")
    {
        QMessageBox::warning(this, "Warning", "Wrong password!", QMessageBox::Ok);
        ui->passwordTextBox->setText("");
    }
    if (status == "EXISTED")
    {
        QMessageBox::warning(this, "Warning", "User is online", QMessageBox::Ok);
        ui->passwordTextBox->setText("");
        ui->usernameTextBox->setText("");
    }
}

void client::Solve_Signup(string message)
{
    string status=fetchMessage(message,2);
    if (status=="SUCCESS")
    {
        QMessageBox::information(this, "Message", "Sign up successful", QMessageBox::Ok);
    }
    if (status == "FAIL")
    {
        QMessageBox::warning(this, "Warning", "Username is existed", QMessageBox::Ok);
        ui->usernameTextBox->setText("");
        ui->passwordTextBox->setText("");
    }
}

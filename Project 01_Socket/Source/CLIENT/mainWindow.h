#ifndef SELECTION_FORM_H
#define SELECTION_FORM_H

#include <QWidget>

namespace Ui {
class Selection_Form;
}

#define NOT_SELECTED 0
#define GROUP_CHAT 1
#define PRIVATE_CHAT 2

class Selection_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Selection_Form(QWidget *parent = nullptr);
    ~Selection_Form();

private slots:
    void on_groupChatButton_clicked();
    void on_privateChatButton_clicked();
    void on_ok_clicked();

private:
    Ui::Selection_Form *ui;
    int selection;
};

#endif // SELECTION_FORM_H

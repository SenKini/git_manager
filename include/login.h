#pragma once
#include <QDialog>
#include <QString>
#include "ui_login.h"

class Login : public QDialog {
    Q_OBJECT

private:
    Ui::LoginClass _ui;

public:
    Login(QWidget *parent = nullptr);
    ~Login();
    void onOkCliked();

Q_SIGNALS:
    void ok(QString token);
};

#include "login.h"

Login::Login(QWidget *parent)
    : QDialog(parent) {
    _ui.setupUi(this);

    _ui.promptLabel->setVisible(false);

    connect(_ui.okButton, &QPushButton::clicked, this, &Login::onOkCliked);
}

Login::~Login() {}

void Login::onOkCliked() {
    QString token(_ui.tokenLine->text());

    if (!token.isEmpty()) {
        emit ok(token);
        this->close();
    }
    else
        _ui.promptLabel->setVisible(true);
}

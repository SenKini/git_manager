#include "login.h"
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>

Login::Login(QWidget *parent)
	: QDialog(parent) {
	ui.setupUi(this);

	ui.promptLabel->setVisible(false);

	connect(ui.okButton, &QPushButton::clicked, this, &Login::onOkCliked);
}

Login::~Login() {}

void Login::onOkCliked() {
	QString token(ui.tokenLine->text());

	if (!token.isEmpty()) {
		emit ok(token);
		this->close();
	}
	else
		ui.promptLabel->setVisible(true);
	
}

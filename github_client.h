#pragma once
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QByteArray>
#include "module.h"

class GitHubClient : public QObject {
	Q_OBJECT

private:
	QString _token;							  // �û� token
	QJsonDocument _baseInfo;				  // �û�������Ϣ
	QJsonDocument _reposInfo;				  // Զ�ֿ̲���Ϣ
	QByteArray _headImage;					  // �û�ͷ����Ϣ
	QNetworkAccessManager _baseInfoManager;	  // ���ͻ�����Ϣ����
	QNetworkAccessManager _repoInfoManager;	  // ���Ͳֿ���Ϣ����
	QNetworkAccessManager _headImageManager;  // ����ͷ����������

	void initRequest(QNetworkRequest &request);	 // ��ʼ������

public:
	GitHubClient(const QString token);
	bool init();					   // ��ʼ��
	bool updateBaseInfo();			   // ���»�����Ϣ
	bool updateRepoInfo();			   // ���²ֿ���Ϣ
	QStringList getRemoteReposName();  // ��ȡԶ�ֿ̲���
	QString getLoginName();			   // ��ȡ��¼��
	int getFollowerNum();			   // ��ȡ��˿��
	QByteArray getHeadImage();		   // ��ȡͷ������

Q_SIGNALS:
	void hadBaseInfo();	 // �Ѿ���ȡ�˻�����Ϣ
	void hadRepoInfo();	 // �Ѿ���ȡ�˲ֿ���Ϣ
};

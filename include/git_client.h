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
#include "error_mess.h"
#include "logger.h"

class GitClient : public QObject {
    Q_OBJECT

protected:
    QString _token;            // �û� token
    QJsonDocument _baseInfo;   // �û�������Ϣ
    QJsonDocument _reposInfo;  // Զ�ֿ̲���Ϣ
    QByteArray _headImage;     // �û�ͷ����Ϣ

    virtual void initRequest(QNetworkRequest &request) = 0;  // ��ʼ������
    virtual QString getInfoUrl() = 0;              // ��ȡ������Ϣ������ URL
    virtual QString getRepoUrl() = 0;              // ��ȡ�ֿ���Ϣ������ URL

public:
    GitClient(const QString t);
    bool init();                       // ��ʼ��
    bool updateBaseInfo();             // ���»�����Ϣ
    bool updateRepoInfo();             // ���²ֿ���Ϣ
    QStringList getRemoteReposName();  // ��ȡԶ�ֿ̲���
    QString getLoginName();            // ��ȡ��¼��
    int getFollowerNum();              // ��ȡ��˿��
    QByteArray getHeadImage();         // ��ȡͷ������

Q_SIGNALS:
    void hadBaseInfo();  // �Ѿ���ȡ�˻�����Ϣ
    void hadRepoInfo();  // �Ѿ���ȡ�˲ֿ���Ϣ
};
#pragma once
#include <QString>
#include "git_client.h"

class GithubClient : public GitClient {
protected:
    virtual void initRequest(QNetworkRequest &request);  // ��ʼ������
    virtual QString getInfoUrl();                        // ��ȡ������Ϣ������ URL
    virtual QString getRepoUrl();                        // ��ȡ�ֿ���Ϣ������ URL

public:
    GithubClient(const QString t);
};
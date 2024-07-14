#pragma once
#include "git_client.h"

class GiteeClient : public GitClient {
protected:
    virtual void initRequest(QNetworkRequest &request);  // ��ʼ������
    virtual QString getInfoUrl();                        // ��ȡ������Ϣ������ URL
    virtual QString getRepoUrl();                        // ��ȡ�ֿ���Ϣ������ URL

public:
    GiteeClient(const QString t);
};
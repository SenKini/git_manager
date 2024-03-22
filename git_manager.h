#pragma once
#include <QtWidgets/QMainWindow>
#include <QScopedPointer>
#include <QPixmap>
#include <QStringListModel>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QVector>
#include <QDataStream>
#include <QDesktopServices>
#include "github_client.h"
#include "ui_git_manager.h"
#include "login.h"
#include "github_client.h"
#include "git.h"
#include "persistence.h"
#include "repo_item.h"
#include "local_repos_info.h"

class GitManager : public QMainWindow {
	Q_OBJECT

private:
	Ui::git_managerClass _ui;
	GitHubClient *_githubClient;	  // �� GitHub API ��������
	QStringList _localRepos;		  // ���زֿ�·��
	QString _curWebsite;//��ǰ��¼����վ
	QString _token;
	Persistence _persist;			  // �־û�
	LocalReposInfo *_localReposInfo;  // ���زֿ���Ϣ

	void updateLocalRepos();  // ���±��زֿ���ʾ
	void updateBaseInfo();	  // ���»�����Ϣ
	void updateRepoInfo();	  // ���²ֿ���Ϣ

public:
	GitManager(QWidget *parent = nullptr);
	~GitManager();

public Q_SLOTS:
	void onLoginClicked();						 // ��¼�¼�
	void onHadBaseInfo();						 // �ѻ�ȡ������Ϣ�¼�
	void onHadRepoInfo();						 // �ѻ�ȡԶ�ֿ̲���Ϣ�¼�
	void onRefreshRemoteRepoClicked();			 // ˢ��Զ�ֿ̲��¼�
	void onAddLocalRepoClicked();				 // ��ӱ��زֿ��¼�
	void onCommitClicked();						 // �ύ�¼�
	void onPushClicked();						 // �����¼�
	void onPullClicked();						 // ��ȡ�¼�
	void onLookOverClicked();					 // �鿴�¼�
	void onRemoveRepoClicked(QString repoName);	 // �Ƴ����زֿ��¼�
	void onLocalRepoClicked();
};

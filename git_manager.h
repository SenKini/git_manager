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
	GitHubClient *_githubClient;	  // 向 GitHub API 发送请求
	QStringList _localRepos;		  // 本地仓库路径
	QString _curWebsite;//当前登录的网站
	QString _token;
	Persistence _persist;			  // 持久化
	LocalReposInfo *_localReposInfo;  // 本地仓库信息

	void updateLocalRepos();  // 更新本地仓库显示
	void updateBaseInfo();	  // 更新基本信息
	void updateRepoInfo();	  // 更新仓库信息

public:
	GitManager(QWidget *parent = nullptr);
	~GitManager();

public Q_SLOTS:
	void onLoginClicked();						 // 登录事件
	void onHadBaseInfo();						 // 已获取基本信息事件
	void onHadRepoInfo();						 // 已获取远程仓库信息事件
	void onRefreshRemoteRepoClicked();			 // 刷新远程仓库事件
	void onAddLocalRepoClicked();				 // 添加本地仓库事件
	void onCommitClicked();						 // 提交事件
	void onPushClicked();						 // 推送事件
	void onPullClicked();						 // 拉取事件
	void onLookOverClicked();					 // 查看事件
	void onRemoveRepoClicked(QString repoName);	 // 移除本地仓库事件
	void onLocalRepoClicked();
};

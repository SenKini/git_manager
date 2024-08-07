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
#include "login.h"
#include "git.h"
#include "persistence.h"
#include "repo_item.h"
#include "local_repos_info.h"
#include "github_client.h"
#include "ui_git_manager.h"
#include "logger.h"

enum GitType {
    GITHUB,
    GITEE
};

struct GitClients {
    GitClient *client = nullptr;
    QString token;
};

class GitManager : public QMainWindow {
    Q_OBJECT

private:
    Ui::GitManagerClass _ui;
    GitType _gitType;                 // 当前是哪一个 git 平台
    QVector<GitClients> _clients{2};  // 两个 git 平台
    QStringList _localRepos;          // 本地仓库路径
    Persistence *_persist;            // 持久化
    LocalReposInfo *_localReposInfo;  // 本地仓库信息

    void updateLocalRepos();                 // 更新本地仓库显示
    void updateBaseInfo();                   // 更新基本信息
    void updateRepoInfo();                   // 更新仓库信息
    bool readInfoAll();                      // 读取本地信息
    bool saveInfoAll();                      // 保存信息到本地
    GitClient *newGitClient(QString token);  // 根据 _gitType 创建一个 GitClient
    void clearView();                        // 清除当前显示的内容

public:
    GitManager(QWidget *parent = nullptr);
    ~GitManager();

public Q_SLOTS:
    void onLoginClicked();                       // 登录事件
    void onHadBaseInfo();                        // 已获取基本信息事件
    void onHadRepoInfo();                        // 已获取远程仓库信息事件
    void onRefreshRemoteRepoClicked();           // 刷新远程仓库事件
    void onAddLocalRepoClicked();                // 添加本地仓库事件
    void onCommitClicked();                      // 提交事件
    void onPushClicked();                        // 推送事件
    void onPullClicked();                        // 拉取事件
    void onLookOverClicked();                    // 查看事件
    void onRemoveRepoClicked(QString repoName);  // 移除本地仓库事件
    void onLocalRepoClicked();                   // 查看本地仓库分支事件
    void onGithubClicked();                      // 切换到 Github
    void onGiteeClicked();                       // 切换到 Gitee
};

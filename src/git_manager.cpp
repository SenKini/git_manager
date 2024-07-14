#include "git_manager.h"

extern Logger *logger;

void GitManager::updateLocalRepos() {
    logger->addLog("info", "update local repos view");
    _ui.localReposList->clear();  // 先清空显示

    // 重新读取并显示
    for (auto repo : _localRepos) {
        QFileInfo repoDir(repo);  // 用于提取仓库目录名
        auto item = new QListWidgetItem;
        auto repo = new RepoItem(repoDir.baseName());

        item->setSizeHint(QSize(130, 40));
        _ui.localReposList->addItem(item);
        repo->setFixedSize(QSize(130, 40));
        _ui.localReposList->setItemWidget(item, repo);

        connect(repo, &RepoItem::removeRepo, this, &GitManager::onRemoveRepoClicked);
    }
    _ui.localReposList->update();
}

void GitManager::updateBaseInfo() {
    logger->addLog("info", "update base info view");
    QPixmap pixmap;  // 存放头像数据
    pixmap.loadFromData(_clients[_gitType].client->getHeadImage());

    // 更新头像和名字
    _ui.headImageLabel->setPixmap(pixmap);
    _ui.headImageLabel->setScaledContents(true);  // 头像尺寸自适应
    _ui.headImageLabel->update();
    _ui.loginNameLabel->setText(_clients[_gitType].client->getLoginName());
    _ui.loginNameLabel->update();
}

void GitManager::updateRepoInfo() {
    logger->addLog("info", "update repos info view");
    QFont font;
    QStringList remoteReposName(_clients[_gitType].client->getRemoteReposName());

    // 设置字体
    font.setPointSize(20);

    // 更新仓库数和粉丝数
    _ui.repoNumLabel->setText(QString::number(remoteReposName.size()));
    _ui.repoNumLabel->setAlignment(Qt::AlignCenter);
    _ui.repoNumLabel->setFont(font);
    _ui.repoNumLabel->update();
    _ui.followerNumLabel->setText(QString::number(_clients[_gitType].client->getFollowerNum()));
    _ui.followerNumLabel->setAlignment(Qt::AlignCenter);
    _ui.followerNumLabel->setFont(font);
    _ui.followerNumLabel->update();

    // 更新远程仓库列表
    _ui.remoteReposList->addItems(remoteReposName);
    _ui.remoteReposList->update();
}

bool GitManager::readInfoAll() {
    logger->addLog("info", "Read local saved info");
    QStringList tokens;

    if (_persist->readInfoByLocalFile(_localRepos, tokens)) {
        logger->addLog("info", "Successfully readed saved info");
        for (int i = 0; i < tokens.size(); i++)
            _clients[i].token = tokens[i];
        return true;
    }
    else
        logger->addLog("info", "Reading saved info failed");
    return false;
}

bool GitManager::saveInfoAll() {
    logger->addLog("info", "Saving info to local");
    QStringList tokens;

    for (auto c : _clients)
        if (!c.token.isEmpty())
            tokens.append(c.token);
    return _persist->saveInfoToLocalFile(_localRepos, tokens);
}

GitClient *GitManager::newGitClient(QString token) {
    switch (_gitType) {
        case GITHUB:
            return new GithubClient(token);
    }

    return nullptr;
}

void GitManager::clearView() {
    _ui.headImageLabel->clear();
    _ui.repoNumLabel->clear();
    _ui.followerNumLabel->clear();
    _ui.remoteReposList->clear();
    _ui.loginNameLabel->clear();
}

GitManager::GitManager(QWidget *parent)
    : QMainWindow(parent), _gitType(GITHUB), _persist(Persistence::getInstance()), _localReposInfo(nullptr) {
    _ui.setupUi(this);

    _ui.promptLabel->setVisible(false);  // 隐藏错误提示信息

    // 尝试读取保存在本地的信息
    if (readInfoAll()) {
        logger->addLog("info", "Read locally saved info");
        _clients[_gitType].client = newGitClient(_clients[_gitType].token);
        if (!_clients[_gitType].client->init())
            _ui.statusBar->showMessage("user info update failed", 5000);
        else {
            updateLocalRepos();
            connect(_clients[_gitType].client, &GitClient::hadBaseInfo, this, &GitManager::onHadBaseInfo);
            connect(_clients[_gitType].client, &GitClient::hadRepoInfo, this, &GitManager::onHadRepoInfo);
        }
    }

    connect(_ui.loginButton, &QPushButton::clicked, this, &GitManager::onLoginClicked);                      // 连接登录按钮
    connect(_ui.addLocalRepoButton, &QPushButton::clicked, this, &GitManager::onAddLocalRepoClicked);        // 连接添加按钮
    connect(_ui.commitButton, &QPushButton::clicked, this, &GitManager::onCommitClicked);                    // 连接提交按钮
    connect(_ui.pushButton, &QPushButton::clicked, this, &GitManager::onPushClicked);                        // 连接推送按钮
    connect(_ui.pullButton, &QPushButton::clicked, this, &GitManager::onPullClicked);                        // 连接拉取按钮
    connect(_ui.lookOverButton, &QPushButton::clicked, this, &GitManager::onLookOverClicked);                // 连接查看按钮
    connect(_ui.localReposAction, &QAction::triggered, this, &GitManager::onLocalRepoClicked);               // 连接本地仓库分支按钮
    connect(_ui.refreshRemoteButton, &QPushButton::clicked, this, &GitManager::onRefreshRemoteRepoClicked);  // 连接刷新远程仓库按钮
    connect(_ui.githubAction, &QAction::triggered, this, &GitManager::onGithubClicked);
    connect(_ui.giteeAction, &QAction::triggered, this, &GitManager::onGiteeClicked);

    connect(_ui.remoteReposList, &QListWidget::clicked, [&] {
        _ui.commitButton->setEnabled(false);
        _ui.pushButton->setEnabled(false);
        _ui.pullButton->setEnabled(false);
    });  // 当选择远程仓库时，禁止提交、推送、拉取按钮
    connect(_ui.localReposList, &QListWidget::clicked, [&] {
        _ui.commitButton->setEnabled(true);
        _ui.pushButton->setEnabled(true);
        _ui.pullButton->setEnabled(true);
    });  // 当选择本地仓库时，允许提交、推送、拉取按钮
}

GitManager::~GitManager() {
    saveInfoAll();
    delete _localReposInfo;
    for (auto c : _clients)
        if (c.client != nullptr)
            delete c.client;
}

void GitManager::onLoginClicked() {
    Login loginDialog(this);  // 登录对话框
    QString loginName;        // 登录名

    connect(&loginDialog, &Login::ok, this, [&](QString token) {
        logger->addLog("info", "Start login");
        _clients[_gitType].client = new GithubClient(token);
        _clients[_gitType].token = token;
        connect(_clients[_gitType].client, &GithubClient::hadBaseInfo, this, &GitManager::onHadBaseInfo);
        connect(_clients[_gitType].client, &GithubClient::hadRepoInfo, this, &GitManager::onHadRepoInfo);
    });  // 连接登录对话框的确认键

    loginDialog.exec();
}

void GitManager::onHadBaseInfo() {
    updateBaseInfo();
}

void GitManager::onHadRepoInfo() {
    updateRepoInfo();
}

void GitManager::onRefreshRemoteRepoClicked() {
    _clients[_gitType].client->updateRepoInfo();
    updateRepoInfo();

    _ui.statusBar->clearMessage();
    _ui.statusBar->showMessage("reflush", 5000);
}

void GitManager::onAddLocalRepoClicked() {
    QString newLocalRepo = QFileDialog::getExistingDirectory(this);  // 新添加的本地仓库
    QFileInfo newRepoDir(newLocalRepo);                              // 新仓库的路径信息

    // 更新新添加的仓库
    if (!newLocalRepo.isEmpty() && !_localRepos.contains(newLocalRepo)) {
        _localRepos.push_back(newLocalRepo);
        updateLocalRepos();
    }
}

void GitManager::onCommitClicked() {
    int repoIndex = _ui.localReposList->currentRow();
    QString mess = _ui.commitText->toPlainText();
    if (!mess.isEmpty()) {
        _ui.promptLabel->setVisible(false);
        if (repoIndex > -1) {
            Git git(_localRepos[repoIndex]);

            if (git.addAndCommit(mess))
                _ui.statusBar->showMessage("add and commit", 5000);
            else
                _ui.statusBar->showMessage("commit failed", 5000);
        }
        else
            _ui.statusBar->showMessage("no select repo", 5000);
    }
    else
        _ui.promptLabel->setVisible(true);
}

void GitManager::onPushClicked() {
    int repoIndex = _ui.localReposList->currentRow();

    if (repoIndex > -1) {  // 确定选中的是本地仓库
        Git git(_localRepos[repoIndex]);

        if (git.push())
            _ui.statusBar->showMessage("push", 5000);
        else
            _ui.statusBar->showMessage("push failed：" + errorMess, 5000);
    }
}

void GitManager::onPullClicked() {
    int repoIndex = _ui.localReposList->currentRow();

    if (repoIndex > -1) {  // 确定选中的是本地仓库
        Git git(_localRepos[repoIndex]);

        if (git.pull())
            _ui.statusBar->showMessage("pull", 5000);
        else
            _ui.statusBar->showMessage("pull failed：" + errorMess, 5000);
    }
}

void GitManager::onLookOverClicked() {
    int repoIndex;

    // TODO: 添加 Gitee 后，改为职责链模式
    if ((repoIndex = _ui.localReposList->currentRow()) > -1)
        QDesktopServices::openUrl(QUrl("file:" + _localRepos[repoIndex]));
    else if ((repoIndex = _ui.remoteReposList->currentRow()) > -1) {
        QString userName = _clients[_gitType].client->getLoginName();
        QString repoName = _ui.remoteReposList->currentItem()->text();
        QDesktopServices::openUrl(QUrl("http://github.com/" + userName + "/" + repoName));
    }
}

void GitManager::onRemoveRepoClicked(QString repoName) {
    for (auto repo : _localRepos) {
        QFileInfo repoDir(repo);
        if (repoDir.baseName() == repoName) {
            _localRepos.removeOne(repo);
            break;
        }
    }
    updateLocalRepos();
}

void GitManager::onLocalRepoClicked() {
    _localReposInfo = new LocalReposInfo(_localRepos);
    _localReposInfo->setWindowModality(Qt::ApplicationModal);  // 阻塞其他窗口
    _localReposInfo->show();
}

void GitManager::onGithubClicked() {
    logger->addLog("info", "Switch Github");
    _gitType = GITHUB;
    clearView();
    if (_clients[_gitType].client != nullptr) {
        updateBaseInfo();
        updateRepoInfo();
    }
}

void GitManager::onGiteeClicked() {
    logger->addLog("info", "Switch Gitee");
    _gitType = GITEE;
    clearView();
    if (_clients[_gitType].client != nullptr) {
        updateBaseInfo();
        updateRepoInfo();
    }
}

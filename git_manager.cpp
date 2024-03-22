#include "git_manager.h"

void GitManager::updateLocalRepos() {
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
	QPixmap pixmap;	 // 存放头像数据
	pixmap.loadFromData(_githubClient->getHeadImage());

	// 更新头像和名字
	_ui.headImageLabel->setPixmap(pixmap);
	_ui.headImageLabel->setScaledContents(true);  // 头像尺寸自适应
	_ui.headImageLabel->update();
	_ui.loginNameLabel->setText(_githubClient->getLoginName());
	_ui.loginNameLabel->update();
}

void GitManager::updateRepoInfo() {
	QFont font;
	QStringList remoteReposName(_githubClient->getRemoteReposName());

	// 设置字体
	font.setPointSize(20);

	// 更新仓库数和粉丝数
	_ui.repoNumLabel->setText(QString::number(remoteReposName.size()));
	_ui.repoNumLabel->setAlignment(Qt::AlignCenter);
	_ui.repoNumLabel->setFont(font);
	_ui.repoNumLabel->update();
	_ui.followerNumLabel->setText(QString::number(_githubClient->getFollowerNum()));
	_ui.followerNumLabel->setAlignment(Qt::AlignCenter);
	_ui.followerNumLabel->setFont(font);
	_ui.followerNumLabel->update();

	// 更新远程仓库列表
	_ui.remoteReposList->addItems(remoteReposName);
	_ui.remoteReposList->update();
}

GitManager::GitManager(QWidget *parent)
	: QMainWindow(parent), _githubClient(nullptr), _localReposInfo(nullptr) {
	_ui.setupUi(this);

	_ui.promptLabel->setVisible(false);	 // 隐藏错误提示信息

	if (_persist.updateInfoByLocalFile(_localRepos, _token) == OK) {
		_githubClient = new GitHubClient(_token);
		if (!_githubClient->init())
			_ui.statusBar->showMessage("个人信息更新失败", 5000);

		updateLocalRepos();
		connect(_githubClient, &GitHubClient::hadBaseInfo, this, &GitManager::onHadBaseInfo);
		connect(_githubClient, &GitHubClient::hadRepoInfo, this, &GitManager::onHadRepoInfo);
	}

	connect(_ui.loginButton, &QPushButton::clicked, this, &GitManager::onLoginClicked);				   // 连接登录按钮
	connect(_ui.addLocalRepoButton, &QPushButton::clicked, this, &GitManager::onAddLocalRepoClicked);  // 连接添加按钮
	connect(_ui.commitButton, &QPushButton::clicked, this, &GitManager::onCommitClicked);			   // 连接提交按钮
	connect(_ui.pushButton, &QPushButton::clicked, this, &GitManager::onPushClicked);				   // 连接推送按钮
	connect(_ui.pullButton, &QPushButton::clicked, this, &GitManager::onPullClicked);				   // 连接推送按钮
	connect(_ui.lookOverButton, &QPushButton::clicked, this, &GitManager::onLookOverClicked);		   // 连接推送按钮
	connect(_ui.localReposAction, &QAction::triggered, this, &GitManager::onLocalRepoClicked);
	connect(_ui.refreshRemoteButton, &QPushButton::clicked, this, &GitManager::onRefreshRemoteRepoClicked);

	connect(_ui.remoteReposList, &QListWidget::clicked, [&] {
		_ui.commitButton->setEnabled(false);
		_ui.pushButton->setEnabled(false);
		_ui.pullButton->setEnabled(false);
	});	 // 当选择远程仓库时，禁止提交、推送、拉取按钮
	connect(_ui.localReposList, &QListWidget::clicked, [&] {
		_ui.commitButton->setEnabled(true);
		_ui.pushButton->setEnabled(true);
		_ui.pullButton->setEnabled(true);
	});	 // 当选择本地仓库时，允许提交、推送、拉取按钮
}

GitManager::~GitManager() {
	delete _githubClient;
	delete _localReposInfo;
	_persist.saveInfoToLocalFile(_localRepos, _token);
}

void GitManager::onLoginClicked() {
	Login loginDialog(this);  // 登录对话框
	ErrCode errCode;		  // 错误码
	QString loginName;		  // 登录名

	connect(&loginDialog, &Login::ok, this, [&](QString token) {
		_githubClient = new GitHubClient(token);
		_token = token;
		connect(_githubClient, &GitHubClient::hadBaseInfo, this, &GitManager::onHadBaseInfo);
		connect(_githubClient, &GitHubClient::hadRepoInfo, this, &GitManager::onHadRepoInfo);
	});	 // 连接登录对话框的确认键

	loginDialog.exec();
}

void GitManager::onHadBaseInfo() {
	updateBaseInfo();
}

void GitManager::onHadRepoInfo() {
	updateRepoInfo();
}

void GitManager::onRefreshRemoteRepoClicked() {
	_githubClient->updateRepoInfo();
	updateRepoInfo();

	_ui.statusBar->clearMessage();
	_ui.statusBar->showMessage("刷新成功", 5000);
}

void GitManager::onAddLocalRepoClicked() {
	QString newLocalRepo = QFileDialog::getExistingDirectory(this);	 // 新添加的本地仓库
	QFileInfo newRepoDir(newLocalRepo);								 // 新仓库的路径信息

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
				_ui.statusBar->showMessage("添加并提交成功", 5000);
			else
				_ui.statusBar->showMessage("提交失败", 5000);
		}
		else
			_ui.statusBar->showMessage("没有选择仓库", 5000);
	}
	else
		_ui.promptLabel->setVisible(true);
}

void GitManager::onPushClicked() {
	int repoIndex = _ui.localReposList->currentRow();

	if (repoIndex > -1) {  // 确定选中的是本地仓库
		Git git(_localRepos[repoIndex]);

		if (git.push())
			_ui.statusBar->showMessage("推送成功", 5000);
		else
			_ui.statusBar->showMessage("推送失败：" + errorMess, 5000);
	}
}

void GitManager::onPullClicked() {
	int repoIndex = _ui.localReposList->currentRow();

	if (repoIndex > -1) {  // 确定选中的是本地仓库
		Git git(_localRepos[repoIndex]);

		if (git.pull() == OK)
			_ui.statusBar->showMessage("拉取成功", 5000);
		else
			_ui.statusBar->showMessage("拉取失败：" + errorMess, 5000);
	}
}

void GitManager::onLookOverClicked() {
	int repoIndex;

	//TODO: 添加 Gitee 后，改为职责链模式
	if ((repoIndex = _ui.localReposList->currentRow()) > -1)
		QDesktopServices::openUrl(QUrl("file:" + _localRepos[repoIndex]));
	else if ((repoIndex = _ui.remoteReposList->currentRow()) > -1) {
		QString userName = _githubClient->getLoginName();
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

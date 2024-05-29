#include "local_repos_info.h"

LocalReposInfo::LocalReposInfo(QStringList repos, QWidget *parent)
    : _localRepos(repos), QWidget(parent) {
    _ui.setupUi(this);

    for (auto repo : repos) {
        QFileInfo repoDir(repo);
        _ui.localReposList->addItem(repoDir.baseName());
    }
    _ui.localReposList->update();

    connect(_ui.localReposList, &QListWidget::itemClicked, this, &LocalReposInfo::onItemSelected);
    connect(_ui.setCurButton, &QPushButton::clicked, this, &LocalReposInfo::onSetCurClicked);
}

LocalReposInfo::~LocalReposInfo() {}

void LocalReposInfo::updateBranchs() {
    int index = _ui.localReposList->currentRow();
    if (index > -1) {
        Git git(_localRepos[index]);
        QStringList branchList;

        branchList = git.getBranchAll().split('\n');
        _ui.branchList->clear();
        _ui.branchList->addItems(branchList);
        _ui.branchList->update();
    }
}

void LocalReposInfo::onItemSelected(QListWidgetItem *item) {
    updateBranchs();
}

void LocalReposInfo::onSetCurClicked() {
    int index = _ui.localReposList->currentRow();
    if (index > -1) {
        Git git(_localRepos[index]);
        QListWidgetItem *item = _ui.branchList->currentItem();
        if (item) {
            git.setCurBranch(item->text());
            updateBranchs();
        }
    }
}

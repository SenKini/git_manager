#pragma once
#include <QWidget>
#include <QStringList>
#include <QFileInfo>
#include "ui_local_repos_info.h"
#include "git.h"

class LocalReposInfo : public QWidget {
	Q_OBJECT

private:
	Ui::LocalReposInfoClass _ui;
	QStringList _localRepos;
	QString _curRepo;

public:
	LocalReposInfo(QStringList repos, QWidget *parent = nullptr);
	~LocalReposInfo();
	void updateBranchs();

public Q_SLOTS:
	void onItemSelected(QListWidgetItem *item);
	void onSetCurClicked();
};

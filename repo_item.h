#pragma once
#include <QWidget>
#include <QPushButton>
#include <QString>
#include "ui_repo_item.h"

class RepoItem : public QWidget
{
	Q_OBJECT

public:
	RepoItem(QString text, QWidget *parent = nullptr);
	~RepoItem();

Q_SIGNALS:
	void removeRepo(QString repoName);

private:
	Ui::RepoItemClass _ui;
};

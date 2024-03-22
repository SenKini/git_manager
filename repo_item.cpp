#include "repo_item.h"

RepoItem::RepoItem(QString text, QWidget *parent)
	: QWidget(parent) {
	_ui.setupUi(this);
	_ui.repoName->setText(text);
	_ui.repoName->update();

	connect(_ui.remove, &QPushButton::clicked, [=] {
		emit removeRepo(_ui.repoName->text());
	});
}

RepoItem::~RepoItem() {}

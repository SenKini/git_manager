#include "git.h"

QString Git::getCurText() {
	QByteArray outData = _process.readAllStandardOutput();
	QStringList branches = QString(outData).split('\n');
	for (auto branch : branches)
		if (branch[0] == '*')
			return branch.mid(2);

	return QString();
}

bool Git::pushCommand(QStringList args) {
	_process.start("git", args);
	if (_process.waitForFinished() && (_process.exitCode() == 0))
		return true;
	else {
		errorMess = _process.errorString();
		return false;
	}
}

bool Git::updateCurBranch() {
	QStringList args;  // �������
	args << "branch";

	if (pushCommand(args)) {
		_curBranch = getCurText();
		return true;
	}
	else
		return false;
}

bool Git::updateCurRemote() {
	QStringList args;  // �������

	args << "remote";

	if (pushCommand(args)) {
		_curBranch = getCurText();
		return true;
	}
	else
		return false;
}

Git::Git(QString proc) {
	_process.setWorkingDirectory(proc);
}

bool Git::addAndCommit(QString mess) {
	QStringList args;  // �������
	args << "add" << ".";

	if (pushCommand(args)) {
		args.clear();
		args << "commit" << "-m" << mess;

		return pushCommand(args);
	}
	else
		return false;
}

bool Git::push() {
	if (updateCurBranch()) {
		QStringList args;  // �������
		args << "push" << _curRemote << _curBranch;

		return pushCommand(args);
	}
	else
		return false;
}

bool Git::pull() {
	if (updateCurBranch()) {
		QStringList args;  // �������

		args << "pull" << _curRemote << _curBranch;

		_process.start("git", args);
		if (_process.waitForStarted() && (_process.exitCode() == 0))
			return true;
		else
			errorMess = _process.errorString();
	}

	return false;
}

bool Git::clone(QString url) {
	QStringList args;  // �������
	args << "clone" << url;

	return pushCommand(args);
}

QString Git::getBranchAll() {
	QStringList args;  // �������
	args << "branch";

	return pushCommand(args) ? _process.readAllStandardOutput() : QString();
}

bool Git::setCurBranch(QString branch) {
	QStringList args;  // �������
	args << "checkout" << "-b" << branch;

	return pushCommand(args) ? updateCurBranch() : false;
}

#pragma once
#include <QString>
#include <QProcess>
#include "module.h"

class Git {
private:
	QString _curBranch;	 // ��ǰ��֧
	QString _curRemote;	 // ��ǰԶ�ֿ̲�
	QProcess _process;	 // system ����

	QString getCurText();				 // ��ȡ�б��е�ǰ��֧���ֿ�
	bool pushCommand(QStringList args);	 // �����̨����ָ��
	bool updateCurBranch();				 // ���µ�ǰ��֧
	bool updateCurRemote();				 // ���µ�ǰ�ֿ�

public:
	Git(QString proc);
	bool addAndCommit(QString mess);	// ��Ӳ��ύ
	bool push();						// �ϴ�
	bool pull();						// ��ȡ
	bool clone(QString url);			// ��¡
	QString getBranchAll();				// ��ȡ���з�֧
	bool setCurBranch(QString branch);	// ���õ�ǰ��֧
};

#pragma once
#include <qstring.h>
#include <qprocess.h>
#include "error_mess.h"

class Git {
private:
    QString _curBranch;  // ��ǰ��֧
    QString _curRemote;  // ��ǰԶ�ֿ̲�
    QProcess _process;   // ϵͳ����̨����

    QString getCurText();                   // ��ȡ git ָ��ص��б��е���Ϣ
    bool generalCommand(QStringList args);  // һ���ָ��ģ��
    bool updateCurBranch();                 // ���µ�ǰ��֧
    bool updateCurRemote();                 // ���µ�ǰ�ֿ�

public:
    Git(QString procDir);
    bool addAndCommit(QString mess);    // ��Ӳ��ύ
    bool push();                        // ����
    bool pull();                        // ��ȡ
    bool clone(QString url);            // ��¡
    QString getBranchAll();             // ��ȡ���з�֧
    bool setCurBranch(QString branch);  // ���õ�ǰ��֧
};

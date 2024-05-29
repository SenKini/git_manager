#include "git_manager.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    GitManager w;
    w.show();
    return a.exec();
}

#include "widget.h"
#include <QWidget>
#include <QPushButton>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.move(300, 300);
    w.show();

    return a.exec();
}

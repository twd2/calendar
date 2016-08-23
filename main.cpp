#include <QApplication>
#include <QDesktopWidget>

#include "widget.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QDesktopWidget *desktop = QApplication::desktop();
    QRect screen = desktop->screenGeometry();

    Widget *w = new Widget();
    w->resize(screen.width() / 3, screen.height() / 3);
    w->show();
    /*Calendar *c = new Calendar();
    c->resize(screen.width() / 3, screen.height() / 3);
    c->show();*/

    return app.exec();
}

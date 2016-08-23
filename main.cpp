#include <QApplication>
#include <QDesktopWidget>

#include "widget.h"
#include "storage.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QDesktopWidget *desktop = QApplication::desktop();
    QRect screen = desktop->screenGeometry();

    Storage::i()->load();

    /*Widget *w = new Widget();
    w->resize(screen.width() / 3, screen.height() / 3);
    w->show(); // */
    Calendar *c = new Calendar();
    c->resize(screen.width() * 2 / 3, screen.height() * 2 / 3);
    c->show(); // */

    Storage::i()->save();
    return app.exec();
}

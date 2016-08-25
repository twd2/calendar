#include "widget.h"
#include "storage.h"
#include "draggablelabel.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QTranslator>
#include <QtGlobal>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QTranslator trans;
    trans.load(":/locale/zh_CN.qm");
    app.installTranslator(&trans);

    QTranslator qtTrans;
    qtTrans.load(":/locale/qt_zh_CN.qm");
    app.installTranslator(&qtTrans);
#ifdef Q_OS_WIN
    QFont font;
    font.setFamily("Microsoft YaHei");
    app.setFont(font);
#endif

    QDesktopWidget *desktop = QApplication::desktop();
    QRect screen = desktop->screenGeometry();

    Storage::i()->load();

    /*Widget *w = new Widget();
    w->resize(screen.width() / 3, screen.height() / 3);
    w->show(); // */
    Calendar c;
    c.resize(screen.width() * 2 / 3, screen.height() * 2 / 3);
    c.setWindowTitle(QObject::tr("Wandai's Calendar"));
    c.show(); // */

    Storage::i()->save();
    return app.exec();
}

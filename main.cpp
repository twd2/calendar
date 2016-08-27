#include "storage.h"
#include "draggablelabel.h"
#include "global.h"
#include "calendar.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QTranslator>
#include <QtGlobal>
#include <QLocale>
#include <QIcon>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QLocale locale;
    qDebug() << locale.name().toLower();
    QTranslator trans, qtTrans;
    if (locale.name().toLower() == "zh_cn")
    {
        qDebug() << "loading zh_CN";
        trans.load(":/locale/zh_CN.qm");
        app.installTranslator(&trans);


        qtTrans.load(":/locale/qt_zh_CN.qm");
        app.installTranslator(&qtTrans);
    }
#ifdef Q_OS_WIN
    QFont font;
    font.setFamily("Microsoft YaHei");
    app.setFont(font);
#endif
    Storage::todo()->load();

    Calendar c;
    c.resize(Global::getScreen().width() * 2 / 3, Global::getScreen().height() * 2 / 3);
    c.setWindowTitle(QObject::tr("Wandai's Calendar"));
    QPixmap pixmap(":/icon/icon.png");
    QIcon icon(pixmap);
    c.setWindowIcon(icon);
    c.show();

    Storage::todo()->save();
    Storage::end();
    return app.exec();
}

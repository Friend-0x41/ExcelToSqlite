#include <QCoreApplication>
#include <QApplication>
#include <exceltool.h>
#include "sqlite3generatetool.h"
#include "sqliteprocessresult.h"
#include <QList>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    qRegisterMetaType<QList<SqliteProcessResult>>("QList<SqliteProcessResult>");
    qRegisterMetaType<QList<SqliteProcessResult>>("QList<SqliteProcessResult>&");
    Sqlite3GenerateTool widget;
    widget.show();
    return app.exec();
}

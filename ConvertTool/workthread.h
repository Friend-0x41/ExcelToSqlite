#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QObject>
#include <QList>
#include "sqliteprocessresult.h"
#include <QList>

class WorkThread : public QObject
{
    Q_OBJECT
public:
    explicit WorkThread(QString excel,QString database,QString scriptDir,QObject *parent = nullptr);
public slots:
    void run();
    void startProcess(int total);
    void updateProgress(int value);
    void onError(QString msg);
signals:
    void finshed(QList<SqliteProcessResult>);
    void start(int);
    void update(int value);
    void error(QString);
private:
    QString _excelPath;
    QString _databasePath;
    QString _scriptDir;
};

#endif // WORKTHREAD_H

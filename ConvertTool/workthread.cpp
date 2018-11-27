#include "workthread.h"
#include "exceltool.h"
#include "process.h"
#include <QThread>

WorkThread::WorkThread(QString excel,QString database,QString scriptDir,QObject *parent) : QObject(parent),
    _excelPath(excel),_databasePath(database),_scriptDir(scriptDir)
{

}


void WorkThread::run()
{
    ExcelTool excelTool(_excelPath,_databasePath,_scriptDir);
    connect(&excelTool,&ExcelTool::startProcess,this,&WorkThread::startProcess);
    connect(&excelTool,&ExcelTool::updateProgess,this,&WorkThread::updateProgress);
    connect(&excelTool,&ExcelTool::error,this,&WorkThread::onError);
    auto results = excelTool.Process();
    emit finshed(results);
    QThread::currentThread()->exit(0);
}

void WorkThread::startProcess(int total)
{
    emit start(total);
}

void WorkThread::updateProgress(int value)
{
    emit update(value);
}

void WorkThread::onError(QString msg)
{
    emit error(msg);
}

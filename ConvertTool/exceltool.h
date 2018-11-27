#ifndef EXCELTOOL_H
#define EXCELTOOL_H

#include <QString>
#include "log.h"
#include <memory>
#include "sqliteprocessresult.h"
#include <QObject>
#include <QList>

class QAxObject;

class ExcelTool : public Log
{
    Q_OBJECT
public:
    ExcelTool(const QString& excelPath,const QString& databasePath,const QString& scriptFilePath,QObject* parent = nullptr);
    ~ExcelTool();
    QList<SqliteProcessResult> Process();
signals:
    void startProcess(int);
    void updateProgess(int);
    void error(QString);
private:
    void initExcel();
    std::vector<std::vector<QString>> getExcelContents(std::shared_ptr<QAxObject> work_sheet);
    std::vector<QString> getPropertyies(std::shared_ptr<QAxObject> work_sheet,int row_start,int column_start,int column_count);

private:
    QString _excelPath;
    QString _databasePath;
    QString _scriptFilePath;
    QAxObject* _excel = nullptr;
};

#endif // EXCELTOOL_H

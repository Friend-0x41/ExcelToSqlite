#ifndef EXCELTOOL_H
#define EXCELTOOL_H

#include <QString>
#include "log.h"
#include <vector>
#include <memory>

class QAxObject;

class ExcelTool : public Log
{
public:
    ExcelTool(const QString& excelPath,const QString& databasePath,const QString& scriptFilePath);
    ~ExcelTool();
    void Process();

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

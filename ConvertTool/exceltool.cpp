#include "exceltool.h"
#include <QAxObject>
#include <exception>
#include <windows.h>
#include <sapi.h>
#include "sqlitetool.h"
#include <thread>
#include "csharptool.h"

ExcelTool::ExcelTool(const QString& excelPath,const QString& databasePath,const QString& scriptFilePath)
    :_excelPath(excelPath),
      _databasePath(databasePath),
      _scriptFilePath(scriptFilePath)
{
    initExcel();
}

ExcelTool::~ExcelTool()
{
    if(_excel != nullptr)
    {
        _excel->dynamicCall("Quit(void)");
        delete _excel;
    }
}

void ExcelTool::initExcel()
{
    OleInitialize(0);
    _excel = new QAxObject();
    _excel->setControl("Excel.Application");
    _excel->dynamicCall("SetVisible (bool Visible)","false");
    _excel->setProperty("DisplayAlerts",false);
}

void ExcelTool::Process()
{
    try{
        std::shared_ptr<QAxObject> _workBooks(_excel->querySubObject("WorkBooks"));
        std::shared_ptr<QAxObject> _workBook(_workBooks->querySubObject("Open(const QString &)",_excelPath));
        std::shared_ptr<QAxObject> _work_sheets(_workBook->querySubObject("Sheets"));
        int count = _work_sheets->property("Count").toInt();
        for(int i = 1;i <= count;++i)
        {
            std::shared_ptr<QAxObject> work_sheet(_workBook->querySubObject("Sheets(int)",i));
            QString tableName = work_sheet->property("Name").toString();
            auto contents = getExcelContents(work_sheet);
            print_message("Excel Table " + tableName + " load complete!");
            std::thread generateCSharpScriptThread{[&](){
                CSharpTool csharpTool(_scriptFilePath);
                csharpTool.generate_csharp_script(tableName,contents);
                print_message("C Sharp file generate complete!");
            }};
            SqliteTool sqliteTool(_databasePath);
            sqliteTool.generate_table(tableName,contents);
            generateCSharpScriptThread.join();
        }
        _workBook->dynamicCall("Close(Boolean)",false);
    }
    catch(std::exception e)
    {
        print_message(e.what());
    }
}

std::vector<std::vector<QString>> ExcelTool::getExcelContents(std::shared_ptr<QAxObject> work_sheet)
{
    std::shared_ptr<QAxObject> used_range(work_sheet->querySubObject("UsedRange"));
    std::shared_ptr<QAxObject> rows(used_range->querySubObject("Rows"));
    std::shared_ptr<QAxObject> columns(used_range->querySubObject("Columns"));
    int row_start = used_range->property("Row").toInt();
    int column_start = used_range->property("Column").toInt();
    int row_count = rows->property("Count").toInt();
    int column_count = columns->property("Count").toInt();
    std::vector<std::vector<QString>> contents;
    std::vector<QString> properties = getPropertyies(work_sheet,row_start,column_start,column_count);
    contents.push_back(properties);
    for(int i = row_start + 1;i <= row_count;++i)
    {
        std::vector<QString> values;
        int column = column_start;
        for(int j = 0;j < properties.size();++j)
        {
            std::shared_ptr<QAxObject> cell(work_sheet->querySubObject("Cells(int,int)",i,column++));
            QVariant cell_value = cell->property("Value");
            values.push_back(cell_value.toString());
        }
        contents.push_back(values);
    }
    return contents;
}

std::vector<QString> ExcelTool::getPropertyies(std::shared_ptr<QAxObject> work_sheet, int row_start, int column_start, int column_count)
{
    std::vector<QString> properties;
    for(int i = column_start;i <= column_count;++i)
    {
        std::shared_ptr<QAxObject> cell(work_sheet->querySubObject("Cells(int,int)",row_start,i));
        QVariant cell_value = cell->property("Value");
        if(!cell_value.toString().isEmpty())
        {
            properties.push_back(cell_value.toString());
        }
    }
    return properties;
}

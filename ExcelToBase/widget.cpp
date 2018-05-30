#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QFile>
#include <thread>
#include <QDebug>
#include <QProcess>
#include <QDir>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    timer(this)
{
    ui->setupUi(this);
    _defaultExcelPath = "./";
    _databasePath = "./";
    _databasePath = "./";
    _process = new QProcess(this);
    void (QProcess::*f)(int) = &QProcess::finished;
    connect(_process,f,this,&Widget::onProcessFinshed);
    timer.start(1);
    connect(&timer,&QTimer::timeout,this,&Widget::read_process_output);
}

Widget::~Widget()
{
    delete ui;
    _process->close();
    delete _process;
    timer.stop();
}

void Widget::on_generateBtn_clicked()
{
    if(!verify_input())
    {
        return;
    }
    QString cmdLine = QString("%1 %2 %3 %4")
            .arg(R"(D:\qt\ConvertTool\release\ConvertTool.exe)")
            .arg(_defaultExcelPath)
            .arg(_databasePath)
            .arg(_scriptPath);
    if(_process->state() != QProcess::ProcessState::Running)
    {
        _process->start(cmdLine);
        ui->generateBtn->setEnabled(false);
        show_message("Start Process...");
    }
}

void Widget::show_message(QString msg)
{
    auto widget = ui->infoListWidget;
    widget->addItem(msg);
    widget->setCurrentRow(widget->count() - 1);
}

void Widget::onProcessFinshed()
{
    ui->generateBtn->setEnabled(true);
}

void Widget::read_process_output()
{
    while(_process->canReadLine())
    {
        show_message(_process->readLine());
    }
}

void Widget::on_chooseExcelFileBtn_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,"Open File",_defaultExcelPath,"*.xlsx");
    if(!path.isEmpty())
    {
        ui->excelFilenameLineEdit->setText(path);
        _defaultExcelPath = path;
    }
}

void Widget::on_chooseDatabaseFileBtn_clicked()
{
    QString path = QFileDialog::getSaveFileName(this,"Save DataBase",_databasePath,"*ntf");
    if(!path.isEmpty())
    {
        ui->databaseFileLineEdit->setText(path);
        _databasePath = path;
    }
}

void Widget::on_chooseScriptPathBtn_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,"Save Script Dir",_scriptPath);
    if(!path.isEmpty())
    {
        ui->scriptPathLineEdit->setText(path);
        _scriptPath = path;
    }
}

bool Widget::verify_input()
{
    if(ui->excelFilenameLineEdit->text().isEmpty())
    {
        _defaultExcelPath = ui->excelFilenameLineEdit->text();
        show_message("Chose Excel File");
        return false;
    }
    if(!QFile(ui->excelFilenameLineEdit->text()).exists())
    {
        show_message("Excel File no exists");
        return false;
    }
    else
    {
        _defaultExcelPath = ui->excelFilenameLineEdit->text();
    }
    if(ui->databaseFileLineEdit->text().isEmpty())
    {
        show_message("Chose Database Save Directory");
        return false;
    }
    if(ui->scriptPathLineEdit->text().isEmpty())
    {
        show_message("Chose Script Save Directory");
        return false;
    }
    else
    {
        QDir dir(ui->scriptPathLineEdit->text());
        if(!dir.exists())
        {
            if(QMessageBox::question(this,"Directory not exists","\"" + ui->scriptPathLineEdit->text() + "\" " + "is not exists,create it?") == QMessageBox::Yes)
            {
                if(!dir.mkdir(ui->scriptPathLineEdit->text()))
                {
                    show_message("Can't Create \"" + ui->scriptPathLineEdit->text() + "\"");
                    return false;
                }
                else
                {
                    _scriptPath = dir.path();
                    return true;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return true;
        }
    }
}

void Widget::on_clearBtn_clicked()
{
    ui->infoListWidget->clear();
}

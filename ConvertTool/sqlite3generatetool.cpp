#include "sqlite3generatetool.h"
#include "ui_sqlite3generatetool.h"
#include "sqliteprocessresult.h"
#include "exceltool.h"
#include <QString>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QFile>
#include <QStandardItem>
#include <QDir>
#include <thread>
#include <QThread>
#include <QProgressDialog>
#include "workthread.h"
#include <QMessageBox>

Sqlite3GenerateTool::Sqlite3GenerateTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sqlite3GenerateTool)
{
    ui->setupUi(this);
    connect(ui->excelPathLineEdit,&QLineEdit::textChanged,this,&Sqlite3GenerateTool::on_pathLineEdit_textChanged);
    connect(ui->outputPathLineEdit,&QLineEdit::textChanged,this,&Sqlite3GenerateTool::on_pathLineEdit_textChanged);
    ui->startBtn->setEnabled(false);
    ui->horizontalLayout_4->setStretch(3,1);
    QStandardItemModel* model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels(QStringList() << "TableName" << "Error Count");
    ui->treeView->setModel(model);
    if(_config.loadConfig())
    {
        ui->excelPathLineEdit->setText(_config.getExcelPath());
        ui->outputPathLineEdit->setText(_config.getOutputPath());
    }
    _thread = new QThread(this);
}

Sqlite3GenerateTool::~Sqlite3GenerateTool()
{
    delete ui;
}

void Sqlite3GenerateTool::on_ExcelPathBtn_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,"Open Excel File",ui->excelPathLineEdit->text(),"*.xlsb;*.xls;*.xlsx");
    ui->excelPathLineEdit->setText(path);
}

void Sqlite3GenerateTool::on_outputPathSelectBtn_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,"Save Path");
    ui->outputPathLineEdit->setText(path);
}

void Sqlite3GenerateTool::on_pathLineEdit_textChanged(QString text)
{
    Q_UNUSED(text);
    bool enable = false;
    if(QFileInfo(QFile(ui->excelPathLineEdit->text())).absoluteDir().exists() && QDir(ui->outputPathLineEdit->text()).exists())
    {
        enable = true;
    }
    ui->startBtn->setEnabled(enable);
}

void Sqlite3GenerateTool::on_startBtn_clicked()
{
    _config.setExcelPath(ui->excelPathLineEdit->text());
    _config.setOutputPath(ui->outputPathLineEdit->text());
    _config.saveConfig();
    ui->startBtn->setEnabled(false);
    ui->excelPathLineEdit->setEnabled(false);
    ui->ExcelPathBtn->setEnabled(false);
    ui->outputPathLineEdit->setEnabled(false);
    ui->outputPathSelectBtn->setEnabled(false);
    WorkThread* worker = new WorkThread(ui->excelPathLineEdit->text(),ui->outputPathLineEdit->text() + "/city.ntf",ui->outputPathLineEdit->text());
    worker->moveToThread(_thread);
    connect(worker,&WorkThread::finshed,this,&Sqlite3GenerateTool::onFinshed);
    connect(worker,&WorkThread::start,this,&Sqlite3GenerateTool::onStartProcess);
    connect(worker,&WorkThread::update,this,&Sqlite3GenerateTool::onUpdateProgress);
    connect(worker,&WorkThread::error,this,&Sqlite3GenerateTool::onError);
    connect(_thread,&QThread::started,worker,&WorkThread::run);
    _thread->start();
}

void Sqlite3GenerateTool::onFinshed(QList<SqliteProcessResult> results)
{
    _results = results;
    static_cast<QStandardItemModel*>(ui->treeView->model())->clear();
    static_cast<QStandardItemModel*>(ui->treeView->model())->setHorizontalHeaderLabels(QStringList() << "TableName" << "Error Count");
    for(int i = 0;i < _results.size();++i)
    {
        QStandardItem* item0 = new QStandardItem(_results[i].getTableName());
        QStandardItem* item1 = new QStandardItem(QString::number(_results[i].getOrderErrorTotal()));
        for(int j = 0;j < _results[i].getOrderErrorTotal();++j)
        {
            item0->appendRow(new QStandardItem(_results[i].GetErrorMessage(j).second));
        }
        static_cast<QStandardItemModel*>(ui->treeView->model())->appendRow(QList<QStandardItem*>{item0,item1});
    }
    if(_progress)
    {
        _progress->hide();
    }
    int total = 0;
    int error = 0;
    for(SqliteProcessResult r : _results)
    {
        total += r.getOrderTotal();
        error += r.getOrderErrorTotal();
    }
    QString resultInfo = QString("Total: %1\t Error: %2").arg(total).arg(error);
    ui->resultLabel->setText(resultInfo);
    ui->startBtn->setEnabled(true);
    ui->excelPathLineEdit->setEnabled(true);
    ui->ExcelPathBtn->setEnabled(true);
    ui->outputPathLineEdit->setEnabled(true);
    ui->outputPathSelectBtn->setEnabled(true);
}

void Sqlite3GenerateTool::on_treeView_clicked(const QModelIndex &index)
{
    ui->orderLineEdit->clear();
    ui->errorLineEdit->clear();
    if(index.parent().row() == -1)
    {
        return;
    }
    if(index.column() == -1)
    {
        return;
    }
    SqliteProcessResult result = _results[index.parent().row()];
    if(result.getOrderErrorTotal() == 0)
    {
        return;
    }
    ui->orderLineEdit->setText(result.GetErrorMessage(index.row()).first);
    ui->errorLineEdit->setText(result.GetErrorMessage(index.row()).second);
}

void Sqlite3GenerateTool::onStartProcess(int total)
{
    if(!_progress)
    {
        _progress = new QProgressDialog("Processing...","Cancel",0,total,this);
        connect(_progress,&QProgressDialog::canceled,_thread,&QThread::terminate);
    }
    else
    {
        _progress->setMaximum(total);
        _progress->setMinimum(0);
    }
    _progress->setWindowTitle("Progress");
    _progress->setValue(0);
}

void Sqlite3GenerateTool::onUpdateProgress(int value)
{
    if(_progress)
    {
        _progress->setValue(value);
    }
}

void Sqlite3GenerateTool::onError(QString msg)
{
    QMessageBox::critical(this,"Error",msg + "\napplication will Terminate");
}

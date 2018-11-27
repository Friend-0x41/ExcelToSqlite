#ifndef SQLITE3GENERATETOOL_H
#define SQLITE3GENERATETOOL_H

#include <QWidget>
#include <QList>
#include <sqliteprocessresult.h>
#include <config.h>

class QThread;
class QProgressDialog;
class QListWidgetItem;

namespace Ui {
class Sqlite3GenerateTool;
}

class Sqlite3GenerateTool : public QWidget
{
    Q_OBJECT

public:
    explicit Sqlite3GenerateTool(QWidget *parent = 0);
    ~Sqlite3GenerateTool();

public slots:
    void onFinshed(QList<SqliteProcessResult> results);
    void onStartProcess(int total);
    void onUpdateProgress(int value);
    void onError(QString msg);

private slots:
    void on_ExcelPathBtn_clicked();

    void on_outputPathSelectBtn_clicked();

    void on_pathLineEdit_textChanged(QString text);

    void on_startBtn_clicked();

    void on_treeView_clicked(const QModelIndex &index);

private:
    Ui::Sqlite3GenerateTool *ui;
    QList<SqliteProcessResult> _results;
    Config _config;
    QProgressDialog* _progress = nullptr;
    QThread* _thread = nullptr;
};

#endif // SQLITE3GENERATETOOL_H

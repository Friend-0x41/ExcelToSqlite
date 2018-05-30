#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>

class ExcelTool;

class QProcess;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:

    void onProcessFinshed();

private slots:

    void on_generateBtn_clicked();

    void read_process_output();

    void on_chooseExcelFileBtn_clicked();

    void on_chooseDatabaseFileBtn_clicked();

    void on_chooseScriptPathBtn_clicked();

    void on_clearBtn_clicked();

private:

    void show_message(QString msg);

    bool verify_input();
private:
    Ui::Widget *ui;

    QString _defaultExcelPath;

    QString _databasePath;

    QString _scriptPath;

    QProcess* _process = nullptr;

    QTimer timer;
};

#endif // WIDGET_H

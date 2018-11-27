#ifndef LOG_H
#define LOG_H

#include <QString>
#include <cstdio>
#include <fstream>
#include <QObject>

class Log : public QObject
{
    Q_OBJECT
public:
    Log(QObject* parent = nullptr);
    ~Log();
    int get_log_count() const;
protected:
    void print_message(QString msg);
private:
    static int _count;
    std::ofstream out;
};

#endif // LOG_H

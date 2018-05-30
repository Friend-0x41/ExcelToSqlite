#ifndef LOG_H
#define LOG_H

#include <QString>
#include <cstdio>

class Log
{
public:
    Log();
protected:
    void print_message(QString msg);
private:
    static int _count;
};

#endif // LOG_H

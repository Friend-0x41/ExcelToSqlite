#include "log.h"
#include <fstream>
#include <sstream>

int Log::_count = 0;

Log::Log(QObject* parent)
    :QObject(parent)
{
    out.open("log.txt",std::ios_base::app);
    Q_ASSERT(out.is_open());
}

Log::~Log()
{
    if(out.is_open())
    {
        out.close();
    }
}

void Log::print_message(QString msg)
{
    std::string s = msg.toStdString();
    out.write(s.c_str(),s.length());
    out.flush();
}

int Log::get_log_count() const
{
    return _count;
}

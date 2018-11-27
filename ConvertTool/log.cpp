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
    char str[2048];
    memset(str,2048,0);
    sprintf(str,"%d\t%s\n",_count++,msg.toStdString().c_str());
    out.write(str,strlen(str));
    out.flush();
}

int Log::get_log_count() const
{
    return _count;
}

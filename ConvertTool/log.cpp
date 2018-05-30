#include "log.h"

int Log::_count = 0;

Log::Log()
{

}

void Log::print_message(QString msg)
{
    printf("%d\t%s",_count++,msg.toStdString().c_str());
    printf("\n");
}

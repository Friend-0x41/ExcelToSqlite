#include "sqliteprocessresult.h"

SqliteProcessResult::SqliteProcessResult(const QString& tableName)
    :_tableName(tableName)
{

}

void SqliteProcessResult::AddResult(bool success,QString errorMessage,QString order)
{
    ++_orderTotalCount;
    if(!success)
    {
        ++_orderErrorCount;
        _errorMessages.push_back(std::make_pair(order,errorMessage));
    }
}

std::pair<QString,QString> SqliteProcessResult::GetErrorMessage(int index)
{
    Q_ASSERT(index < _orderErrorCount);
    return _errorMessages[index];
}

int SqliteProcessResult::getOrderTotal() const
{
    return _orderTotalCount;
}

int SqliteProcessResult::getOrderErrorTotal() const
{
    return _orderErrorCount;
}

QString SqliteProcessResult::getTableName() const
{
    return _tableName;
}

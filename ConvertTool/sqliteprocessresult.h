#ifndef SQLITEPROCESSRESULT_H
#define SQLITEPROCESSRESULT_H

#include "log.h"
#include <QString>
#include <vector>

class SqliteProcessResult
{
public:
    SqliteProcessResult() = default;
    SqliteProcessResult(const QString& tableName);
    void LogResults() const;
    void AddResult(bool success = true,QString errorMessage = "",QString order = "");
    std::pair<QString,QString> GetErrorMessage(int index);
    int getOrderTotal() const;
    int getOrderErrorTotal() const;
    QString getTableName() const;

private:
    QString _tableName;
    int _orderTotalCount = 0;
    int _orderErrorCount = 0;
    std::vector<std::pair<QString,QString>> _errorMessages;
};
#endif // SQLITEPROCESSRESULT_H

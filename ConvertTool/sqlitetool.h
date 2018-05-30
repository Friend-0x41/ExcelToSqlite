#ifndef SQLITETOOL_H
#define SQLITETOOL_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <vector>
#include "log.h"
#include <QString>

class QAxObject;

class SqliteTool : public Log
{
public:
    SqliteTool(const QString& databasePath);
    ~SqliteTool();
public:
    void generate_table(const QString& tableName,const std::vector<std::vector<QString>>& contents);
private:
    void createTable(const QString& table,const std::vector<std::pair<QString,QString>>& properties);
    void insert(const QString& table,const std::vector<std::pair<QString,QString>>& properties,const std::vector<QString>& values);
    inline  std::vector<std::pair<QString,QString>> get_property_nameAndType(const std::vector<QString> properties);
private:
    QSqlDatabase database;
    QString _databasePath;
};

#endif // SQLITETOOL_H

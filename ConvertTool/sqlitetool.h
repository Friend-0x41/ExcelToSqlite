#ifndef SQLITETOOL_H
#define SQLITETOOL_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <vector>
#include "log.h"
#include <QString>
#include "sqliteprocessresult.h"
#include <QObject>

class QAxObject;

class SqliteTool : public Log
{
    Q_OBJECT
public:
    SqliteTool(const QString& databasePath,QObject* parent = nullptr);
    ~SqliteTool();
public:
    SqliteProcessResult generate_table(const QString& tableName,const std::vector<std::vector<QString>>& contents);
private:
    void createTable(const QString& table,const std::vector<std::pair<QString,QString>>& properties);
    void insert(const QString& table,const std::vector<std::pair<QString,QString>>& properties,const std::vector<QString>& values,SqliteProcessResult& result);
    inline  std::vector<std::pair<QString,QString>> get_property_nameAndType(const std::vector<QString> properties);
private:
    QSqlDatabase database;
    QString _databasePath;
};

#endif // SQLITETOOL_H

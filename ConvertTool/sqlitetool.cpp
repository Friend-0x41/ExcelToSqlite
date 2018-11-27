#include "sqlitetool.h"
#include <QDir>
#include <QAxObject>
#include "exceltool.h"

SqliteTool::SqliteTool(const QString& databasePath,QObject* parent)
    :_databasePath(databasePath),Log(parent)
{
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(databasePath);
    }

    if(database.open())
    {
        print_message("Database Open");
    }
}

SqliteTool::~SqliteTool()
{
    if(database.open())
    {
        database.close();
        print_message("Datebase Close");
    }
}

SqliteProcessResult SqliteTool::generate_table(const QString& tableName,const std::vector<std::vector<QString>>& contents)
{
    auto properties = get_property_nameAndType(contents[0]);
    createTable(tableName,properties);
    SqliteProcessResult result(tableName);
    std::for_each(contents.cbegin() + 1,contents.cend(),[&](const std::vector<QString>& values){
        insert(tableName,properties,values,result);
    });
    print_message("Table " + tableName + " generate complete!");
    return result;
}

void SqliteTool::insert(const QString& tableName,const std::vector<std::pair<QString,QString>>& properties,const std::vector<QString>& values,SqliteProcessResult& result)
{
    QString type;
    QString insert_properties_statement = "INSERT INTO " + tableName + "(";
    QString insert_value_statement = "VALUES(";
    for(int i = 0;i < properties.size();++i)
    {
        type = properties[i].second;
        if(!values[i].isEmpty())
        {
            if(type == "int")
            {
                insert_value_statement += QString::number(values[i].toInt()) + ",";
            }
            else if(type == "string")
            {
                QString s = values[i];
                s.replace('\'',"''");
                insert_value_statement += "'" +  s + "'" + ",";
            }
            else if(type == "float")
            {
                insert_value_statement += QString::number(values[i].toFloat()) + ",";
            }
            insert_properties_statement += properties[i].first + ",";
        }
    }
    insert_properties_statement[insert_properties_statement.length() - 1] = ' ';
    insert_properties_statement += ") ";
    insert_value_statement[insert_value_statement.length() - 1] = ' ';
    insert_value_statement += ")";
    QString insert_statement = insert_properties_statement + insert_value_statement;
    print_message(insert_statement);
    QSqlQuery sqlQuery;
    sqlQuery.prepare((insert_statement));
    if(!sqlQuery.exec())
    {
        print_message(sqlQuery.lastError().text());
        result.AddResult(false,sqlQuery.lastError().text(),insert_statement);
    }
    else
    {
        print_message("Insert Success!");
        result.AddResult();
    }
}

void SqliteTool::createTable(const QString& tableName,const std::vector<std::pair<QString,QString>>& properties)
{
    QString create_sql = "CREATE TABLE " + tableName + "(";

    create_sql += properties[0].first + " " + "INT " + "PRIMARY KEY";

    QString type;

    for(int i = 1;i < properties.size();++i)
    {
        if(properties[i].second == "string")
        {
            type = " TEXT";
        }
        else if(properties[i].second == "int")
        {
            type = " INT";
        }
        else if(properties[i].second == "float")
        {
            type = " FLOAT";
        }
        create_sql += "," + properties[i].first + type;
    }
    create_sql += ")";
    print_message(create_sql);

    QSqlQuery sql_query ;
    sql_query.prepare(create_sql);
    if(!sql_query.exec())
    {
         print_message(sql_query.lastError().text());
    }
    else
    {
        print_message("Table " + tableName + " create success");
    }
}

 std::vector<std::pair<QString,QString>> SqliteTool::get_property_nameAndType(const std::vector<QString> properties)
{
    std::vector<std::pair<QString,QString>> names;
    for(auto p : properties)
    {
        names.push_back(std::make_pair(p.split('-')[0],p.split('-')[1]));
    }
    return names;
}

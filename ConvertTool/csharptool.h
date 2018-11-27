#ifndef CSHARPTOOL_H
#define CSHARPTOOL_H

#include <vector>
#include <QString>
#include "log.h"

class CSharpTool : public Log
{
public:
    CSharpTool(const QString& scriptPath);
public:
    void generate_csharp_script(const QString& tableName,const std::vector<std::vector<QString>>& contents);
private:
    void generate_file(const QString& file_name,const QString& code);
    void generate_structData(const QString& className,const std::vector<std::pair<QString,QString>>& properties);
    void generate_enum(const QString& tableName,const QString& className,const std::vector<QString>& names);
    void generate_load_script(const QString& className,const std::vector<std::pair<QString,QString>>& properties,QString returnStruct);
    std::vector<std::pair<QString,QString>> get_properties(const std::vector<QString>& properties);
    std::vector<QString> get_names(const std::vector<std::vector<QString>>& contents);
private:
    QString _scriptPath;
};

#endif // CSHARPTOOL_H

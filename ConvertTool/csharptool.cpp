#include "csharptool.h"
#include <QFile>
#include <algorithm>
#include <QIODevice>

CSharpTool::CSharpTool(const QString& scriptPath)
    :_scriptPath(scriptPath)
{

}

void CSharpTool::generate_csharp_script(const QString& tableName,const std::vector<std::vector<QString>>& contents)
{
    auto properties = get_properties(contents[0]);
    generate_structData(tableName + "Info",properties);
    auto names = get_names(contents);
    generate_enum(tableName+"Type",names);
}

void CSharpTool::generate_file(const QString& file_name, const QString &code)
{
    QFile file(_scriptPath + "\\" + file_name);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(code.toStdString().c_str(),code.length());
    }
    print_message("Generate file " + file_name + " completed!");
}

void CSharpTool::generate_structData(const QString &className, const std::vector<std::pair<QString,QString>> &properties)
{
    QString code;
    code += "public class " + className + "\n{\n";
    for(auto p :properties)
    {
        code += "\tpublic " + p.first + " " + p.second + ";\n";
    }
    code += "}";
    generate_file(className+".cs",code);
}

void CSharpTool::generate_enum(const QString &className, const std::vector<QString> &names)
{
    QString code;
    code += "public enum " + className + "\n{\n";
    for(auto n : names)
    {
        code += "\t" + n + ",\n";
    }
    code += "}";
    generate_file(className + ".cs",code);
}

std::vector<std::pair<QString,QString>> CSharpTool::get_properties(const std::vector<QString>& properties)
{
    std::vector<std::pair<QString,QString>> result;
    for(auto p : properties)
    {
        if(p.isEmpty())
        {
            break;
        }
        result.push_back(std::make_pair(p.split('-')[1],p.split('-')[0]));
    }
    return result;
}

std::vector<QString> CSharpTool::get_names(const std::vector<std::vector<QString> > &contents)
{
    std::vector<QString> result;
    std::for_each(contents.cbegin() + 1,contents.cend(),[&result](const std::vector<QString>& row)
    {
        result.push_back(row[1] + row[2]);
    });
    return result;
}

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
    generate_enum(tableName,tableName+"Type",names);
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
    code += "public struct " + className + "\n{\n";
    for(auto p :properties)
    {
        code += "\tpublic " + p.first + " " + p.second + ";\n";
    }
    code += "}";
    generate_file(className+".cs",code);
    generate_load_script(className + "Loader",properties,className);
}

void CSharpTool::generate_load_script(const QString &className, const std::vector<std::pair<QString,QString>> &properties,QString returnStruct)
{
    QString code;
    code += "using System.Data;\n\n";
    code += "public class " + className + "\n{\n";
    QString reader = "reader";
    QString result = "result";
    code += "\tpublic static " + returnStruct + " Load(IDataReader " + reader + ")\n{\n";
    code += "\t\t" + returnStruct + " " + result + " = new " + returnStruct +"();\n";
    QString type;
    for(int i = 0;i < properties.size();++i)
    {
        /*if(properties[i].first == "string")
        {
            type = "(string)";
        }
        else if(properties[i].first == "int")
        {
            type = "(int)";
        }
        else if(properties[i].first == "float")
        {
            type = "(float)";
        }
        code += QString("%1%2%3%4%5%6%7%8%9%10%11%12%13%14%15")
                .arg("\t\tif(")
                .arg(reader)
                .arg(".GetValue(")
                .arg(i)
                .arg(").ToString() != \"\")\n")
                .arg("\t\t\t")
                .arg(result)
                .arg(".")
                .arg(properties[i].second)
                .arg(" = ")
                .arg(type)
                .arg(reader)
                .arg(".GetValue(")
                .arg(i)
                .arg(");\n");
    }*/
        if(properties[i].first == "string")
        {
            type = "GetString";
        }
        else if(properties[i].first == "int")
        {
            type = "GetInt32";
        }
        else if(properties[i].first == "float")
        {
            type = "GetFloat";
        }
        code += QString("%1%2%3%4%5%6%7%8%9%10%11%12%13%14%15%16")
                .arg("\t\tif(")
                .arg(reader)
                .arg(".GetValue(")
                .arg(i)
                .arg(").ToString() != \"\")\n")
                .arg("\t\t\t")
                .arg(result)
                .arg(".")
                .arg(properties[i].second)
                .arg(" = ")
                .arg("reader")
                .arg(".")
                .arg(type)
                .arg("(")
                .arg(i)
                .arg(");\n");
    }
    code += "\t\treturn " + result + ";\n";
    code += "\t}\n";
    code += "}";
    generate_file(className + ".cs",code);
}

void CSharpTool::generate_enum(const QString& tableName,const QString &className, const std::vector<QString> &names)
{
    QString code;
    code += "public enum " + className + "\n{\n";
    for(auto n : names)
    {
        code += "\t" + n.replace(" ","") + ",\n";
    }
    code += "\t" + tableName + ",\n";
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
        QString s = row[1];
        if(!row[2].isEmpty())
        {
            s += "_" + row[2];
        }
        result.push_back(s);
    });
    return result;
}

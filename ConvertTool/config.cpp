#include "config.h"
#include <fstream>
#include <sstream>

const QString Config::_filePath = "Config.ini";

Config::Config()
{

}

QString Config::getExcelPath() const
{
    return _excelPath;
}
QString Config::getOutputPath() const
{
    return _outputPath;
}
void Config::setExcelPath(QString path)
{
    _excelPath = path;
}
void Config::setOutputPath(QString path)
{
    _outputPath = path;
}

bool Config::loadConfig()
{
    std::ifstream in;
    in.open(_filePath.toStdString());
    if(!in.is_open())
    {
        return false;
    }
    std::stringstream ss;
    try
    {
        std::string s;
        ss << in.rdbuf();
        ss >> s;
        _excelPath = s.c_str();
        ss >> s;
        _outputPath = s.c_str();
    }
    catch(std::exception e)
    {
        Q_UNUSED(e);
        in.close();
        return false;
    }
    in.close();
    return true;
}
bool Config::saveConfig()
{
    std::ofstream out;
    out.open(_filePath.toStdString());
    if(!out.is_open())
    {
        return false;
    }
    out << _excelPath.toStdString() << "\n";
    out << _outputPath.toStdString();
    return true;
}

#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class Config
{
public:
    Config();
    QString getExcelPath() const;
    QString getOutputPath() const;
    void setExcelPath(QString path);
    void setOutputPath(QString path);
    bool loadConfig();
    bool saveConfig();
private:
    static const QString _filePath;
    QString _excelPath;
    QString _outputPath;
};

#endif // CONFIG_H

#include <QCoreApplication>
#include <exceltool.h>
int main(int argc, char *argv[])
{
    QCoreApplication app(argc,argv);
    ExcelTool excelTool(argv[1],argv[2],argv[3]);
    excelTool.Process();
    printf("Process Complete!\n");
    app.exit();
}

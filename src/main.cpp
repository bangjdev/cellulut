#include <QApplication>
#include <QString>
#include "components/main_window.h"
#include "utils/database.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    DBManager* db = new DBManager(QString::fromStdString("./DBB.db"));

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

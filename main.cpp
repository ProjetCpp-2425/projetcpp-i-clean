#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Connection c;

    bool test = c.createconnect();


    if (test) {

        QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");

        if (db.isOpen()) {

            w.show();
            QMessageBox::information(nullptr, QObject::tr("Database is Open"),
                                     QObject::tr("Connection successful.\nClick Cancel to exit."), QMessageBox::Cancel);
        } else {
            // Database is not open even though the connection was successful
            QMessageBox::critical(nullptr, QObject::tr("Database is Not Open"),
                                  QObject::tr("Database connection was not established properly.\nClick Cancel to exit."), QMessageBox::Cancel);
        }
    } else {
        // Connection failed
        QMessageBox::critical(nullptr, QObject::tr("Database is Not Open"),
                              QObject::tr("Connection failed.\nClick Cancel to exit."), QMessageBox::Cancel);
    }

    return a.exec();
}

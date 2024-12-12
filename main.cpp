#include "mainwindow.h"
#include "dialog.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Connection c;

    // Show the dialog first
    Dialog dialog(&w);
    dialog.exec(); // Show dialog modally, blocking until closed

    // Attempt to establish database connection
    bool test = c.createconnect();

    if (test) {
        QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");

        if (db.isOpen()) {
            // Show main window if the database is open
            w.show();
            QMessageBox::information(nullptr, QObject::tr("Database is Open"),
                                     QObject::tr("Connection successful.\nClick Cancel to exit."), QMessageBox::Cancel);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Database is Not Open"),
                                  QObject::tr("Database connection was not established properly.\nClick Cancel to exit."), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Database is Not Open"),
                              QObject::tr("Connection failed.\nClick Cancel to exit."), QMessageBox::Cancel);
    }

    return a.exec();
}

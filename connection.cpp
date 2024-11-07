// connection.cpp
#include "connection.h"
#include <QDebug>
#include <QSqlError>

Connection::Connection() {}

bool Connection::createconnect()
{
    // Check if there's already an open connection to avoid duplicates
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
        if (db.isOpen()) {
            qDebug() << "Using existing database connection.";
            return true;
        }
    }

    // Add a new default connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("CPP_Project"); // Replace with your ODBC data source name
    db.setUserName("ala");
    db.setPassword("123456");

    // Open the database and check for success
    if (!db.open()) {
        qDebug() << "Database connection failed: " << db.lastError().text();
        return false;
    }

    qDebug() << "Database connected successfully.";
    return true;
}

#include "connection.h"


Connection::Connection(){}

bool Connection::createconnection(){
    db=QSqlDatabase::addDatabase("QODBC");
    bool test=false;
db.setDatabaseName("projetcpp2a34");//inserer le nom de la source de données
db.setUserName("system");//inserer nom de l'utilisateur
db.setPassword("1234");//inserer mot de passe de cet utilisateur

if (db.open()) test=true;
return  test;
}
void Connection::closeConnection()
{

    (db.close());

}

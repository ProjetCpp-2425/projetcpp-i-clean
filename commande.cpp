#include "commande.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QString>
#include <QSqlError>

commande::commande() {}

commande::commande(int id, QString nom, QString prenom, QString location, int tel, QString email, int nbor)
{
    this->id = id;
    this->nom = nom;
    this->prenom = prenom;
    this->location = location;
    this->tel = tel;
    this->email = email;
    this->nbor = nbor;
}


void commande::setnom(QString n) { this->nom = n; }
void commande::setprenom(QString n) { this->prenom = n; }
void commande::setlocation(QString n) { this->location = n; }
void commande::settel(int n) { this->tel = n; }
void commande::setemail(QString n) { this->email = n; }
void commande::setnbor(int n) { this->nbor = n; }


int commande::get_id() { return id; }
QString commande::get_nom() { return nom; }
QString commande::get_prenom() { return prenom; }
QString commande::get_location() { return location; }
int commande::get_tel() { return tel; }
QString commande::get_email() { return email; }
int commande::get_nbor() { return nbor; }

// Add a new record
bool commande::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO commande (id, nom, prenom, location, tel, email, nbor) "
                  "VALUES (:id, :nom, :prenom, :location, :tel, :email, :nbor)");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":location", location);
    query.bindValue(":tel", tel);
    query.bindValue(":email", email);
    query.bindValue(":nbor", nbor);

    return query.exec();
}

QSqlQueryModel* commande::afficher()
{
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");  // Use the default connection

    if (!db.isOpen()) {
        qDebug() << "Database is not open in afficher().";  // Error message for debugging
        return nullptr;  // Return nullptr if the connection is not open
    }

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query(db);  // Use the default connection

    query.prepare("SELECT * FROM commande");
    if (query.exec()) {
        model->setQuery(query);
        qDebug() << "Data loaded with" << model->rowCount() << "rows.";
    } else {
        qDebug() << "Query execution failed:" << query.lastError().text();  // Log SQL errors
    }

    return model;
}



bool commande::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM commande WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}



bool commande::modifier(int id)
{
    QSqlQuery query;
    query.prepare("UPDATE commande SET nom = :nom, prenom = :prenom, "
                  "location = :location, tel = :tel, email = :email, nbor = :nbor "
                  "WHERE id = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":location", location);
    query.bindValue(":tel", tel);
    query.bindValue(":email", email);
    query.bindValue(":nbor", nbor);

    return query.exec();
}

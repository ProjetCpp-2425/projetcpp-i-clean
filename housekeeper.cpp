#include "housekeeper.h"
#include "qsqlerror.h"
#include<QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>



Housekeeper::Housekeeper(QString nomf, QString prenomf, QString adressef, int idf, QString telf){ //, QString type, QString nb_taches, int salaire) {
    this->nomf=nomf;
    this->prenomf=prenomf;
    this->adressef=adressef;
    this->idf=idf;
    this->telf=telf;
   // this->type = type;
   // this->nb_taches = nb_taches;
   // this->salaire = salaire;
}
bool Housekeeper::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO HOUSEKEEPER(NOMF, PRENOMF, ADRESSEF, IDF,TELF)" //, TYPE, NB_TACHES, SALAIRE) "
                  "VALUES(:nomf, :prenomf, :adressef, :idf, :telf)"); //, :type, :nb_taches, :salaire)");

    query.bindValue(":nomf", nomf);
    query.bindValue(":prenomf", prenomf);
    query.bindValue(":adressef", adressef);
    query.bindValue(":idf", idf);
    query.bindValue(":telf", telf);
    //query.bindValue(":type", type);
   // query.bindValue(":nb_taches", nb_taches);
  //  query.bindValue(":salaire", salaire);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Erreur lors de l'ajout: " << query.lastError().text();
        return false;
    }
}



bool Housekeeper::supprimer(int idf) {
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM HOUSEKEEPER WHERE IDF = :idf");
    checkQuery.bindValue(":idf", idf);
    checkQuery.exec();

    if (checkQuery.next()) {
        int count = checkQuery.value(0).toInt();

        if (count == 0) {
            qDebug() << "Erreur : L'ID n'existe pas dans la base de données.";
            return false;
        }
    }
    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM HOUSEKEEPER WHERE IDF = :idf");
    deleteQuery.bindValue(":idf", idf);

    if (deleteQuery.exec()) {
        qDebug() << "Enregistrement supprimé avec succès.";
        return true;
    } else {
        qDebug() << "Erreur lors de la suppression de l'enregistrement.";
        return false;
    }
}
QSqlQueryModel* Housekeeper::afficher(QString adressef) {
    QSqlQueryModel * model = new QSqlQueryModel();
    QString queryStr = "SELECT * FROM HOUSEKEEPER";

    if (!adressef.isEmpty()) {
        queryStr += " WHERE ADRESSEF LIKE :adresse";
    }

    QSqlQuery query;
    query.prepare(queryStr);
    if (!adressef.isEmpty()) {
        query.bindValue(":adresse", "%" + adressef + "%");
    }

    query.exec();
    model->setQuery(query);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("SURNAME"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NAME"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ADRESS"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("TELF"));

    return model;
}
bool Housekeeper::modifier() {
    QSqlQuery query;
    query.prepare("UPDATE HOUSEKEEPER SET NOMF = :nomf, PRENOMF = :prenomf, ADRESSEF = :adressef, TELF = :telf WHERE IDF = :idf");

    query.bindValue(":nomf", nomf);
    query.bindValue(":prenomf", prenomf);
    query.bindValue(":adressef", adressef);
    query.bindValue(":idf", idf);
    query.bindValue(":telf", telf);
   // query.bindValue(":type", type);
    //query.bindValue(":nb_taches", nb_taches);
  //  query.bindValue(":salaire", salaire);

    if (query.exec()) {
        qDebug() << "Housekeeper updated successfully!";
        return true;
    } else {
        qDebug() << "Error updating housekeeper: " << query.lastError();
        return false;
    }
}
QSqlQueryModel* Housekeeper::rechercher(int idf) {
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    // Préparer la requête pour rechercher le housekeeper par ID
    query.prepare("SELECT * FROM HOUSEKEEPER WHERE IDF = :idf");
    query.bindValue(":idf", idf);
    query.exec();

    model->setQuery(query);

    // Définir les en-têtes des colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("TELF"));

    return model;
}
QSqlQueryModel* Housekeeper::trier() {
    QSqlQueryModel *model = new QSqlQueryModel();

    // Requête SQL pour trier les housekeepers par la date d'ajout (DATEF)
    QSqlQuery query;
    query.prepare("SELECT * FROM HOUSEKEEPER ORDER BY DATEF ASC"); // Tri croissant selon DATEF
    query.exec();

    model->setQuery(query);

    // Définir les en-têtes des colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date d'ajout"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("phone num"));


    return model;
}
QSqlQueryModel* Housekeeper::stat() {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare(R"(
        SELECT ADRESSEF, COUNT(*) * 100.0 / (SELECT COUNT(*) FROM HOUSEKEEPER) AS pourcentage
        FROM HOUSEKEEPER
        GROUP BY ADRESSEF
        ORDER BY pourcentage DESC
    )");

    if (query.exec()) {
        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Localisation"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Pourcentage (%)"));
    } else {
        qDebug() << "Erreur dans la requête stat:" << query.lastError().text();
    }

    return model;
}

int Housekeeper::calculerSalaire(QString& type, QString nb_taches) {
    int salaire = 0;

    if (type == "monthly") {
        salaire = 1000; // Salaire mensuel
    } else if (type == "weekly") {
        salaire = 250;  // Salaire hebdomadaire
    } else if (type == "daily") {
        if (nb_taches == "1->3 tasks") {
            salaire = 20 ; // 20 DT par tâche pour 1 à 3 tâches
        } else if (nb_taches == "4->6 tasks") {
            salaire = 35 ; // 35 DT par tâche pour 4 à 6 tâches
        } else {
            salaire = 50 ; // 50 DT par tâche pour plus de 6 tâches
        }
    }

    return salaire;
}

bool Housekeeper::envoyerSMS(const QString &telf, const QString &localisation, bool classic, bool smart, bool urgence) {
    // Validation du numéro de téléphone
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM HOUSEKEEPER WHERE TELF = :telf");
    query.bindValue(":telf", telf);
    query.exec();

    if (!query.next() || query.value(0).toInt() == 0) {
        qDebug() << "Numéro de téléphone invalide:" << telf;
        return false; // Indiquer que le numéro n'existe pas
    }

    // Construction du message
    QString message = "Localisation du client : " + localisation + "\nType de nettoyage : ";

    if (classic) { message += "Classic Clean "; }
    if (smart) { message += "Smart Clean "; }
    if (urgence) { message += "Urgence "; }

    // Code fictif d'envoi de SMS
    qDebug() << "Envoi de message à:" << telf << "Message:" << message;
    // Vous devez appeler votre API SMS ici pour l'envoi réel

    return true; // Retourne vrai pour que l'appelant sache que l'envoi a réussi
}

#ifndef HOUSEKEEPER_H
#define HOUSEKEEPER_H
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include <QTableView>
class Housekeeper
{
    QString nomf, prenomf,adressef , telf,  nb_taches, type;
    int idf, salaire;
    //QString telf;
public:
    Housekeeper(){}
    Housekeeper (QString, QString, QString, int, QString);
    QString getNomf(){return nomf;}
    QString getPrenomf(){return prenomf;}
    QString getAdressef(){return adressef;}
    QString getType() { return type; }
    QString getTelf(){return telf;}
    int getIdf(){return idf;}
    QString getNb_Taches() { return nb_taches; }
    int getSalaire() { return salaire; }
    void setNomf(QString n){nomf=n;}
    void setPrenomf(QString p){prenomf=p;}
    void setAdressef(QString adr){adressef=adr;}
    void setIdf(int idf){this->idf=idf;}
    void setTelf(QString tel){telf=tel;}
    void setType(QString t) { type = t; }
    void setNb_Taches(QString nb) { nb_taches = nb; }
    void setSalaire(int s) { salaire = s; }
    QSqlQueryModel * afficher(QString adressef);
    QSqlQueryModel * rechercher(int idf);
    QSqlQueryModel* trier();
    QSqlQueryModel * stat();

    bool afficher();
    bool supprimer(int idf);
    bool ajouter();
    bool modifier();
    bool rechercher();
    int calculerSalaire( QString& type, QString nb_taches);
    bool envoyerSMS(const QString &tel, const QString &localisation, bool classic, bool smart, bool urgence);

};
#endif // HOUSEKEEPER_H

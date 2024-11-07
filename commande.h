#ifndef COMMANDE_H
#define COMMANDE_H
#include <QSqlQueryModel>
#include <QSqlQuery>


class commande
{
public:
    commande();
    commande(int, QString, QString, QString, int, QString, int);



    void setnom(QString n);
    void setprenom(QString n);
    void setlocation(QString n);
    void settel(int tel);
    void setemail(QString n);
    void setnbor(int n);


    int get_id();
    QString get_nom();
    QString get_prenom();
    QString get_location();
    int get_tel();
    QString get_email();
    int get_nbor();



    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier(int);
private:
    QString  nom, prenom,location, email;
    int id ,tel,nbor;
};

#endif // COMMANDE_H

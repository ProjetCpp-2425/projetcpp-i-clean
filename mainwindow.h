#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "housekeeper.h"

#include <QMainWindow>
#include <QSqlQueryModel> // Assure-toi d'inclure QSqlQueryModel
#include "qnetworkreply.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_ajouter_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_afficher_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_rechercher_clicked();
    void on_pushButton_trier_clicked();
    void on_pushButton_pdf_clicked();
    void on_pushButton_pdf_2_clicked();
    void on_pushButton_stat_clicked();
    void on_pushButton_calculer_clicked();
    void on_pushButton_envoyer_clicked();
    void onSmsSent(QNetworkReply* reply);
    void sendSms();


private:
    Ui::MainWindow *ui;
    QSqlQueryModel *model;
    Housekeeper Htmp;
    Housekeeper H;

    int calculerSalaire(QString type, QString nb_taches);
};

#endif // MAINWINDOW_H

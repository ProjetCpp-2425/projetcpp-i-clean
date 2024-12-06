#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "housekeeper.h"

#include <QMainWindow>
#include <QSqlQueryModel> // Assure-toi d'inclure QSqlQueryModel
#include "qnetworkreply.h"
#include "qserialport.h"
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
    void keyPressEvent(QKeyEvent *event);
    void on_addButton_clicked();

    void on_settingsButton_clicked();

    void on_notifButton_clicked() ;

    void on_salaireButton_clicked() ;
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
    void on_startMonitoringButton_clicked();
    void sendSms();
    void setupSerialPort();
    void startMonitoring();
    void readSerialData();
    void onSerialDataReceived();
    void processMessage(const QString &message);



private:
    Ui::MainWindow *ui;
    QSqlQueryModel *model;
    Housekeeper Htmp;
    Housekeeper H;
    QSerialPort *serialPort;

    int calculerSalaire(QString type, QString nb_taches);
};

#endif // MAINWINDOW_H

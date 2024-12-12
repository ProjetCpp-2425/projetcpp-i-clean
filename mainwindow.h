#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connection.h"
#include "commande.h"
#include "arduino.h"





namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Declare envoyerEmail and saveQRCodeToFile functions
    void envoyerEmail(const QString &destinataire, const QString &sujet, const QString &message, const QString &attachmentPath = "");
    QString saveQRCodeToFile(const QString &data, const QString &filePath);
    void setStackedWidgetPage(int index); // Method to change stacked widget page

private slots:
    QString controlSaisie();
    void loadcommandeData();
    void onRowSelected(const QModelIndex &index);
    void on_ajouterCl_clicked();
    void on_modifierCl_clicked();
    void on_supprimerCl_clicked();
    void on_pdfCl_clicked();
    void on_email_3_clicked();
    void on_chercher_idCl_cursorPositionChanged(int arg1, int arg2);
    void on_pushButton_clicked();
    void on_tricomboBox_activated(int index);
    void on_statistiqueCl_clicked();
    void on_stat_anchorClicked(const QUrl &arg1);
    void on_tableView_clicked(const QModelIndex &index);

      void handleArduinoInput(const QByteArray &input);
       void on_chercher_idCl_textChanged(const QString &text);

    void on_chercher_location_cursorPositionChanged(int arg1, int arg2);

    void on_Back_clicked();



       void on_chercher_idCl_cursorPositionChanged(int arg1, int arg2);

       void on_pushButton_clicked();



       void on_tricomboBox_activated(int index);






       void on_pushButton_2_clicked();

       void on_QR_code_clicked();

       void on_statistiqueCl_clicked();

private:
    Ui::MainWindow *ui;
    commande C;
<<<<<<< HEAD

};



#endif
=======
    Arduino myArduino;

    void setupArduinoConnection();
        void performSearch(const QString &searchTerm);
};

#endif // MAINWINDOW_H
>>>>>>> f86bd98 (gestion commande)

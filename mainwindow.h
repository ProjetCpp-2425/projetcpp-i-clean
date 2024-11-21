#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "connection.h"
#include "commande.h"



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
    QString controlSaisie();
    void loadcommandeData();
       void onRowSelected(const QModelIndex &index);
       void on_ajouterCl_clicked();
       void on_modifierCl_clicked();

       void on_supprimerCl_clicked();

       void on_pdfCl_clicked();



       void on_chercher_idCl_cursorPositionChanged(int arg1, int arg2);

       void on_pushButton_clicked();



       void on_tricomboBox_activated(int index);






       void on_pushButton_2_clicked();

       void on_QR_code_clicked();

       void on_statistiqueCl_clicked();

private:
    Ui::MainWindow *ui;
    commande C;

};



#endif

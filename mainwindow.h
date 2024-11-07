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

private:
    Ui::MainWindow *ui;
    commande C;
};

#endif

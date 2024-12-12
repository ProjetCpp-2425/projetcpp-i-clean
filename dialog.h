#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "mainwindow.h" // Forward declaration of MainWindow

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(MainWindow *mainWindow, QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::Dialog *ui;
    MainWindow *mainWindow; // Pointer to MainWindow
};

#endif // DIALOG_H

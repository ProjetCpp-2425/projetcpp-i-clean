#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"

Dialog::Dialog(MainWindow *mainWindow, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    mainWindow(mainWindow) // Initialize the pointer
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    // Navigate to the first page of the stacked widget
    if (mainWindow) {
        mainWindow->setStackedWidgetPage(0);
        this->close(); // Close the dialog window
    }
}

void Dialog::on_pushButton_2_clicked()
{
    // Navigate to the second page of the stacked widget
    if (mainWindow) {
        mainWindow->setStackedWidgetPage(1);
        this->close(); // Close the dialog window
    }
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->locationComboBox->addItems({
        "Tunis", "Sfax", "Sousse", "Gabès", "Bizerte", "Ariana", "Kairouan",
        "Gafsa", "La Marsa", "Monastir", "Nabeul", "Ben Arous", "Hammamet",
        "Mahdia", "Zarzis", "Douz", "Tozeur", "Kebili", "Beja", "Jendouba",
        "Siliana", "Kasserine", "Le Kef", "Medenine", "Tataouine"
    });

    // Connect row selection to populate fields
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &MainWindow::onRowSelected);

    // Connect double-click on a row to populate the fields
    connect(ui->tableView, &QTableView::doubleClicked,
            this, &MainWindow::onRowSelected);

    loadcommandeData();
}

void MainWindow::loadcommandeData()
{
    commande cmd;
    QSqlQueryModel *model = cmd.afficher();  // Load data using afficher from commande class

    ui->tableView->setModel(model);  // Set the model for the table view
    ui->tableView->resizeColumnsToContents();
}

MainWindow::~MainWindow()
{
    delete ui;
}


// Slot to handle row selection and populate the QLineEdits with data
void MainWindow::onRowSelected(const QModelIndex &index)
{
    if (index.isValid()) {
        int row = index.row();
        ui->id_2->setText(ui->tableView->model()->index(row, 0).data().toString());
        ui->nom_2->setText(ui->tableView->model()->index(row, 1).data().toString());
        ui->prenom_2->setText(ui->tableView->model()->index(row, 2).data().toString());
        ui->locationComboBox->setCurrentText(ui->tableView->model()->index(row, 3).data().toString());
        ui->tel_2->setText(ui->tableView->model()->index(row, 4).data().toString());
        ui->email_2->setText(ui->tableView->model()->index(row, 5).data().toString());
        ui->nborSpinBox->setValue(ui->tableView->model()->index(row, 6).data().toInt());
    }
}



// Slot to handle add operation
void MainWindow::on_ajouterCl_clicked()
{
    int id = ui->id_2->text().toInt();
    QString nom = ui->nom_2->text();
    QString prenom = ui->prenom_2->text();
    QString location = ui->locationComboBox->currentText();
    int tel = ui->tel_2->text().toInt();
    QString email = ui->email_2->text();
    int nbor = ui->nborSpinBox->value();
    commande C(id, nom, prenom, location, tel, email, nbor);

    QString validationMessage = controlSaisie();
    if (validationMessage.isEmpty()) {
        if (C.ajouter()) {
            QMessageBox::information(this, QObject::tr("OK"),
                                     QObject::tr("Ajout effectué\n"), QMessageBox::Ok);
            loadcommandeData();  // Reload data
        } else {
            QMessageBox::critical(this, QObject::tr("Not OK"),
                                  QObject::tr("Ajout non effectué."), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::warning(this, "Input Error", validationMessage);
    }
}

// Slot to handle modify operation
void MainWindow::on_modifierCl_clicked()
{
    int id = ui->id_2->text().toInt();
    QString nom = ui->nom_2->text();
    QString prenom = ui->prenom_2->text();
    QString location = ui->locationComboBox->currentText();
    int tel = ui->tel_2->text().toInt();
    QString email = ui->email_2->text();
    int nbor = ui->nborSpinBox->value();
    commande C(id, nom, prenom, location, tel, email, nbor);

    QString validationMessage = controlSaisie();
    if (validationMessage.isEmpty()) {
        if (C.modifier(id)) {
            QMessageBox::information(this, QObject::tr("OK"),
                                     QObject::tr("Modification réussie."), QMessageBox::Ok);
            loadcommandeData();  // Reload data
        } else {
            QMessageBox::critical(this, QObject::tr("Update Failed"),
                                  QObject::tr("Modification échouée."), QMessageBox::Cancel);
        }
    } else {
        QMessageBox::warning(this, "Input Error", validationMessage);
    }
}

// Updated controlSaisie function with custom error messages
QString MainWindow::controlSaisie()
{
    QRegularExpression emailRegex(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
    QRegularExpression nameRegex(R"(^[A-Za-zÀ-ÿ\s-]{1,50}$)");
    QRegularExpression phoneRegex(R"(^\d{8,15}$)");

    if (ui->id_2->text().isEmpty())
        return "ID cannot be empty.";
    if (!nameRegex.match(ui->nom_2->text()).hasMatch())
        return "Nom must only contain letters and be 1-50 characters.";
    if (!nameRegex.match(ui->prenom_2->text()).hasMatch())
        return "Prenom must only contain letters and be 1-50 characters.";
    if (!phoneRegex.match(ui->tel_2->text()).hasMatch())
        return "Téléphone must be a valid number (8-15 digits).";
    if (!emailRegex.match(ui->email_2->text()).hasMatch())
        return "Email format is invalid.";
    if (ui->locationComboBox->currentText().isEmpty())
        return "Location cannot be empty.";

    return "";  // No errors found
}

void MainWindow::on_supprimerCl_clicked()
{
    QModelIndex selectedIndex = ui->tableView->currentIndex();

    if (!selectedIndex.isValid()) {
        QMessageBox::warning(this, "Selection Error", "Please select a row to delete.");
        return;
    }

    int row = selectedIndex.row();
    int id = ui->tableView->model()->index(row, 0).data().toInt();

    // Confirm delete action
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Deletion", "Are you sure you want to delete this record?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Use commande class's delete method for consistency
        commande cmd;
        if (cmd.supprimer(id)) {
            QMessageBox::information(this, "Success", "Record deleted successfully.");
            loadcommandeData();  // Reload data to update the view
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete the record.");
        }
    }
}


void MainWindow::on_pdfCl_clicked()
{
    loadcommandeData();

}


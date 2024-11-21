 #include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QRegularExpression>
#include <QPdfWriter>
#include <QTextTable>
#include <QDesktopServices>
#include <QPainter>
#include <QPrinter>
#include <QSortFilterProxyModel>
#include <QComboBox>
#include<QTextBrowser>
#include<QVBoxLayout>
#include <QFileDialog>
#include <QSslSocket>
#include <QTcpSocket>
#include <QGraphicsView>
#include <QtCharts>








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
    QSqlQuery query;
    query.exec("SELECT * FROM commande");

    // Set up the QPrinter for PDF export
    QPrinter printer(QPrinter::HighResolution);
    QPageSize pageSize(QPageSize::A4);  // A4 size
    QPageLayout pageLayout(pageSize, QPageLayout::Portrait, QMarginsF(20, 20, 20, 20)); // Set margins
    printer.setPageLayout(pageLayout);  // Apply the layout with margins

    // Begin painting on the printer (creates a QPainter object)
    QPainter painter;
    if (!painter.begin(&printer)) {
        qWarning("Failed to open PDF for writing.");
        return;
    }

    // Create a QTextDocument to hold your content
    QTextDocument pdfDoc;
    pdfDoc.setDocumentMargin(20);  // Margin for the document

    QTextCursor cursor(&pdfDoc);
    QTextCharFormat titleFormat;
    titleFormat.setFontWeight(QFont::Bold);
    titleFormat.setFontPointSize(16);

    QTextCharFormat headerFormat;
    headerFormat.setFontWeight(QFont::Bold);
    headerFormat.setFontPointSize(12);

    QTextCharFormat dataFormat;
    dataFormat.setFontPointSize(10);

    QTextTableFormat tableFormat;
    tableFormat.setCellPadding(8);
    tableFormat.setCellSpacing(2);
    tableFormat.setHeaderRowCount(1);
    tableFormat.setBorder(1);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    tableFormat.setBorderBrush(QBrush(QColor("#D3D3D3")));

    cursor.insertText("Données des commandes", titleFormat);
    cursor.insertBlock();
    cursor.insertBlock();

    QTextTable* table = cursor.insertTable(1, 7, tableFormat);  // 7 columns, adjust as per data

    // Header Row
    QStringList headerRow;
    headerRow << "id" << "First Name" << "Second Name" << "Location" << "Phone Number" << "Email Address" << "Number of Orders";

    // Insert header into table
    for (int i = 0; i < headerRow.size(); ++i) {
        QTextCursor cellCursor = table->cellAt(0, i).firstCursorPosition();
        cellCursor.insertText(headerRow[i], headerFormat);
    }

    // Insert data rows
    int row = 1;
    while (query.next()) {
        int id = query.value("id").toInt();
        QString nom = query.value("first name").toString();
        QString prenom = query.value("second name").toString();
        QString location = query.value("location").toString();
        int tel = query.value("phone number").toInt();
        QString email = query.value("email address").toString();
        int nbor = query.value("number of orders").toInt();

        table->insertRows(table->rows(), 1);

        table->cellAt(row, 0).firstCursorPosition().insertText(QString::number(id), dataFormat);
        table->cellAt(row, 1).firstCursorPosition().insertText(nom, dataFormat);
        table->cellAt(row, 2).firstCursorPosition().insertText(prenom, dataFormat);
        table->cellAt(row, 3).firstCursorPosition().insertText(location, dataFormat);
        table->cellAt(row, 4).firstCursorPosition().insertText(QString::number(tel), dataFormat);
        table->cellAt(row, 5).firstCursorPosition().insertText(email, dataFormat);
        table->cellAt(row, 6).firstCursorPosition().insertText(QString::number(nbor), dataFormat);

        row++;
    }

    // Finalize and save the PDF
    pdfDoc.drawContents(&painter);  // Render the document
    painter.end();  // End the painting process

    // Optionally, open the generated PDF
    QDesktopServices::openUrl(QUrl("exported_data.pdf"));
}








void MainWindow::on_chercher_idCl_cursorPositionChanged(int arg1, int arg2)
{
    bool ok;
        int id = ui->chercher_idCl->text().toInt(&ok);  // Convert the text from the QLineEdit to an integer

        if (!ok) {
            QMessageBox::warning(this, "Invalid Input", "Please enter a valid ID.");
            return;
        }

        // Create a query model to display only the record with the searched ID
        QSqlQueryModel *model = new QSqlQueryModel;
        QSqlQuery query;

        // Filter query by the entered ID
        query.prepare("SELECT * FROM commande WHERE id = :id");
        query.bindValue(":id", id);

        if (query.exec()) {
            // Set the filtered query as the model for the table view
            model->setQuery(query);
            ui->tableView->setModel(model);

            if (model->rowCount() == 0) {
                QMessageBox::warning(this, "Not Found", "Commande with the specified ID was not found.");
            }
        } else {
            QMessageBox::warning(this, "Error", "Failed to execute search query.");
        }

}


void MainWindow::on_pushButton_clicked()
{
    loadcommandeData();
}









void MainWindow::on_tricomboBox_activated(int index)
{

    // Assuming the model you're using is already set in the tableView
    QSqlQueryModel* model = dynamic_cast<QSqlQueryModel*>(ui->tableView->model());
    if (!model) {
        qWarning() << "No valid model set on tableView";
        return;
    }

    // Create a proxy model for sorting
    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);  // Set the source model

    switch (index) {
        case 0: // Sort by name (alphabetical order)
            proxyModel->sort(1, Qt::AscendingOrder);  // Sort by the 2nd column (index 1)
            break;

        case 1: // Sort by number of orders (ascending order)
            proxyModel->sort(6, Qt::AscendingOrder);  // Sort by the 7th column (index 6)
            break;

        default:
            break;
    }

    // Set the proxy model for the table view
    ui->tableView->setModel(proxyModel);
    ui->tableView->resizeColumnsToContents();  // Adjust the column widths
}
















void MainWindow::on_QR_code_clicked()
{
    QModelIndex selectedIndex = ui->tableView->currentIndex();

        if (!selectedIndex.isValid()) {
            QMessageBox::warning(this, "Selection Error", "Please select a row from the table.");
            return;
        }

        int row = selectedIndex.row();
        QString id = ui->tableView->model()->index(row, 0).data().toString();
        QString firstName = ui->tableView->model()->index(row, 1).data().toString();
        QString secondName = ui->tableView->model()->index(row, 2).data().toString();
        QString location = ui->tableView->model()->index(row, 3).data().toString();
        QString phone = ui->tableView->model()->index(row, 4).data().toString();
        QString email = ui->tableView->model()->index(row, 5).data().toString();
        QString nbor = ui->tableView->model()->index(row, 6).data().toString();

        // Combine data into a single string
        QString qrData = QString("ID: %1\nFirst Name: %2\nSecond Name: %3\nLocation: %4\nPhone: %5\nEmail: %6\nNumber of Orders: %7")
                             .arg(id, firstName, secondName, location, phone, email, nbor);

        // Generate the QR code using QZXing
        QZXing zxing;
        QImage qrImage = zxing.encodeData(qrData);

        if (qrImage.isNull()) {
            QMessageBox::critical(this, "QR Code Error", "Failed to generate the QR Code.");
            return;
        }

        // Display the QR code in a QLabel or save it as an image
        QString savePath = QFileDialog::getSaveFileName(this, "Save QR Code", "", "Images (*.png *.jpg *.bmp)");

        if (!savePath.isEmpty()) {
            if (qrImage.save(savePath)) {
                QMessageBox::information(this, "Success", "QR Code saved successfully!");
            } else {
                QMessageBox::critical(this, "Error", "Failed to save the QR Code.");
            }
        } else {
            // Optional: Display the QR code in a QLabel if not saving
            QLabel *qrLabel = new QLabel(this);
            qrLabel->setPixmap(QPixmap::fromImage(qrImage));
            qrLabel->setWindowTitle("Generated QR Code");
            qrLabel->show();
        }

}


void MainWindow::on_statistiqueCl_clicked()
{
    // Remove any existing widgets from the stat layout
            QLayout *layout = ui->stat->layout();
            if (layout != nullptr) {
                QLayoutItem *item;
                while ((item = layout->takeAt(0)) != nullptr) {
                    delete item->widget();  // Delete the widget if it exists
                    delete item;  // Delete the layout item
                }
            }

            QMap<QString, int> stats = C.getCountByAdresse();
            QPieSeries *series = new QPieSeries();

            // Populate the series with data
            int total = 0;
            for (auto it = stats.begin(); it != stats.end(); ++it) {
                total += it.value();
            }

            for (auto it = stats.begin(); it != stats.end(); ++it) {
                QPieSlice *slice = series->append(it.key() + " (" + QString::number(it.value()) + ")", it.value());
                slice->setLabelVisible(true);

                // Explode slices with values > 25% of total
                if (it.value() > 0.25 * total) {
                    slice->setExploded(true);
                    slice->setExplodeDistanceFactor(0.1);  // Adjust explode distance
                    slice->setLabelColor(Qt::red);  // Highlight large values with red labels
                    slice->setBrush(Qt::darkRed);  // Highlight slice color
                }
            }

            QChar *chart = new QChar();
            chart->addSeries(series);
            chart->setTitle("Nombre des fournisseurs par gouvernement:");
            chart->setTitleFont(QFont("Arial", 14, QFont::Bold));  // Bigger, bold title font
            chart->legend()->setAlignment(Qt::AlignRight);
            chart->legend()->setFont(QFont("Arial", 10));  // Larger legend font

            // Create a chart view and set it up
            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);

            // Add the new chart view to the layout
            layout->addWidget(chartView);
}


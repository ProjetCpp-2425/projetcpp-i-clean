 #include "mainwindow.h"
#include "ui_mainwindow.h"
#include "arduino.h"
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
<<<<<<< HEAD
#include <QGraphicsView>
#include <QtCharts>
=======
#include <QVBoxLayout>
#include "qrcode.h"
#include <QLabel>
#include "dialog.h"
#include "dialog.h"
>>>>>>> f86bd98 (gestion commande)







<<<<<<< HEAD
=======
QString MainWindow::saveQRCodeToFile(const QString &data, const QString &filePath) {
    qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(data.toUtf8().constData(), qrcodegen::QrCode::Ecc::MEDIUM);

    int size = qr.getSize();
    QImage image(size, size, QImage::Format_RGB32);
    QRgb black = qRgb(0, 0, 0);
    QRgb white = qRgb(255, 255, 255);

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            image.setPixel(x, y, qr.getModule(x, y) ? black : white);
        }
    }

    // Save the QR code as an image
    if (image.save(filePath)) {
        qDebug() << "QR code saved to:" << filePath;
        return filePath;
    } else {
        qDebug() << "Failed to save QR code to:" << filePath;
        return "";
    }
}

void MainWindow::on_email_3_clicked() {
    QString destinataire = "alarezgui98@gmail.com";
    QString objet = "Confirmation de votre réservation";
    QString corps = "Bonjour,\n\nVotre commande est confirmée.\n\nMerci de votre confiance.";

    // Generate and save the QR code
    QString data = "Example Reservation Details";  // Replace with real data
    QString filePath = QFileDialog::getSaveFileName(this, "Save QR Code", "", "Images (*.png)");
    if (!filePath.isEmpty()) {
        QString savedFile = saveQRCodeToFile(data, filePath);
        if (!savedFile.isEmpty()) {
            // Attach QR code to the email and send
            envoyerEmail(destinataire, objet, corps, savedFile);
        } else {
            QMessageBox::warning(this, "Error", "Failed to save the QR code.");
        }
    }
}

// Updated envoyerEmail to send an email with an attachment
void MainWindow::envoyerEmail(const QString &destinataire, const QString &sujet, const QString &message, const QString &attachmentPath) {
    QString smtpServer = "smtp.gmail.com";
    int smtpPort = 465;  // Use SSL
    QString from = "aziz228nasri@gmail.com";
    QString password = "paqd yivs yzja uyxa";

    QString emailBody = "From: " + from + "\r\n" +
                        "To: " + destinataire + "\r\n" +
                        "Subject: " + sujet + "\r\n\r\n" +
                        message;

    QSslSocket socket;
    socket.connectToHostEncrypted(smtpServer, smtpPort);
    if (!socket.waitForConnected()) {
        qDebug() << "SMTP connection error:" << socket.errorString();
        return;
    }

    qDebug() << "SMTP connected successfully.";

    // Send SMTP commands
    socket.write("EHLO localhost\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "EHLO response:" << socket.readAll();

    socket.write("AUTH LOGIN\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "AUTH LOGIN response:" << socket.readAll();

    socket.write(QByteArray().append(from.toUtf8()).toBase64() + "\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Email (AUTH) response:" << socket.readAll();

    socket.write(QByteArray().append(password.toUtf8()).toBase64() + "\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Password (AUTH) response:" << socket.readAll();

    socket.write("MAIL FROM:<" + from.toUtf8() + ">\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "MAIL FROM response:" << socket.readAll();

    socket.write("RCPT TO:<" + destinataire.toUtf8() + ">\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "RCPT TO response:" << socket.readAll();

    socket.write("DATA\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "DATA response:" << socket.readAll();

    // Compose email body with attachment
    QFile attachment(attachmentPath);
    if (attachment.open(QIODevice::ReadOnly)) {
        QByteArray attachmentData = attachment.readAll();
        QString encodedAttachment = attachmentData.toBase64();

        emailBody += "\r\n--boundary\r\n"
                     "Content-Type: image/png\r\n"
                     "Content-Disposition: attachment; filename=\"qrcode.png\"\r\n"
                     "Content-Transfer-Encoding: base64\r\n\r\n" +
                     encodedAttachment +
                     "\r\n--boundary--";
    }

    socket.write(emailBody.toUtf8() + "\r\n.\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Email body response:" << socket.readAll();

    socket.write("QUIT\r\n");
    socket.waitForBytesWritten();
    socket.close();
    qDebug() << "Email sent successfully.";
}






>>>>>>> f86bd98 (gestion commande)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    myArduino() // Initialize the Arduino instance
{
    ui->setupUi(this);

    // Populate the ComboBox with predefined locations
    ui->locationComboBox->addItems({
        "Tunis", "Sfax", "Sousse", "Gabès", "Bizerte", "Ariana", "Kairouan",
        "Gafsa", "La Marsa", "Monastir", "Nabeul", "Ben Arous", "Hammamet",
        "Mahdia", "Zarzis", "Douz", "Tozeur", "Kebili", "Beja", "Jendouba",
        "Siliana", "Kasserine", "Le Kef", "Medenine", "Tataouine"
    });

    setupArduinoConnection();


    // Connect signals for row selection and double-click
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &MainWindow::onRowSelected);

    connect(ui->tableView, &QTableView::doubleClicked,
            this, &MainWindow::onRowSelected);

    // Load initial data for the table view
    loadcommandeData();
}


void MainWindow::setStackedWidgetPage(int index)
{
    ui->stackedWidget->setCurrentIndex(index); // Change the page
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
    // Close Arduino connection before exiting

    delete ui;
}

void MainWindow::setupArduinoConnection()
{
    int result = myArduino.connectArduino();
    if (result == 0) {
        // Successfully connected
        connect(&myArduino, &Arduino::dataReceived,
                this, &MainWindow::handleArduinoInput);
        qDebug() << "Arduino connected successfully";
    } else {
        QMessageBox::warning(this, "Connection Error",
                              "Could not connect to Arduino");
    }
}

void MainWindow::handleArduinoInput(const QByteArray &input)
{
    // Trim and convert input to QString
    QString nom = QString::fromUtf8(input.trimmed());

    // Debug output
    qDebug() << "Received from Arduino:" << nom;

    if (!nom.isEmpty()) {
        // Update the search field
        ui->chercher_idCl->setText(nom);

        // Perform the search
        performSearch(nom);
    }
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
<<<<<<< HEAD
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
=======
    query.exec("SELECT id, nom, prenom, location, tel, email, nbor FROM commande");
>>>>>>> f86bd98 (gestion commande)

    // Set up QPrinter with smaller page size (A5)
    QPrinter printer(QPrinter::HighResolution);
    QPageSize pageSize(QPageSize::A5);
    QPageLayout pageLayout(pageSize, QPageLayout::Portrait, QMarginsF(15, 15, 15, 15)); // Compact margins
    printer.setPageLayout(pageLayout);

    // Begin painting
    QPainter painter;
    if (!painter.begin(&printer)) {
        qWarning("Failed to open PDF for writing.");
        return;
    }

    // Create QTextDocument
    QTextDocument pdfDoc;
    pdfDoc.setDocumentMargin(15);

    QTextCursor cursor(&pdfDoc);

    // Title Format (Centered and underlined)
    QTextCharFormat titleFormat;
    titleFormat.setFontWeight(QFont::Bold);
    titleFormat.setFontPointSize(20); // Larger title
    titleFormat.setForeground(QBrush(QColor("#2A5298"))); // Blue color

    QTextBlockFormat titleBlockFormat;
    titleBlockFormat.setAlignment(Qt::AlignCenter); // Center-align title

    cursor.insertBlock(titleBlockFormat);
    cursor.insertText("Données des commandes\n", titleFormat);
    cursor.insertBlock(titleBlockFormat);
    cursor.insertHtml("<hr>"); // Decorative underline

    // Table Header Format
    QTextCharFormat headerFormat;
    headerFormat.setFontWeight(QFont::Bold);
    headerFormat.setFontPointSize(14); // Larger header font
    headerFormat.setForeground(QBrush(Qt::white)); // White text
    headerFormat.setBackground(QBrush(QColor("#4CAF50"))); // Green header background

    // Data Format
    QTextCharFormat dataFormat;
    dataFormat.setFontPointSize(12); // Larger data font

    // Alternating Row Colors
    QTextCharFormat rowFormat1;
    rowFormat1.setBackground(QBrush(QColor("#F5F5F5"))); // Light gray

    QTextCharFormat rowFormat2;
    rowFormat2.setBackground(QBrush(QColor("#FFFFFF"))); // White

    // Table Format
    QTextTableFormat tableFormat;
    tableFormat.setCellPadding(6);
    tableFormat.setCellSpacing(2);
    tableFormat.setBorder(1);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    tableFormat.setBorderBrush(QBrush(QColor("#D3D3D3")));

    // Create table with 7 columns
    QTextTable* table = cursor.insertTable(1, 7, tableFormat);

    // Table Headers
    QStringList headers = {"ID", "First Name", "Last Name", "Location", "Phone Number", "Email", "Orders Count"};
    for (int i = 0; i < headers.size(); ++i) {
        QTextCursor cellCursor = table->cellAt(0, i).firstCursorPosition();
        cellCursor.insertText(headers[i], headerFormat);
    }

    // Insert Data Rows
    int row = 1;
    while (query.next()) {
        table->insertRows(table->rows(), 1);

        int id = query.value("id").toInt();
        QString nom = query.value("nom").toString();
        QString prenom = query.value("prenom").toString();
        QString location = query.value("location").toString();
        QString tel = query.value("tel").toString();
        QString email = query.value("email").toString();
        int nbor = query.value("nbor").toInt();

        QTextCharFormat* rowFormat = (row % 2 == 0) ? &rowFormat1 : &rowFormat2; // Alternate row colors

        table->cellAt(row, 0).firstCursorPosition().insertText(QString::number(id), *rowFormat);
        table->cellAt(row, 1).firstCursorPosition().insertText(nom, *rowFormat);
        table->cellAt(row, 2).firstCursorPosition().insertText(prenom, *rowFormat);
        table->cellAt(row, 3).firstCursorPosition().insertText(location, *rowFormat);
        table->cellAt(row, 4).firstCursorPosition().insertText(tel, *rowFormat);
        table->cellAt(row, 5).firstCursorPosition().insertText(email, *rowFormat);
        table->cellAt(row, 6).firstCursorPosition().insertText(QString::number(nbor), *rowFormat);

        row++;
    }

    // Render document and finalize PDF
    pdfDoc.drawContents(&painter);
    painter.end();

    // Save PDF File
    QString filePath = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");
    if (!filePath.isEmpty()) {
        printer.setOutputFileName(filePath);
        QMessageBox::information(this, "PDF Export", "PDF file successfully created!");
    }
}


<<<<<<< HEAD
=======






void MainWindow::on_chercher_idCl_textChanged(const QString &text)
{
    // This method will be called when the text is changed
    performSearch(text);
}

void MainWindow::performSearch(const QString &searchTerm)
{
    if (searchTerm.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input",
                              "Please enter a valid ID.");
        return;
    }

    // Create a new query model
    QSqlQueryModel *model = new QSqlQueryModel(this);
    QSqlQuery query;

    // Prepare the SQL query to search by ID
    query.prepare("SELECT * FROM commande WHERE id = :id");
    query.bindValue(":id", searchTerm);

    // Execute the query
    if (query.exec()) {
        model->setQuery(query);
        ui->tableView->setModel(model);

        if (model->rowCount() == 0) {
            QMessageBox::information(this, "Not Found",
                                     "No records found with the specified ID.");
        }
    } else {
        QMessageBox::warning(this, "Error",
                              "Failed to execute search query.");
    }
}





>>>>>>> f86bd98 (gestion commande)
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












<<<<<<< HEAD




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

=======
void MainWindow::on_statistiqueCl_clicked()
{
    // Fetch statistics based on locations
    QMap<QString, int> stats = C.getCountByAdresse();  // Make sure this function is working correctly

    if (stats.isEmpty()) {
        qDebug() << "No data available for statistics.";
        return;
    }

    // Calculate the total value of all locations
    int total = 0;
    for (auto it = stats.begin(); it != stats.end(); ++it) {
        total += it.value();
    }

    // Create HTML content to display in QTextBrowser
    QString htmlContent = "<html><body>";

    htmlContent += "<h2>Statistiques des commandes par lieu</h2>";
    htmlContent += "<p><b>Total des commandes: " + QString::number(total) + "</b></p>";

    // Create an HTML table to display the statistics
    htmlContent += "<table border='1' style='border-collapse: collapse; width: 100%;'>";
    htmlContent += "<tr><th>Location</th><th>Commandes</th><th>Pourcentage</th></tr>";

    for (auto it = stats.begin(); it != stats.end(); ++it) {
        QString location = it.key();
        int count = it.value();
        float percentage = (float(count) / float(total)) * 100;

        htmlContent += "<tr>";
        htmlContent += "<td>" + location + "</td>";
        htmlContent += "<td>" + QString::number(count) + "</td>";
        htmlContent += "<td>" + QString::number(percentage, 'f', 2) + " %</td>";
        htmlContent += "</tr>";
    }

    htmlContent += "</table>";

    // Optionally, add a pie chart-like view or other data representation in HTML
    htmlContent += "<br><p>Note: This is a simplified view of the data.</p>";
    htmlContent += "</body></html>";

    // Set the HTML content in the QTextBrowser (ui->stat)
    ui->stat->setHtml(htmlContent);  // Assuming ui->stat is a QTextBrowser
}


void MainWindow::on_stat_anchorClicked(const QUrl &arg1)
{
    // Assuming arg1 refers to the anchor or the URL clicked, you can handle it here if needed.
    // In this case, we want to just show the statistics after the button is clicked.

    // You can keep this function to process other URL actions if required, but for now,
    // it's triggered by the statistics button click to show statistics.
}






void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    if (index.isValid()) {
           QString id = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0)).toString();
           QString nom = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 1)).toString();
           QString prenom = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 2)).toString();

           QString data = id + " " + nom + " " + prenom;
           qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(data.toUtf8().constData(), qrcodegen::QrCode::Ecc::MEDIUM);

           // Créer l'image du code QR
           int size = qr.getSize();
           QImage image(size, size, QImage::Format_RGB32);
           QRgb black = qRgb(0, 0, 0);
           QRgb white = qRgb(255, 255, 255);
           for (int y = 0; y < size; y++) {
               for (int x = 0; x < size; x++) {
                   image.setPixel(x, y, qr.getModule(x, y) ? black : white);
               }
           }

           // Afficher l'image du code QR
           ui->qrLabel->setPixmap(QPixmap::fromImage(image.scaled(100, 100, Qt::KeepAspectRatio, Qt::FastTransformation)));
       }
}


void MainWindow::on_chercher_location_cursorPositionChanged(int arg1, int arg2)
{
    QString location = ui->chercher_location->text();  // Get the location entered by the user

        // If location is empty, display a warning and exit
        if (location.isEmpty()) {
            QMessageBox::warning(this, "Invalid Input", "Please enter a valid location.");
            return;
        }

        // Create a query model to display filtered records
        QSqlQueryModel *model = new QSqlQueryModel;
        QSqlQuery query;

        // Prepare the query to search by location
        QString queryString = "SELECT * FROM commande WHERE location LIKE :location";

        query.prepare(queryString);

        // Bind the search criteria with wildcard for partial matching
        query.bindValue(":location", "%" + location + "%");  // Wildcard for location search

        // Execute the query and update the model if successful
        if (query.exec()) {
            model->setQuery(query);
            ui->tableView->setModel(model);

            // Show a warning if no records match the search criteria
            if (model->rowCount() == 0) {
                QMessageBox::information(this, "No Results", "No records matching the location were found.");
            }
        } else {
            // Handle query execution failure
            QMessageBox::warning(this, "Error", "Failed to execute the location search query. Please try again.");
        }
}


void MainWindow::on_Back_clicked()
{
    // Hide the main window first
    this->close();

    // Show the dialog window
    Dialog dialog(this);
    dialog.exec();
}
>>>>>>> f86bd98 (gestion commande)

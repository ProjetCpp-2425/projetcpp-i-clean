#include "mainwindow.h"
#include "qsqlerror.h"
#include "ui_mainwindow.h"
#include "housekeeper.h"
#include "QMessageBox"
#include "QSqlQueryModel"
#include "QSqlQuery"
#include "QFileDialog"
#include <QPdfWriter>
#include <QPainter>
#include <QTextDocument>
#include <QJsonDocument>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QKeyEvent>



#include <QDebug>


#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QColor>
#include <QTextCharFormat>



#include <QSerialPort>
#include <QSerialPortInfo>
QString buffer;


QSerialPort *serial;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupSerialPort();
    ui->stackedWidget->setFocusPolicy(Qt::StrongFocus);  // ou Qt::FocusPolicy::ClickFocus
    ui->stackedWidget->setFocus();

    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::on_addButton_clicked);
    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::on_settingsButton_clicked);
    connect(ui->notifButton, &QPushButton::clicked, this, &MainWindow::on_notifButton_clicked);
    connect(ui->salaireButton, &QPushButton::clicked, this, &MainWindow::on_salaireButton_clicked);


    // Connectez le bouton à la fonction de démarrage de la surveillance
    connect(ui->startMonitoringButton, &QPushButton::clicked, this, &MainWindow::startMonitoring);


    serialPort = new QSerialPort(this);

    // Configurez les paramètres du port série
    serialPort->setPortName("COM5"); // Changez "COM3" selon votre configuration
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    // Connectez le signal `readyRead` au slot
    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::onSerialDataReceived);

    // Ouvrir le port série
    if (!serialPort->open(QIODevice::ReadOnly)) {
        qDebug() << "Erreur : Impossible d'ouvrir le port série.";
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Obtenez l'index actuel de la page affichée dans le QStackedWidget
    int currentIndex = ui->stackedWidget->currentIndex();

    // Si la touche flèche droite est pressée
    if (event->key() == Qt::Key_Right) {
        int nextIndex = currentIndex + 1;
        if (nextIndex < ui->stackedWidget->count()) {
            // Passe à la page suivante
            ui->stackedWidget->setCurrentIndex(nextIndex);
        }
    }
    // Si la touche flèche gauche est pressée
    else if (event->key() == Qt::Key_Left) {
        int prevIndex = currentIndex - 1;
        if (prevIndex >= 0) {
            // Passe à la page précédente
            ui->stackedWidget->setCurrentIndex(prevIndex);
        }
    }
    else {
        // Si une autre touche est pressée, laisse l'événement se propager
        QWidget::keyPressEvent(event);
    }
}
void MainWindow::on_addButton_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->page_add);
}

void MainWindow::on_settingsButton_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->page_settings);
}

void MainWindow::on_notifButton_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->page_notif);
}

void MainWindow::on_salaireButton_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->page_salaire);
}





void MainWindow::on_pushButton_ajouter_clicked()
{
    QString nomf = ui->nom->text();
    QString prenomf = ui->prenom->text();
    QString adressef = ui->adresse->text();
    QString idf_str = ui->id->text();
    QString telf = ui->tel->text();
    //  QString type = ui->type->text();
    // QString nb_taches = ui->nb_taches->text();


    // Conversion des champs numériques avec vérification
    bool okId; //, okNbTaches = false, okSalaire;
    int idf = idf_str.toInt(&okId);
    //  int salaire = ui->lineEdit_5->text().toInt(&okSalaire);

    // Vérification de l'ID
    if (!okId || idf_str.isEmpty()) {
        QMessageBox::warning(this, QObject::tr("Erreur ID"), QObject::tr("L'ID doit être un entier valide."));
        return;
    }

    // Vérification si l'ID existe déjà
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM HOUSEKEEPER WHERE IDF = :idf");
    query.bindValue(":idf", idf);
    query.exec();

    if (query.next()) {
        int count = query.value(0).toInt();
        if (count > 0) {
            // Mise à jour du housekeeper existant
            Housekeeper H(nomf, prenomf, adressef, idf, telf);
            bool test = H.modifier();
            if (test) {
                QMessageBox::information(this, QObject::tr("Succès"), QObject::tr("Les informations ont été mises à jour avec succès!"));
            } else {
                QMessageBox::critical(this, QObject::tr("Erreur"), QObject::tr("Erreur lors de la mise à jour des informations."));
            }
        } else {
            // Ajout d'un nouveau housekeeper
            Housekeeper H(nomf, prenomf, adressef, idf, telf);
            bool test = H.ajouter();
            if (test) {
                QMessageBox::information(this, QObject::tr("Succès"), QObject::tr("Ajout effectué avec succès!"));
            } else {
                QMessageBox::critical(this, QObject::tr("Erreur"), QObject::tr("Erreur lors de l'ajout du housekeeper."));
            }
        }
    }
}



void MainWindow::on_pushButton_supprimer_clicked()
{
    int idf = ui->lineEditSupprimer->text().toInt();
    Housekeeper Htmp;
    bool test = Htmp.supprimer(idf);
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Succès"), QObject::tr("Suppression effectuée avec succès!"));
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Erreur lors de la suppression du housekeeper."));
    }
}



void MainWindow::on_pushButton_afficher_clicked()
{
    QString adresse = ui->lineEditAfficher->text();
    Housekeeper Htmp;
    model = Htmp.afficher(adresse);
    ui->tableView1->setModel(model);

    if (model->rowCount() == 0) {
        QMessageBox::information(this, QObject::tr("Aucun Résultat"), QObject::tr("Aucun housekeeper trouvé pour l'adresse spécifiée."));
    }
}



void MainWindow::on_pushButton_modifier_clicked()
{
    int idf = ui->id->text().toInt();
    QSqlQuery query;
    query.prepare("SELECT * FROM HOUSEKEEPER WHERE IDF = :idf");
    query.bindValue(":idf", idf);

    if (query.exec() && query.next()) {
        ui->id->setText(query.value(0).toString());
        ui->nom->setText(query.value(1).toString());
        ui->prenom->setText(query.value(2).toString());
        ui->adresse->setText(query.value(3).toString());
        ui->tel->setText(query.value(4).toString());
    } else {
        QMessageBox::warning(this, QObject::tr("Erreur"), QObject::tr("ID introuvable dans la base de données."));
    }
}



void MainWindow::on_pushButton_rechercher_clicked() {
    int idf = -1;  // Par défaut, pas de recherche par ID
    QString adressef;
    QString telf;

    // Vérifie quel champ est utilisé pour la recherche
    if (!ui->lineEditSupprimer->text().isEmpty()) {
        bool ok;
        idf = ui->lineEditSupprimer->text().toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, QObject::tr("Erreur de saisie"), QObject::tr("Veuillez entrer un ID valide (entier)."));
            return;
        }
    } else if (!ui->lineEditAfficher->text().isEmpty()) {
        adressef = ui->lineEditAfficher->text();
    } else if (!ui->lineEditTel->text().isEmpty()) {
        telf = ui->lineEditTel->text();
    } else {
        QMessageBox::warning(this, QObject::tr("Critère manquant"), QObject::tr("Veuillez entrer un critère de recherche."));
        return;
    }

    // Appel de la méthode rechercher
    Housekeeper Htmp;
    model = Htmp.rechercher(idf, adressef, telf);
    ui->tableView1->setModel(model);

    // Vérifier les résultats
    if (model->rowCount() == 0) {
        QMessageBox::information(this, QObject::tr("Aucun Résultat"), QObject::tr("Aucun housekeeper trouvé pour le critère spécifié."));
    }
}





void MainWindow::on_pushButton_trier_clicked() {
    QString critere = ui->comboBox_tri->currentText(); // Obtenir le critère sélectionné
    Housekeeper Htmp;

    if (critere == "sort by adding date") {
        model = Htmp.trier();
    } else if (critere == "sort by adress") {
        model = Htmp.trierParAdresse();
    } else if (critere == "sort by name") {
        model = Htmp.trierParPrenom();
    } else {
        QMessageBox::warning(this, QObject::tr("Erreur"), QObject::tr("Critère de tri invalide."));
        return;
    }

    ui->tableView1->setModel(model);

    if (model->rowCount() == 0) {
        QMessageBox::information(this, QObject::tr("Aucun Résultat"), QObject::tr("Aucun housekeeper trouvé."));
    }
}



void MainWindow::on_pushButton_pdf_clicked()
{
    QFile pdfFile("D:/Oracle/Atelier_Connexion/housekeepers_list.pdf");
    if (pdfFile.open(QIODevice::WriteOnly)) {
        QPdfWriter writer(&pdfFile);
        writer.setPageSize(QPageSize::A4);
        QPainter painter(&writer);
        QFont font = painter.font();
        font.setPointSize(200);
        painter.setFont(font);

        QTextDocument doc;
        QString content = "<h1 style='font-size: 50pt;'>Housekeepers List</h1>"
                          "<table border='1' cellpadding='5' cellspacing='0' style='font-size: 25pt;'>"
                          "<tr><th>ID</th><th>Last Name</th><th>Name</th><th>Address</th><th>Phone number</th></tr>";
        QSqlQuery query("SELECT IDF, NOMF, PRENOMF, ADRESSEF,TELF FROM HOUSEKEEPER");
        while (query.next()) {
            content += "<tr>"
                       "<td>" + query.value("IDF").toString() + "</td>"
                                                         "<td>" + query.value("NOMF").toString() + "</td>"
                                                          "<td>" + query.value("PRENOMF").toString() + "</td>"
                                                             "<td>" + query.value("ADRESSEF").toString() + "</td>"
                                                              "<td>" + query.value("TELF").toString() + "</td>"
                                                          "</tr>";
        }

        content += "</table>";
        doc.setHtml(content);
        doc.drawContents(&painter);
        painter.end();
    }
    QMessageBox::information(this, "PDF généré", "Le fichier PDF des housekeepers a été mis à jour avec succès.");
}




void MainWindow::on_pushButton_pdf_2_clicked()
{
    QFile pdfFile("D:/Oracle/Atelier_Connexion/housekeepers_list.pdf");
    if (pdfFile.open(QIODevice::WriteOnly)) {
        QPdfWriter writer(&pdfFile);
        writer.setPageSize(QPageSize::A4);
        QPainter painter(&writer);
        QFont font = painter.font();
        font.setPointSize(200);
        painter.setFont(font);

        QTextDocument doc;
        QString content = "<h1 style='font-size: 50pt;'>Housekeepers List</h1>"
                          "<table border='1' cellpadding='5' cellspacing='0' style='font-size: 25pt;'>"
                          "<tr><th>ID</th><th>Last Name</th><th>Name</th><th>Address</th><th>Phone number</th></tr>";
        QSqlQuery query("SELECT IDF, NOMF, PRENOMF, ADRESSEF, TELF FROM HOUSEKEEPER");
        while (query.next()) {
            content += "<tr>"
                       "<td>" + query.value("IDF").toString() + "</td>"
                                                         "<td>" + query.value("NOMF").toString() + "</td>"
                                                          "<td>" + query.value("PRENOMF").toString() + "</td>"
                                                             "<td>" + query.value("ADRESSEF").toString() + "</td>"
                                                              "<td>" + query.value("TELF").toString() + "</td>"
                                                          "</tr>";
        }

        content += "</table>";
        doc.setHtml(content);
        doc.drawContents(&painter);
        painter.end();
    }
    QMessageBox::information(this, "PDF généré", "Le fichier PDF des housekeepers a été mis à jour avec succès.");
}



void MainWindow::on_pushButton_stat_clicked() {
    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphic->setScene(scene);

    // Nouvelle requête pour obtenir les statistiques selon la localisation
    QString queryStr = "SELECT adressef, COUNT(*) AS frequency "
                       "FROM HOUSEKEEPER "
                       "GROUP BY adressef "
                       "ORDER BY frequency DESC";

    QSqlQuery query;
    if (!query.exec(queryStr)) {
        QMessageBox::critical(this, "Erreur", "Échec de l'exécution de la requête : " + query.lastError().text());
        return;
    }

    int barWidth = 60;
    int spacing = 25;
    int x = 100;
    int maxBarHeight = 300;
    QFont font("Helvetica", 14, QFont::Bold);
    int maxFrequency = 1;

    // Calculer le maximum de la fréquence
    while (query.next()) {
        int frequency = query.value("frequency").toInt();
        if (frequency > maxFrequency) {
            maxFrequency = frequency;
        }
    }

    query.first(); // Réinitialiser le curseur pour lire les données à nouveau

    int yAxisHeight = maxBarHeight + 50; // Hauteur de l'axe Y
    int xAxisLength = query.size() * (barWidth + spacing); // Longueur de l'axe X

    // Dessiner les axes
    scene->addLine(100, 0, 100, -yAxisHeight, QPen(Qt::black));
    scene->addLine(100, 0, 800 + xAxisLength, 0, QPen(Qt::black));

    QGraphicsTextItem *yAxisLabel = scene->addText("Fréquence", font);
    yAxisLabel->setPos(10, -yAxisHeight - 30);

    QGraphicsTextItem *xAxisLabel = scene->addText("Localisation", font);
    xAxisLabel->setPos(50 + xAxisLength / 2, 30);

    // Dessiner les barres du graphique
    do {
        QString adresse = query.value("adressef").toString();
        int frequency = query.value("frequency").toInt();

        int barHeight = (frequency * maxBarHeight) / maxFrequency;

        // Couleur de la barre par défaut
        QColor barColor = Qt::blue; // Changez la couleur si nécessaire

        QGraphicsRectItem *bar = scene->addRect(x, -barHeight, barWidth, barHeight, QPen(Qt::black), QBrush(barColor));
        bar->setPos(x + 2, 0);

        QGraphicsTextItem *textItem = scene->addText(adresse, font);
        textItem->setPos(x * 1.75 + barWidth - 10, 0);

        QGraphicsTextItem *countText = scene->addText(QString::number(frequency), font);
        countText->setPos(x * 2 + barWidth / 4, -barHeight - 40);

        x += barWidth + spacing;
    } while (query.next());

    ui->graphic->setRenderHint(QPainter::Antialiasing);
    ui->graphic->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

    qDebug() << "Graphique des statistiques généré avec succès.";
}



void MainWindow::on_pushButton_calculer_clicked() {
    // Récupération des données de l'interface utilisateur
    QString type = ui->comboBox1->currentText(); // Exemple d'un comboBox pour sélectionner le type
    QString nb_taches = ui->comboBox2->currentText();     // Exemple d'un spinBox pour le nombre de tâches

    // Création d'une instance de Housekeeper
    Housekeeper housekeeper;

    // Calcul du salaire
    int salaire = housekeeper.calculerSalaire(type, nb_taches);

    // Affichage du résultat dans l'interface
    ui->label_afficher_salaire->setText(QString::number(salaire) + " DT");
}


void MainWindow::on_pushButton_envoyer_clicked() {
    sendSms();
}

void MainWindow::sendSms()
{
    // Récupération des informations des champs de texte de l'interface utilisateur
    QString telf = ui->lineEdit_tel->text(); // Récupération du numéro de téléphone
    QString localisation = ui->lineEdit_localisation_client->text(); // Récupération de la localisation

    // Validation des cases à cocher
    bool classic_clean = ui->classic_clean->isChecked();
    bool smart_clean = ui->smart_clean->isChecked();
    bool urgence = ui->urgence->isChecked();

    if ((classic_clean && smart_clean) || (!classic_clean && !smart_clean && !urgence)) {
        QMessageBox::warning(this, QObject::tr("Erreur"), QObject::tr("Vous devez choisir un type de nettoyage."));
        return;
    }

    // Twilio credentials
    const QString accountSid = "ACc0626fa295fbde3b6c2b3f76d35032a3";
    const QString authToken = "4d04955b8ae13125917ddc190ce9c117";
    const QString fromPhoneNumber = "+12512441041"; // Remplacer par votre numéro Twilio

    // API endpoint
    QUrl apiUrl(QString("https://api.twilio.com/2010-04-01/Accounts/%1/Messages.json").arg(accountSid));

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request(apiUrl);

    QString authValue = "Basic " + QString(QByteArray(QString("%1:%2").arg(accountSid).arg(authToken).toUtf8()).toBase64());
    request.setRawHeader("Authorization", authValue.toUtf8());

    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    // Préparation des parties du message
    QHttpPart toPart;
    toPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"To\""));
    toPart.setBody(telf.toUtf8());

    QHttpPart fromPart;
    fromPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"From\""));
    fromPart.setBody(fromPhoneNumber.toUtf8());

    QHttpPart bodyPart;
    QString message = "Localisation: " + localisation + "\n";
    if (classic_clean) {
        message += "Type de nettoyage: Nettoyage classique\n";
    }
    if (smart_clean) {
        message += "Type de nettoyage: Nettoyage intelligent\n";
    }
    if (urgence) {
        message += "Type de nettoyage: Urgence\n";
    }
    bodyPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"Body\""));
    bodyPart.setBody(message.toUtf8());

    // Ajout des parties au multiPart
    multiPart->append(toPart);
    multiPart->append(fromPart);
    multiPart->append(bodyPart);

    // Envoi de la requête
    QNetworkReply* reply = manager->post(request, multiPart);
    multiPart->setParent(reply); // Associe le multiPart à la réponse pour le nettoyage automatique

    // Connecte le signal finished à la méthode onSmsSent
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        onSmsSent(reply);
    });
}

void MainWindow::onSmsSent(QNetworkReply* reply)
{
    // Vérifie si le SMS a été envoyé avec succès
    if (reply->error() == QNetworkReply::NoError) {
        QMessageBox::information(this, "Succès", "SMS envoyé avec succès!");
    } else {
        // Gérer l'erreur et afficher la réponse de l'API Twilio
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

        // Si Twilio renvoie un message d'erreur dans la réponse
        QString errorMessage = jsonResponse["message"].toString();

        qDebug() << "Erreur lors de l'envoi du SMS:" << reply->errorString();
        qDebug() << "Réponse Twilio:" << jsonResponse;

        QMessageBox::warning(this, "Erreur", "Échec de l'envoi du SMS: " + errorMessage);
    }

    // Nettoyage après la réponse
    reply->deleteLater();
}








void MainWindow::setupSerialPort() {
    serial = new QSerialPort(this);
    serial->setPortName("COM5");  // Remplacez par votre port réel

    // Vérifiez si le port est disponible
    bool portFound = false;
    foreach (const QSerialPortInfo &portInfo, QSerialPortInfo::availablePorts()) {
        if (portInfo.portName() == "COM5") {
            portFound = true;
            break;
        }
    }

    if (!portFound) {
        QMessageBox::critical(this, "Erreur", "Le port série spécifié (COM5) n'est pas disponible.");
        return;
    }

    // Configuration du port série
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);

    if (serial->open(QIODevice::ReadOnly)) {
        connect(serial, &QSerialPort::readyRead, this, &MainWindow::onSerialDataReceived);
        ui->alertLabel->setText("Port série configuré. Surveillance active.");
    } else {
        qDebug() << "Erreur lors de l'ouverture du port série : " << serial->errorString();
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le port série.");
    }
}

void MainWindow::readSerialData() {
    QByteArray data = serial->readAll(); // Lire les données
    QString message = QString::fromUtf8(data).trimmed();

    if (message.startsWith("DATA:")) {
        QStringList parts = message.split(":");
        if (parts.size() == 2) {
            int soundLevel = parts[1].toInt();
            if (soundLevel > 10000) {  // Seuil pour détecter une dispute
                ui->alertLabel->setText("Alerte : Dispute détectée !");

                Housekeeper housekeeper;
                housekeeper.incrementDisputeCount(ui->id_securite->text().toInt());
            }
        }
    } else {
        // Affichez uniquement les messages utiles
        qDebug() << "Message reçu (non traité) : " << message;
    }
}



void MainWindow::startMonitoring() {
    if (serial && serial->isOpen()) {
        ui->alertLabel->setText("Surveillance en cours...");
        readSerialData();
    } else {
        QMessageBox::warning(this, "Erreur", "Le port série n'est pas ouvert.");
    }
}
void MainWindow::onSerialDataReceived() {
    QByteArray data = serial->readAll();  // Lire toutes les données disponibles
    buffer += QString::fromUtf8(data);    // Ajouter au buffer

    while (buffer.contains("\n")) {       // Traiter chaque message complet
        int endIndex = buffer.indexOf("\n");
        QString message = buffer.left(endIndex).trimmed();
        processMessage(message);          // Appeler une fonction pour traiter le message
        buffer = buffer.mid(endIndex + 1);
    }
}



void MainWindow::processMessage(const QString &message) {
    if (message.startsWith("DATA:")) {
        QString value = message.mid(5);  // Extraire la valeur après "DATA:"
        int soundLevel = value.toInt();

        if (soundLevel > 500) {          // Seuil pour déclencher une alerte
            ui->alertLabel->setText("Alerte : Dispute détectée !");
            Housekeeper housekeeper;  // Instance de la classe Housekeeper
            housekeeper.incrementDisputeCount(ui->id_securite->text().toInt());
        }
    }
}

void MainWindow::on_startMonitoringButton_clicked() {
    if (ui->id_securite->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer l'ID de sécurité avant de démarrer !");
        return;
    }

    if (serial && serial->isOpen()) {
        ui->alertLabel->setText("Surveillance en cours...");
        // Vous pouvez ajouter ici d'autres initialisations nécessaires.
    } else {
        QMessageBox::warning(this, "Erreur", "Le port série n'est pas ouvert.");
    }
}


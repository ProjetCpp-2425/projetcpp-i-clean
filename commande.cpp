#include "commande.h"


#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QString>
#include <QSqlError>
#include <QPainter>
#include <QPrinter>
#include <QTextDocument>
#include <QMessageBox>
#include <QDateTime>
#include <QImage>
#include <QFileDialog>



commande::commande() {}

commande::commande(int id, QString nom, QString prenom, QString location, int tel, QString email, int nbor)
{
    this->id = id;
    this->nom = nom;
    this->prenom = prenom;
    this->location = location;
    this->tel = tel;
    this->email = email;
    this->nbor = nbor;
}


void commande::setnom(QString n) { this->nom = n; }
void commande::setprenom(QString n) { this->prenom = n; }
void commande::setlocation(QString n) { this->location = n; }
void commande::settel(int n) { this->tel = n; }
void commande::setemail(QString n) { this->email = n; }
void commande::setnbor(int n) { this->nbor = n; }


int commande::get_id() { return id; }
QString commande::get_nom() { return nom; }
QString commande::get_prenom() { return prenom; }
QString commande::get_location() { return location; }
int commande::get_tel() { return tel; }
QString commande::get_email() { return email; }
int commande::get_nbor() { return nbor; }

// Add a new record
bool commande::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO commande (id, nom, prenom, location, tel, email, nbor) "
                  "VALUES (:id, :nom, :prenom, :location, :tel, :email, :nbor)");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":location", location);
    query.bindValue(":tel", tel);
    query.bindValue(":email", email);
    query.bindValue(":nbor", nbor);

    return query.exec();
}

QSqlQueryModel* commande::afficher()
{
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");  // Use the default connection

    if (!db.isOpen()) {
        qDebug() << "Database is not open in afficher().";  // Error message for debugging
        return nullptr;  // Return nullptr if the connection is not open
    }

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query(db);  // Use the default connection

    query.prepare("SELECT * FROM commande");
    if (query.exec()) {
        model->setQuery(query);
        qDebug() << "Data loaded with" << model->rowCount() << "rows.";
    } else {
        qDebug() << "Query execution failed:" << query.lastError().text();  // Log SQL errors
    }

    return model;
}



bool commande::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM commande WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}



bool commande::modifier(int id)
{
    QSqlQuery query;
    query.prepare("UPDATE commande SET nom = :nom, prenom = :prenom, "
                  "location = :location, tel = :tel, email = :email, nbor = :nbor "
                  "WHERE id = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":location", location);
    query.bindValue(":tel", tel);
    query.bindValue(":email", email);
    query.bindValue(":nbor", nbor);

    return query.exec();
}


bool commande::exporterPDF(const QString& fileName, QSqlQueryModel* model)
{
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);

    QPageSize pageSize(QPageSize::A4); // Set A4 size
    QPageLayout pageLayout(pageSize, QPageLayout::Portrait, QMarginsF(20, 20, 20, 20)); // Set layout and margins
    printer.setPageLayout(pageLayout);

    printer.setOutputFileName(fileName);

    QPainter painter(&printer);
    if (!painter.isActive()) {
        QMessageBox::warning(nullptr, "Erreur", "Impossible de dessiner sur l'imprimante.");
        return false;
    }

    QTextDocument doc;

    QString html = "<html><head><style>"
                   "table { border-collapse: collapse; width: 20%; }"
                   "th, td { border: 1px solid #dddddd; text-align: left; padding: 8px; font-size: 50%; }"
                   "th { background-color: #f2f2f2; }"
                   "</style></head><body>"
                   "<table>";
    html += "<tr>";
        for (int col = 0; col < model->columnCount(); ++col) {
            html += "<th>" + model->headerData(col, Qt::Horizontal).toString() + "</th>";
        }
        html += "</tr>";
        for (int row = 0; row < model->rowCount(); ++row) {
            html += "<tr>";
            for (int col = 0; col < model->columnCount(); ++col) {
                if (col == 6) {
                    QDateTime dateTime = model->data(model->index(row, col)).toDateTime();
                    QString formattedDate = dateTime.toString("yyyy-MM-dd");
                    html += "<td>" + formattedDate + "</td>";
                } else {
                    html += "<td>" + model->data(model->index(row, col)).toString() + "</td>";
                }
            }
            html += "</tr>";
        }
        html += "</table></body></html>";
        doc.setHtml(html);
        doc.drawContents(&painter);
        return true;
    }



bool commande::rechercherParId(int id) {
    QSqlQuery query;
    query.prepare("SELECT * FROM commande WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        if (query.next()) {
            // Correct query execution
            return true;
        } else {
            qDebug() << "No record found for ID" << id;
            return false;  // No record found
        }
    } else {
        qDebug() << "Query failed: " << query.lastError();
        return false;  // Query failed
    }
}



QMap<QString, int> commande::getCountByAdresse()
{
    QMap<QString, int> addressCounts;
    QSqlQuery query;

    // Query to count fournisseurs per adresse
    query.prepare("SELECT ADRESSE_F, COUNT(*) as count FROM fournisseur GROUP BY ADRESSE_F");

    if (query.exec()) {
        while (query.next()) {
            QString adresse = query.value(0).toString();
            int count = query.value(1).toInt();
            addressCounts[adresse] = count;
        }
    } else {
        qDebug() << "Error fetching statistics: " << query.lastError().text();
    }

    return addressCounts;
}




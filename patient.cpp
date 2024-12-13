#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QRegularExpression>
#include <QSqlQueryModel>
#include <QPdfWriter>
#include <QPainter>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QRegularExpression>
#include <QSqlQueryModel>
#include <QPdfWriter>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include<QPageSize>
#include <QtCharts/QPieSeries>



#include "patient.h"



// Constructor
patient::patient(int AGE, int NUM_SOCIALE, int TELEPHONE, QString NOM, QString PRENOM, QString GENRE, QString ADRESSE, QString E_MAIL)
{
    this->AGE = AGE;
    this->NUM_SOCIALE = NUM_SOCIALE;
    this->TELEPHONE = TELEPHONE;
    this->NOM = NOM;
    this->PRENOM = PRENOM;
    this->GENRE = GENRE;
    this->ADRESSE = ADRESSE;
    this->E_MAIL = E_MAIL;
}

// Method to add a patient
/*bool patient::ajout()
{
      qDebug() << "patient::ajout() called";  // Message de débogage
      if (rechercher(NUM_SOCIALE)) {
              qDebug() << "Le patient avec ce numéro social existe déjà.";
              return false;
          }
    QSqlQuery query;
    QString NUMfstr = QString::number(NUM_SOCIALE);
    QString AGEfstr = QString::number(AGE);
    QString TELfstr = QString::number(TELEPHONE);
    query.prepare("INSERT INTO PATIENT (NOM, PRENOM, AGE, GENRE, NUM_SOCIALE, TELEPHONE, ADRESSE, E_MAIL) "
                  "VALUES (:NOM, :PRENOM, :AGE, :GENRE, :NUM_SOCIALE, :TELEPHONE, :ADRESSE, :E_MAIL)");

    query.bindValue(":NOM", NOM);
    query.bindValue(":PRENOM", PRENOM);
    query.bindValue(":AGE", AGEfstr);
    query.bindValue(":GENRE", GENRE);
    query.bindValue(":NUM_SOCIALE", NUMfstr);
    query.bindValue(":TELEPHONE", TELfstr);
    query.bindValue(":ADRESSE", ADRESSE);
    query.bindValue(":E_MAIL", E_MAIL);*/

   /* if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du patient : " << query.lastError().text();
        return false;
    }

    return true;*/
     // return query.exec();

//}
bool patient::ajout() {
    qDebug() << "patient::ajout() appelé";  // Message de débogage

    // Vérification si le patient existe déjà
    if (rechercher(NUM_SOCIALE)) {
        qDebug() << "Le patient avec ce numéro social existe déjà.";
        return false;
    }

    QSqlQuery query;
    QString NUMfstr = QString::number(NUM_SOCIALE);
    QString AGEfstr = QString::number(AGE);
    QString TELfstr = QString::number(TELEPHONE);

    // Préparation de la requête SQL (retirer le NULL à la fin)
    query.prepare("INSERT INTO PATIENT (NOM, PRENOM, AGE, GENRE, NUM_SOCIALE, TELEPHONE, ADRESSE, E_MAIL) "
                  "VALUES (:NOM, :PRENOM, :AGE, :GENRE, :NUM_SOCIALE, :TELEPHONE, :ADRESSE, :E_MAIL)");

    query.bindValue(":NOM", NOM);
    query.bindValue(":PRENOM", PRENOM);
    query.bindValue(":AGE", AGEfstr);
    query.bindValue(":GENRE", GENRE);
    query.bindValue(":NUM_SOCIALE", NUMfstr);
    query.bindValue(":TELEPHONE", TELfstr);
    query.bindValue(":ADRESSE", ADRESSE);
    query.bindValue(":E_MAIL", E_MAIL);

    // Exécution de la requête et gestion d'erreur
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du patient : " << query.lastError().text();
        return false;
    }

    return true;
}




// Method to display patients
QSqlQueryModel* patient::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT NOM, PRENOM, AGE, GENRE, NUM_SOCIALE, TELEPHONE, ADRESSE, E_MAIL FROM PATIENT");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Âge"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("GENRE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Numéro Social"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("ADRESSE"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("E-mail"));

    return model;
}

bool patient::supprimer(int numSociale)
{
    QSqlQuery query;
    QString res = QString::number(numSociale);
    query.prepare("DELETE FROM PATIENT WHERE NUM_SOCIALE = :numSociale");
    query.bindValue(":numSociale", res);
    return query.exec();
}
bool patient::rechercher(int NUM_SOCIALE)
{
    QSqlQuery query;
    QString res = QString::number(NUM_SOCIALE);
    query.prepare("SELECT * FROM PATIENT WHERE NUM_SOCIALE = :numSociale");
    query.bindValue(":numSociale", res);

    if (query.exec()) {
        return query.next();  // Vérifie s'il y a au moins un résultat
    }
    return false;
}
bool patient::update(int NUM_SOCIALE, int AGE, int TELEPHONE, QString NOM, QString PRENOM, QString GENRE, QString ADRESSE, QString E_MAIL)
{
    QSqlQuery query;
    QString NUM_SOCIALEStr = QString::number(NUM_SOCIALE);
    QString AGEStr = QString::number(AGE);
    QString TELEPHONEStr = QString::number(TELEPHONE);

    query.prepare("UPDATE PATIENT SET NOM=:nom, PRENOM=:prenom, AGE=:age, GENRE=:genre, TELEPHONE=:telephone, ADRESSE=:adresse, E_MAIL=:email WHERE NUM_SOCIALE=:numSociale");

    query.bindValue(":numSociale", NUM_SOCIALEStr);
    query.bindValue(":nom", NOM);
    query.bindValue(":prenom", PRENOM);
    query.bindValue(":age", AGEStr);
    query.bindValue(":genre", GENRE);
    query.bindValue(":telephone", TELEPHONEStr);
    query.bindValue(":adresse", ADRESSE);
    query.bindValue(":email", E_MAIL);

    return query.exec();
}

 void patient::genererPDF() {
   // QPdfWriter pdf("C:/Users/Admin/Downloads/patient.pdf");
     QString filePath = QFileDialog::getSaveFileName(nullptr, "Enregistrer le profil du patient", "", "PDF Files (*.pdf);;All Files (*)");
     if (filePath.isEmpty())
         return;

     QPdfWriter pdfWriter(filePath);
    QPainter painter(&pdfWriter);

    // Set the page size and orientation
    pdfWriter.setPageSize(QPageSize(QPageSize::A3));
    pdfWriter.setPageOrientation(QPageLayout::Portrait);

    // Set background color
    painter.fillRect(QRect(0, 0, pdfWriter.width(), pdfWriter.height()), QColor("light green"));

    // Title font and text
    QFont font("patient", 30, QFont::Bold);
    painter.setFont(font);
    painter.drawText(1000, 1000, "Profil du patient");  // Adjusted position for title

    // Content font
    QFont contentFont("patient", 14);
    painter.setFont(contentFont);

    // Database query
    QSqlQuery query;
    query.prepare("SELECT * FROM patient");
    query.exec();

    // Define initial vertical position, line height, and table cell width
    int x = 100;           // X-position for table
    int y = 1900;          // Y-position for table, starting after the title
    int rowHeight = 600;   // Height of each row
    int columnWidth = 1200; // Width of each column

    // Draw table header
    painter.setFont(QFont("patient", 8, QFont::Bold));
    painter.drawText(x, y, "NOM");
    painter.drawText(x + columnWidth, y, "PRENOM");
    painter.drawText(x + 2 * columnWidth, y, "AGE");
    painter.drawText(x + 3 * columnWidth, y, "GENRE");
    painter.drawText(x + 4 * columnWidth, y, "NUM_SOCIALE");
    painter.drawText(x + 5 * columnWidth, y, "TELEPHONE");
    painter.drawText(x + 6 * columnWidth, y, "ADRESSE");
    painter.drawText(x + 7 * columnWidth, y, "E_mail");

    // Draw line after header
    painter.drawLine(x, y + 20, x + 8 * columnWidth, y + 20);

    y += rowHeight;  // Move to the next row

    // Reset font for content
    painter.setFont(QFont("patient", 8));

    // Loop to handle multiple rows
    while (query.next()) {
        // Draw each column's data
        painter.drawText(x, y, query.value(0).toString());
        painter.drawText(x + columnWidth, y, query.value(1).toString());
        painter.drawText(x + 2 * columnWidth, y, query.value(2).toString());
        painter.drawText(x + 3 * columnWidth, y, query.value(3).toString());
        painter.drawText(x + 4 * columnWidth, y, query.value(4).toString());
        painter.drawText(x + 5 * columnWidth, y, query.value(5).toString());
        painter.drawText(x + 6 * columnWidth, y, query.value(6).toString());
        painter.drawText(x + 7 * columnWidth, y, query.value(7).toString());


        // Draw borders for the current row
        painter.drawLine(x, y - rowHeight + 20, x + 8 * columnWidth, y - rowHeight + 20); // Top border
        painter.drawLine(x, y + 20, x + 8 * columnWidth, y + 20); // Bottom border

        // Draw vertical lines between columns
        for (int i = 1; i <= 8; ++i) {
            painter.drawLine(x + i * columnWidth, y - rowHeight + 20, x + i * columnWidth, y + 20);
        }

        y += rowHeight; // Move to the next row for the next record
    }

    // Load and scale up the logo
   /* QPixmap logo("C:/Users/amine/Downloads/logo2.png");
    double scaleFactor = 2.0;  // Double the logo size
    int newWidth = static_cast<int>(logo.width() * scaleFactor);
    int newHeight = static_cast<int>(logo.height() * scaleFactor);

    // Draw the scaled logo in the top-right corner
    painter.drawPixmap(pdf.width() - newWidth - 10, 10, newWidth, newHeight, logo);*/

    // End the painter
    painter.end();


}

QSqlQueryModel *patient::trier_age_asc()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PATIENT ORDER BY AGE ASC");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Âge"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("GENRE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Numéro Social"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("ADRESSE"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("E-mail"));

    return model;
}
QSqlQueryModel *patient::trier_age_desc()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PATIENT ORDER BY AGE DESC");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Âge"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("GENRE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Numéro Social"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("ADRESSE"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("E-mail"));

    return model;
}
QPieSeries* patient::generatePatientStatistics() {
    // Créer une série pour le diagramme circulaire
    QPieSeries *series = new QPieSeries();

    // Requêtes SQL pour récupérer les statistiques
    QSqlQuery query;
    int fluCases = 0;     // Cas de grippe
    int otherDiseases = 0; // Autres maladies

    // Récupérer le nombre de cas de grippe
    if (query.exec("SELECT COUNT(*) FROM PATIENT WHERE AGE = 20")) {
        if (query.next()) {
            fluCases = query.value(0).toInt();
        }
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête (grippe):" << query.lastError().text();
        return series; // Retourner une série vide en cas d'erreur
    }

    // Récupérer le nombre de patients avec d'autres diagnostics
    if (query.exec("SELECT COUNT(*) FROM PATIENT WHERE AGE != 20")) {
        if (query.next()) {
            otherDiseases = query.value(0).toInt();
        }
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête (autres):" << query.lastError().text();
        return series; // Retourner une série vide en cas d'erreur
    }

    // Vérifier s'il y a des données à afficher
    int totalPatients = fluCases + otherDiseases;
    if (totalPatients == 0) {
        qDebug() << "Aucun patient trouvé.";
        return series; // Retourner une série vide si aucun patient n'existe
    }

    // Ajouter les données au graphique
    series->append("AGE 20" , fluCases);
    series->append("Autres", otherDiseases);

    // Style des tranches
    QPieSlice *fluSlice = series->slices().at(0);
    fluSlice->setBrush(QColor("#4CAF50")); // Vert pour la grippe
    fluSlice->setLabel(QString("AGE (%1%)")
                           .arg((fluCases * 100.0) / totalPatients, 0, 'f', 1));

    QPieSlice *otherSlice = series->slices().at(1);
    otherSlice->setBrush(QColor("#FF5722")); // Rouge-orange pour les autres maladies
    otherSlice->setLabel(QString("Autres (%1%)")
                             .arg((otherDiseases * 100.0) / totalPatients, 0, 'f', 1));

    // Activer les étiquettes
    series->setLabelsVisible(true);

    return series;
}
QSqlQueryModel* patient::rechercherParNumSocial(int numSocial)
{
    QSqlQueryModel *model = new QSqlQueryModel();

    // Préparer la requête SQL pour rechercher un patient par numéro social
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, AGE, GENRE, NUM_SOCIALE, TELEPHONE, ADRESSE, E_MAIL FROM PATIENT WHERE NUM_SOCIALE = :numSocial");
    query.bindValue(":numSocial", QString::number(numSocial));

    // Exécuter la requête
    if (query.exec()) {
        model->setQuery(query);  // Associer les résultats de la requête au modèle
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Nom"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Prénom"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Âge"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Genre"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Numéro Social"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Adresse"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("E-mail"));
    } else {
        qDebug() << "Erreur lors de la recherche du patient : " << query.lastError().text();
    }

    return model;
}

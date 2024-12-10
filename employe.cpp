#include "employe.h"
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
#include <QPdfWriter>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlRecord>

employe::employe(int CIN,QString NOM_ET_PRENOM,QString ADRESSE,int TELEPHONE,QString POSTE,int SALAIRE,int DATE_EMBAUCHE,int NUM_CONTRAT)
{
    this->CIN=CIN;
    this->NOM_ET_PRENOM=NOM_ET_PRENOM;
    this->ADRESSE_EMP=ADRESSE;
    this->TELEPHONE_EMP=TELEPHONE;
    this->POSTE=POSTE;
    this->SALAIRE=SALAIRE;
    this->DATE_EMBAUCHE=DATE_EMBAUCHE;
    this->NUM_CONTRAT=NUM_CONTRAT;


}

bool employe::ajouter()
{


    QSqlQuery query;
    QString cin =QString::number(CIN);
    QString nom =QString(NOM_ET_PRENOM);
    QString ad =QString(ADRESSE_EMP);
    QString tel =QString::number(TELEPHONE_EMP);
    QString po =QString(POSTE);
    QString sal =QString::number(SALAIRE);
    QString de =QString::number(DATE_EMBAUCHE);
    QString nc =QString::number(NUM_CONTRAT);

    query.prepare("INSERT INTO EMPLOYE(CIN, NOM_ET_PRENOM, ADRESSE_EMP, TELEPHONE_EMP, Poste, Salaire, Date_Embauche, Num_Contrat) VALUES (:CIN, :NomEtPrenom, :Adresse, :Telephone, :Poste, :Salaire, :DateEmbauche, :NumContrat)");

    query.bindValue(":CIN",CIN);
    query.bindValue(":NomEtPrenom",NOM_ET_PRENOM);
    query.bindValue(":Adresse",ADRESSE_EMP);
    query.bindValue(":Telephone",tel);
    query.bindValue(":Poste",POSTE);
    query.bindValue(":Salaire",sal);
    query.bindValue(":DateEmbauche",de);
    query.bindValue(":NumContrat",nc);


    return query.exec();//exec envoie la requete pour execution

}


QSqlQueryModel* employe::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    // S'assurer que la requête est correcte et que la table des employés contient bien les données mises à jour
    model->setQuery("SELECT * FROM employe");

    // Définir les en-têtes de colonnes si nécessaire
    model->setHeaderData(0, Qt::Horizontal, "CIN");
    model->setHeaderData(1, Qt::Horizontal, "NomEtPrenom");
    model->setHeaderData(2, Qt::Horizontal, "Adresse");
    model->setHeaderData(3, Qt::Horizontal, "TÉLÉPHONE");
    model->setHeaderData(4, Qt::Horizontal, "Poste");
    model->setHeaderData(5, Qt::Horizontal, "Salaire");
    model->setHeaderData(6, Qt::Horizontal, "DateEmbauche");
    model->setHeaderData(7, Qt::Horizontal, "NumContrat");

    return model;
}


bool employe::supprimer(int CIN)
{
    QSqlQuery query;
    query.prepare("DELETE FROM employe WHERE CIN = :cin"); // Assurez-vous que la table et la colonne sont correctes
    query.bindValue(":cin", CIN);

    // Exécuter la requête et vérifier si elle réussit
    if (query.exec()) {
        qDebug() << "Suppression réussie pour CIN:" << CIN;
        return true;
    } else {
        qDebug() << "Erreur lors de la suppression:" << query.lastError().text();
        return false;
    }
}

bool employe::modifier() {
    QSqlQuery query;

    query.prepare("UPDATE EMPLOYE SET NOM_ET_PRENOM = :nom, ADRESSE = :adresse, TELEPHONE = :telephone, "
                  "POSTE = :poste, SALAIRE = :salaire, DATE_EMBAUCHE = :dateEmbauche, NUM_CONTRAT = :numContrat "
                  "WHERE CIN = :cin");

    query.bindValue(":cin", CIN);
    query.bindValue(":nom", NOM_ET_PRENOM);
    query.bindValue(":adresse", ADRESSE_EMP);
    query.bindValue(":telephone", TELEPHONE_EMP);
    query.bindValue(":poste", POSTE);
    query.bindValue(":salaire", SALAIRE);
    query.bindValue(":dateEmbauche", DATE_EMBAUCHE);
    query.bindValue(":numContrat", NUM_CONTRAT);

    if (query.exec()) {
        qDebug() << "Modification réussie pour l'employé CIN =" << CIN;
        return true;
    } else {
        qDebug() << "Erreur lors de la modification de l'employé:" << query.lastError().text();
        return false;
    }
}


void employe::exporterPDF(const QString &filePath) {
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setTitle("Liste des employés");

    QPainter painter(&pdfWriter);
    int yPosition = 100;

    // Titre du document PDF
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(200, yPosition, "Liste des Employés");
    yPosition += 50;

    // Créer la table de données à partir de la base de données
    QSqlQuery query("SELECT CIN, NOM_ET_PRENOM, ADRESSE, TELEPHONE, POSTE, SALAIRE, DATE_EMBAUCHE, NUM_CONTRAT FROM EMPLOYE");
    if (query.exec()) {
        painter.setFont(QFont("Arial", 10));

        // En-tête des colonnes
        painter.drawText(50, yPosition, "CIN");
        painter.drawText(150, yPosition, "Nom et Prénom");
        painter.drawText(300, yPosition, "Adresse");
        painter.drawText(450, yPosition, "Téléphone");
        painter.drawText(550, yPosition, "Poste");
        painter.drawText(650, yPosition, "Salaire");
        painter.drawText(750, yPosition, "Date Embauche");
        painter.drawText(850, yPosition, "Num Contrat");
        yPosition += 25;

        // Remplissage des données
        while (query.next()) {
            painter.drawText(50, yPosition, query.value("CIN").toString());
            painter.drawText(150, yPosition, query.value("NOM_ET_PRENOM").toString());
            painter.drawText(300, yPosition, query.value("ADRESSE").toString());
            painter.drawText(450, yPosition, query.value("TELEPHONE").toString());
            painter.drawText(550, yPosition, query.value("POSTE").toString());
            painter.drawText(650, yPosition, query.value("SALAIRE").toString());
            painter.drawText(750, yPosition, query.value("DATE_EMBAUCHE").toString());
            painter.drawText(850, yPosition, query.value("NUM_CONTRAT").toString());
            yPosition += 20;

            if (yPosition > pdfWriter.height() - 50) {
                pdfWriter.newPage();
                yPosition = 50;
            }
        }
    } else {
        qWarning() << "Erreur lors de l'exécution de la requête:" << query.lastError().text();
    }

    painter.end();
}


QSqlQueryModel* employe::rechercher(int CIN)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM employe WHERE CIN = :cin");
    query.bindValue(":cin", CIN);  // Lier le CIN passé en paramètre à la requête

    if (query.exec()) {

        model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom et Prénom"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Adresse"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Téléphone"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Poste"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Salaire"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date d'embauche"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("Numéro de contrat"));
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête : " << query.lastError();
    }

    return model;
}


QMap<QString, int> employe::statistiquesParPoste() {
    QMap<QString, int> statistiques;

    QSqlQuery query;
    query.prepare("SELECT poste, COUNT(*) FROM employe GROUP BY poste");

    if (query.exec()) {
        while (query.next()) {
            QString poste = query.value(0).toString();
            int count = query.value(1).toInt();
            statistiques[poste] = count;  // Ajouter le poste et le nombre d'employés
        }
    } else {
        qDebug() << "Erreur lors de la requête des statistiques par poste : " << query.lastError();
    }

    return statistiques;
}
QSqlQueryModel* employe::afficherTriParNom() {
    QSqlQueryModel* model = new QSqlQueryModel();

    // Créer une requête pour trier les employés par nom
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYE ORDER BY NOM_ET_PRENOM ASC");  // Tri par NOM_ET_PRENOM

    // Exécuter la requête
    if (query.exec()) {
        model->setQuery(query);
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête de tri par nom : " << query.lastError();
    }

    return model;
}
QSqlQueryModel* employe::afficherTriParDateEmbauche() {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    // Requête SQL pour trier les employés par date d'embauche (assurez-vous que la colonne existe)
    query.prepare("SELECT * FROM employe ORDER BY date_embauche ASC"); // Ou DESC pour un tri décroissant

    if (query.exec()) {
        model->setQuery(query);
    } else {
        qDebug() << "Erreur lors du tri par date d'embauche : " << query.lastError();
    }

    return model;
}

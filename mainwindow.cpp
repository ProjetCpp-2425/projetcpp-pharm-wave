#include "mainwindow.h"
#include "employe.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
#include<QTabWidget>
#include <QRegularExpression>
#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

void MainWindow::on_ajouter_2_clicked()
{
    int CIN = ui->cin->text().toInt();
    QString NOM_ET_PRENOM = ui->nom->text();
    QString ADRESSE = ui->ad->text();
    int TELEPHONE = ui->te->text().toInt();
    QString POSTE = ui->po->text();
    int SALAIRE = ui->sal->text().toInt();
    int DATE_EMBAUCHE = ui->de->text().toInt();
    int NUM_CONTRAT = ui->nc->text().toInt();

    // Contrôles de saisie
    if (CIN <= 0 || QString::number(CIN).length() != 8) {
        QMessageBox::warning(this, "Erreur de saisie", "Le CIN doit être un nombre positif à 8 chiffres.");
        return;
    }
    if (NOM_ET_PRENOM.isEmpty() || !NOM_ET_PRENOM.contains(QRegularExpression("^[A-Za-z ]+$"))) {
        QMessageBox::warning(this, "Erreur de saisie", "Le nom et le prénom doivent être composés uniquement de lettres.");
        return;
    }

    if (ADRESSE.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'adresse ne doit pas être vide.");
        return;
    }
    if (TELEPHONE <= 0 || QString::number(TELEPHONE).length() != 8) {
        QMessageBox::warning(this, "Erreur de saisie", "Le numéro de téléphone doit être un nombre positif à 8 chiffres.");
        return;
    }
    if (POSTE.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le poste ne doit pas être vide.");
        return;
    }
    if (SALAIRE <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Le salaire doit être un nombre positif.");
        return;
    }
    if (DATE_EMBAUCHE < 1900 || DATE_EMBAUCHE > 2100) {
        QMessageBox::warning(this, "Erreur de saisie", "La date d'embauche doit être comprise entre 1900 et 2100.");
        return;
    }
    if (NUM_CONTRAT <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Le numéro de contrat doit être un nombre positif.");
        return;
    }

    // Création de l'objet employe avec les valeurs validées
    employe emp(CIN, NOM_ET_PRENOM, ADRESSE, TELEPHONE, POSTE, SALAIRE, DATE_EMBAUCHE, NUM_CONTRAT);
    bool test = emp.ajouter();
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("ok"), QObject::tr("Ajout effectué\n"
                                                                       "click cancel to exit."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("not ok"), QObject::tr("Ajout non effectué\n"
                                                                       "click cancel to exit."), QMessageBox::Cancel);
    }

    ui->afficher->setModel(emp.afficher());
}


MainWindow::~MainWindow()
{
    delete ui;
}







void MainWindow::on_supprimer_clicked()
{
    int CIN = ui->cin->text().toInt();

    // Créer une instance de la classe employe pour accéder aux méthodes
    employe E;

    // Appeler la fonction de suppression avec le CIN
    bool test = E.supprimer(CIN);

    // Vérifier si la suppression a réussi
    if (test) {
        QMessageBox::information(this, "Succès", "Suppression effectuée");

        // Mettre à jour le modèle de données pour recharger l'affichage
        QSqlQueryModel *model = E.afficher();  // Recharger les données
        ui->afficher->setModel(model);         // Réassigner le modèle au QTableView

        // Optionnel : Vous pouvez également actualiser d'autres vues ou éléments si nécessaire
        ui->cin->clear(); // Effacer les champs, par exemple
    } else {
        QMessageBox::critical(this, "Erreur", "Suppression non effectuée");
    }
    ui->afficher->setModel(E.afficher());
}




void MainWindow::on_modifier_2_clicked() {
    // Récupérer les valeurs des champs de l'interface
    int CIN = ui->cin_2->text().toInt();
    QString NOM_ET_PRENOM = ui->nom_2->text();
    QString ADRESSE = ui->ad_2->text();
    int TELEPHONE = ui->te_2->text().toInt();
    QString POSTE = ui->po_2->text();
    int SALAIRE = ui->sal_2->text().toInt();
    int DATE_EMBAUCHE = ui->de_2->text().toInt();
    int NUM_CONTRAT = ui->nc_2->text().toInt();

    // Créer la requête SQL pour mettre à jour l'employé
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYE SET NOM_ET_PRENOM = :NOM_ET_PRENOM, ADRESSE = :ADRESSE, "
                  "TELEPHONE = :TELEPHONE, POSTE = :POSTE, SALAIRE = :SALAIRE, "
                  "DATE_EMBAUCHE = :DATE_EMBAUCHE, NUM_CONTRAT = :NUM_CONTRAT WHERE CIN = :CIN");

    // Associer les valeurs aux paramètres de la requête
    query.bindValue(":NOM_ET_PRENOM", NOM_ET_PRENOM);
    query.bindValue(":ADRESSE", ADRESSE);
    query.bindValue(":TELEPHONE", TELEPHONE);
    query.bindValue(":POSTE", POSTE);
    query.bindValue(":SALAIRE", SALAIRE);
    query.bindValue(":DATE_EMBAUCHE", DATE_EMBAUCHE);
    query.bindValue(":NUM_CONTRAT", NUM_CONTRAT);
    query.bindValue(":CIN", CIN);

    // Exécuter la requête et vérifier si la mise à jour a réussi
    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Les informations de l'employé ont été mises à jour avec succès.");
    } else {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la mise à jour de l'employé.");
    }
}








void MainWindow::on_pdf_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", "", "Fichiers PDF (*.pdf)");
        if (!filePath.isEmpty()) {
            employe emp;
            emp.exporterPDF(filePath);

            QMessageBox::information(this, "Exportation PDF", "La liste des employés a été exportée avec succès en PDF !");
        }
}

void MainWindow::on_tabWidget_currentChanged(int index) {
    // Vérifier si l'onglet actuellement sélectionné est celui où vous souhaitez afficher les employés
    if (index == 1) {  // Par exemple, l'onglet à l'index 1 est celui où vous voulez afficher les employés
        employe emp;
        QSqlQueryModel *model = emp.afficher();

        // Associer le modèle de données au QTableView
        ui->afficher->setModel(model);
    }
}

void MainWindow::on_rechercher_clicked()
{
    int CIN = ui->recherche->text().toInt();  // Récupérer le CIN saisi par l'utilisateur

        employe E;
        QSqlQueryModel *model = E.rechercher(CIN);  // Appeler la méthode rechercher pour récupérer les données

        if (model->rowCount() > 0) {
            ui->afficher->setModel(model);  // Afficher les résultats dans un QTableView
        } else {
            QMessageBox::information(this, "Aucun résultat", "Aucun employé trouvé avec ce CIN.");
        }
}


void MainWindow::on_stat_clicked()
{
    employe E;
        QMap<QString, int> statistiques = E.statistiquesParPoste();  // Récupérer les statistiques par poste

        // Calculer le total des employés
        int totalEmployes = 0;
        for (auto it = statistiques.constBegin(); it != statistiques.constEnd(); ++it) {
            totalEmployes += it.value();
        }

        // Vider le layout avant d'ajouter un nouveau graphique
        QLayoutItem* item;
        while ((item = ui->verticalLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        // Créer un pie series pour le graphique
        QPieSeries *series = new QPieSeries();

        // Ajouter les données du nombre d'employés par poste dans le graphique avec le pourcentage
        for (auto it = statistiques.constBegin(); it != statistiques.constEnd(); ++it) {
            QString poste = it.key();
            int count = it.value();
            double percentage = (totalEmployes > 0) ? (count * 100.0 / totalEmployes) : 0.0;

            // Ajouter l'élément avec le pourcentage
            QString label = QString("%1: %2%").arg(poste).arg(percentage, 0, 'f', 1);
            series->append(label, count);
        }

        // Créer un graphique à partir de la série
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Statistiques des employés par poste");

        // Créer un chart view pour afficher le graphique
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);  // Améliorer l'anti-aliasing

        // Afficher le graphique dans un layout
        ui->verticalLayout->addWidget(chartView); // Assurez-vous que ce layout existe dans votre UI
}



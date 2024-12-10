#include "mainwindow.h"
#include "employe.h"
#include "arduino.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
#include<QTabWidget>
#include <QRegularExpression>
#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QSqlError>
#include <QPdfWriter>
#include <QDate>
#include <QTextCharFormat>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    arduino(new Arduino(this))
{
    ui->setupUi(this);
    connect(&arduino, &Arduino::dataReceived, this, &MainWindow::readFromArduino);
    // Cacher les onglets sauf celui de la page Connexion
        /*for (int i = 0; i < ui->tabWidget->count(); ++i)
        {
            if (ui->widget->Text(i) != "widget_2") { // Remplacez "Connexion" par le nom de l'onglet de connexion
                ui->tabWidget->removeTab(i);
                --i; // Ajuster l'index après suppression
            }
        }*/
        // Initialiser les éléments de l'interface
            cardIDLabel = ui->cardIDLabel;  // Associe un QLabel dans ton fichier .ui

            // Connecter les boutons aux fonctions correspondantes
           // connect(ui->connectButton, &QPushButton::clicked, this, &::MainWindow::on_connectButton_clicked);


        /*// Initialiser le QCalendarWidget
        Calendrier = new QCalendarWidget(this);
        Calendrier->setGeometry(10, 10, 400, 300);  // Positionner et redimensionner le calendrier
        ui->verticalLayout->addWidget(Calendrier);  // Ajouter à un layout si vous utilisez Qt Designer*/



}

void MainWindow::on_ajouter_2_clicked()
{
    int CIN = ui->cin->text().toInt();
    QString NOM_ET_PRENOM = ui->nom->text();
    QString ADRESSE_EMP = ui->ad->text();
    int TELEPHONE_EMP = ui->te->text().toInt();
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

    if (ADRESSE_EMP.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'adresse ne doit pas être vide.");
        return;
    }
    if (TELEPHONE_EMP <= 0 || QString::number(TELEPHONE_EMP).length() != 8) {
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
    employe emp(CIN, NOM_ET_PRENOM, ADRESSE_EMP, TELEPHONE_EMP, POSTE, SALAIRE, DATE_EMBAUCHE, NUM_CONTRAT);
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
    //delete Arduino;
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
    QString ADRESSE_EMP = ui->ad_2->text();
    int TELEPHONE_EMP = ui->te_2->text().toInt();
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
    query.bindValue(":ADRESSE", ADRESSE_EMP);
    query.bindValue(":TELEPHONE", TELEPHONE_EMP);
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
    // Vérifiez si le modèle de la table "afficher" est valide
    QAbstractItemModel *model = ui->afficher->model();
    if (!model) {
        QMessageBox::warning(this, "Erreur", "Aucune donnée disponible dans la table pour générer le PDF.");
        return;
    }

    // Demander le chemin du fichier PDF
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", "", "Fichiers PDF (*.pdf)");
    if (filePath.isEmpty()) {
        return; // L'utilisateur a annulé
    }

    // Initialisation du PDF
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);

    // Définir les marges et dimensions
    int x = 50; // Marge gauche
    int y = 50; // Marge haute
    int rowHeight = 20; // Hauteur d'une ligne
    int colWidth = 100; // Largeur des colonnes

    // Dessiner le titre
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(x, y, "Liste des Employés");
    y += 50;

    // Dessiner les en-têtes de colonnes
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    for (int col = 0; col < model->columnCount(); ++col) {
        painter.drawText(x + col * colWidth, y, model->headerData(col, Qt::Horizontal).toString());
    }
    y += rowHeight;

    // Dessiner les données des lignes
    painter.setFont(QFont("Arial", 10));
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QModelIndex index = model->index(row, col);
            QString data = model->data(index).toString();
            painter.drawText(x + col * colWidth, y, data);
        }
        y += rowHeight;

        // Gérer le saut de page si nécessaire
        if (y > pdfWriter.pageLayout().pageSize().rectPixels(pdfWriter.resolution()).height() - 50) {
            pdfWriter.newPage();
            y = 50;
        }
    }

    painter.end();

    QMessageBox::information(this, "Succès", "Le fichier PDF a été généré avec succès !");
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
    QString recherche = ui->recherche->text().trimmed();

    // Vérifiez que le champ de recherche n'est pas vide
    if (recherche.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir une valeur pour effectuer la recherche.");
        return;
    }

    // Préparer la requête SQL avec plusieurs critères
    QSqlQuery query;
    query.prepare("SELECT * FROM employe WHERE "
                  "CIN = :cin OR "
                  "NOM_ET_PRENOM LIKE :nom OR "
                  "NUM_CONTRAT = :num_contrat");

    // Bind des valeurs pour CIN, NOM_ET_PRENOM, et NUM_CONTRAT
    query.bindValue(":cin", recherche.toInt()); // CIN (converti en entier si possible)
    query.bindValue(":nom", "%" + recherche + "%"); // Recherche partielle sur le nom
    query.bindValue(":num_contrat", recherche.toInt()); // Numéro de contrat (converti en entier)

    // Exécuter la requête
    if (query.exec()) {
        QSqlQueryModel *model = new QSqlQueryModel(this);

        // Charger le résultat dans le modèle
        model->setQuery(std::move(query));  // Utiliser std::move pour éviter la copie de QSqlQuery


        if (model->rowCount() > 0) {
            // Connecter le modèle au QTableView nommé 'afficher'
            ui->afficher->setModel(model);

            // Optionnel : personnaliser les en-têtes
            model->setHeaderData(0, Qt::Horizontal, "CIN");
            model->setHeaderData(1, Qt::Horizontal, "Nom et Prénom");
            model->setHeaderData(2, Qt::Horizontal, "Adresse");
            model->setHeaderData(3, Qt::Horizontal, "Téléphone");
            model->setHeaderData(4, Qt::Horizontal, "Poste");
            model->setHeaderData(5, Qt::Horizontal, "Salaire");
            model->setHeaderData(6, Qt::Horizontal, "Date d'Embauche");
            model->setHeaderData(7, Qt::Horizontal, "Numéro Contrat");

            QMessageBox::information(this, "Recherche réussie", "Résultats affichés dans la table.");
        } else {
            QMessageBox::warning(this, "Recherche", "Aucun résultat trouvé.");
        }
    } else {
        // Afficher l'erreur SQL si la requête échoue
        qDebug() << "Erreur SQL : " << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Une erreur est survenue lors de l'exécution de la recherche.");
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

void MainWindow::on_connect_clicked()
{
    QString CIN = ui->id->text().trimmed();
    QString MDP = ui->mo->text().trimmed();

    if (CIN.isEmpty() || MDP.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM employe WHERE CIN = :cin AND MDP = :mdp");
    query.bindValue(":cin", CIN.toInt());
    query.bindValue(":mdp", MDP);

    if (query.exec()) {
        if (query.next()) {
            QMessageBox::information(this, "Connexion réussie", "Bienvenue, " + CIN + " !");

            // Cacher la partie connexion
            ui->widget_6->setVisible(false);

            // Afficher la partie principale avec les onglets
            ui->tabWidget->setVisible(true);

            // Restaurez les onglets cachés
            ui->tabWidget->addTab(ui->ListeDesEmployes, "Liste des Employés");
            ui->tabWidget->addTab(ui->AjouterEmploye, "Ajouter Employé");
            ui->tabWidget->addTab(ui->ModifierEmploye, "Modifier Employé");
            ui->tabWidget->addTab(ui->Statistique, "Statistiques");
            ui->tabWidget->addTab(ui->Tri, "Tri");
            ui->tabWidget->addTab(ui->Calendrier, "Calendrier");
            ui->tabWidget->addTab(ui->cardID, "cardID");

            // Passer à la page "Liste des Employés"
            ui->tabWidget->setCurrentWidget(ui->ListeDesEmployes);

        } else {
            QMessageBox::warning(this, "Erreur", "Nom d'utilisateur ou mot de passe incorrect.");
        }
    } else {
        qDebug() << "Erreur SQL : " << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Une erreur s'est produite lors de l'authentification.");
    }
}










void MainWindow::on_tri_2_clicked()
{

        employe e;
        QSqlQueryModel* model = e.afficherTriParDateEmbauche();

        // Vérifiez si le modèle a bien été créé
        if (model && model->rowCount() > 0) {
            // Remplir le QTableView avec les employés triés par date d'embauche
            ui->trie->setModel(model);
            QMessageBox::information(this, tr("Tri par Date d'Embauche"), tr("Les employés ont été triés par date d'embauche."), QMessageBox::Ok);
        } else {
            // Si le modèle est vide ou l'erreur s'est produite, afficher un message d'erreur
            QMessageBox::critical(this, tr("Erreur"), tr("Impossible de trier les employés ou aucun employé trouvé."));
        }


}

void MainWindow::on_tri_clicked()
{
    employe E;
    QSqlQueryModel* model = E.afficherTriParNom();

    // Vérifiez si le modèle a bien été créé
    if (model && model->rowCount() > 0) {
        // Remplir le QTableView avec les employés triés par nom
        ui->trie->setModel(model);
        QMessageBox::information(this, tr("Tri par Nom"), tr("Les employés ont été triés par ordre alphabétique."), QMessageBox::Ok);
    } else {
        // Si le modèle est vide ou l'erreur s'est produite, afficher un message d'erreur
        QMessageBox::critical(this, tr("Erreur"), tr("Impossible de trier les employés ou aucun employé trouvé."));
    }
}







void MainWindow::on_employe_clicked()
{
    // Supposons que la page de connexion est à l'index 0 du QTabWidget
    ui->tabWidget->setCurrentIndex(0); // Définit l'onglet actuel sur l'index 0
    int index = ui->tabWidget->indexOf(ui->ListeDesEmployes);

    if (index != -1) {
        // Passer à la page ListeDesEmployes
        ui->tabWidget->setCurrentIndex(index);
    } else {
        qDebug() << "La page 'ListeDesEmployes' est introuvable dans le QTabWidget.";
    }
}

void MainWindow::on_ajouter_clicked()
{
    int index = ui->tabWidget->indexOf(ui->AjouterEmploye); // Assurez-vous que "tab_2" est le nom exact de la page

    if (index != -1) {
        ui->tabWidget->setCurrentIndex(index);
    } else {
        qDebug() << "La page 'AjouterEmploye' est introuvable dans le QTabWidget.";
    }
}



void MainWindow::on_modifier_clicked()
{
    int index = ui->tabWidget->indexOf(ui->ModifierEmploye); // Vérifiez que "tab_3" est le bon nom de la page

    if (index != -1) {
        ui->tabWidget->setCurrentIndex(index);
    } else {
        qDebug() << "La page 'ModifierEmploye' est introuvable dans le QTabWidget.";
    }
}




void MainWindow::on_affiche_clicked()
{
    // Si le nom de l'objet est "tab_10"
    int index = ui->tabWidget->indexOf(ui->ListeDesEmployes);

    if (index != -1) {
        // Passer à la page ListeDesEmployes
        ui->tabWidget->setCurrentIndex(index);
    } else {
        qDebug() << "La page 'ListeDesEmployes' est introuvable dans le QTabWidget.";
    }
}


void MainWindow::on_ajouterc_clicked()
{
    QString cin = ui->ci->text().trimmed();
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer le CIN de l'employé.");
        return;
    }

    QString congeDebut = ui->congeD->text().trimmed();
    QString congeFin = ui->congeF->text().trimmed();

    QRegularExpression dateRegex("^\\d{4}-\\d{2}-\\d{2}$");
    if (!dateRegex.match(congeDebut).hasMatch() || !dateRegex.match(congeFin).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le format des dates est incorrect. Utilisez le format YYYY-MM-DD.");
        return;
    }

    QDate dateDebut = QDate::fromString(congeDebut, "yyyy-MM-dd");
    QDate dateFin = QDate::fromString(congeFin, "yyyy-MM-dd");
    if (!dateDebut.isValid() || !dateFin.isValid() || dateDebut > dateFin) {
        QMessageBox::warning(this, "Erreur", "Les dates de congé sont invalides.");
        return;
    }
    QSqlQuery query;
    query.prepare("UPDATE Employe SET CONGE_D = :conge_debut, CONGE_F = :conge_fin WHERE CIN = :cin");
    query.bindValue(":conge_debut", congeDebut);
    query.bindValue(":conge_fin", congeFin);
    query.bindValue(":cin", cin);

    if (!query.exec()) {
        qDebug() << "Erreur SQL : " << query.lastError().text();
        if (query.lastError().text().contains("ORA-03113") || query.lastError().text().contains("ORA-03114")) {
            qDebug() << "Perte de connexion détectée. Tentative de reconnexion...";
            if (!db.open()) {
                qDebug() << "Erreur : Impossible de reconnecter à la base de données.";
                return;
            }
            // Retenter la requête après reconnexion
            if (!query.exec()) {
                qDebug() << "Erreur après reconnexion : " << query.lastError().text();
            }
        }
    }



    /*QSqlQuery query;
    //query.prepare("UPDATE Employe SET CONGE_D = :conge_debut, CONGE_F = :conge_fin WHERE CIN = :cin");
    query.prepare("UPDATE Employes SET \"CONGE_D\" = :conge_debut, \"CONGE_F\" = :conge_fin WHERE CIN = :cin");
    query.bindValue(":conge_debut", dateDebut.toString("yyyy-MM-dd"));
    query.bindValue(":conge_fin", dateFin.toString("yyyy-MM-dd"));
    query.bindValue(":cin", cin);*/

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Les dates de congé ont été enregistrées.");
    } else {
        QMessageBox::critical(this, "Erreur", "Une erreur s'est produite lors de l'ajout du congé.");
        qDebug() << "Erreur SQL : " << query.lastError().text();
        qDebug() << "Requête exécutée : " << query.lastQuery();
    }
}












/*void MainWindow::on_ajouterA_clicked()
{
    QString cin = ui->ci->text().trimmed();
        if (cin.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer le CIN de l'employé.");
            return;
        }

        // Récupérer la date depuis le QDateEdit
            QDate dateAbsence = ui->dateA->date();
            QString dateAbsenceStr = dateAbsence.toString("dd/MM/yyyy");

            // Préparer la requête SQL
            QSqlQuery query;
            query.prepare("UPDATE employes SET ABSENCE = :absence WHERE CIN = :cin");
            query.bindValue(":absence", dateAbsenceStr);
            query.bindValue(":cin", cin);

            if (query.exec()) {
                QMessageBox::information(this, "Succès", "L'absence a été ajoutée pour l'employé.");
            } else {
                QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout de l'absence : " + query.lastError().text());
            }
}


void MainWindow::on_ajouterH_clicked()
{
    QString cin = ui->ci->text().trimmed();
        if (cin.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer le CIN de l'employé.");
            return;
        }

        // Récupérer les heures depuis les QTimeEdit
        QTime heureDebut = ui->heureD->time();
        QTime heureFin = ui->heureF->time();

        if (heureDebut >= heureFin) {
            QMessageBox::warning(this, "Erreur", "L'heure de fin doit être après l'heure de début.");
            return;
        }

        // Préparer la requête SQL
        QSqlQuery query;
        query.prepare("UPDATE employes SET HEURE_D = :heure_debut, HEURE_F = :heure_fin WHERE CIN = :cin");
        query.bindValue(":heure_debut", heureDebut.toString("hh:mm:ss"));
        query.bindValue(":heure_fin", heureFin.toString("hh:mm:ss"));
        query.bindValue(":cin", cin);

        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Les heures de travail ont été ajoutées pour l'employé.");
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout des heures de travail : " + query.lastError().text());
        }

}*/


void MainWindow::on_connectButton_clicked()
{
    // Connexion à l'Arduino via la classe Arduino
    if (arduino.connectToArduino("COM5")) {  // Utiliser la méthode connectToArduino
        qDebug() << "Connexion réussie à l'Arduino !";
    } else {
        qDebug() << "Erreur de connexion à l'Arduino : " << serialPort->errorString();
    }
}







void MainWindow::readFromArduino(const QString &data) {
    qDebug() << "Donnée brute reçue depuis Arduino :" << data;
    QString validCardID = data.trimmed();

    // Vérifiez si l'ID est vide
    if (validCardID.isEmpty()) {
        qDebug() << "Erreur : aucune carte détectée (cardID est vide).";
        cardIDLabel->setText("Aucune carte détectée.");
        return;
    }

    verifyCardID(data);
}
void MainWindow::onDataReceived(const QString &cardID)
{
    // Mettez à jour votre interface utilisateur avec l'ID de la carte
    ui->cardIDLabel->setText(cardID);
}
/*void MainWindow::verifyCardID(const QString &validCardID) {
    if (validCardID.isEmpty()) {
        qDebug() << "Aucun ID de carte reçu !";
        cardIDLabel->setText("Erreur : aucun ID reçu.");
        return; // Arrête ici si aucune carte n'est lue
    }

    // Vérification de l'ID dans la base de données
    QSqlQuery query;
    query.prepare("SELECT * FROM employe WHERE carteID = :id");
    query.bindValue(":id", validCardID);

    if (query.exec()) {
        if (query.next()) {
            // ID valide, afficher un message
            cardIDLabel->setText("Carte non reconnue.");
            qDebug() << "Carte non reconnue pour l'utilisateur :" << query.value("nom").toString();
        } else {
            // ID non valide
            cardIDLabel->setText("Carte valide.");
            qDebug() << "Carte valide : " << validCardID;
        }
    } else {
        // Erreur SQL
        qDebug() << "Erreur SQL lors de la vérification de la carte : " << query.lastError().text();
    }
}*/


void MainWindow::displayCardID(const QString &data)
{
    // Afficher l'ID de la carte dans le label
    cardIDLabel->setText("ID de la carte : " + data);
    verifyCardID(data);  // Vérifier l'ID dans la base de données
}

void MainWindow::verifyCardID(const QString &data)
{

    // Vérification de l'ID dans la base de données
    QSqlQuery query;
    query.prepare("SELECT * FROM employe WHERE carteID = :id");
    query.bindValue(":id", data);

    if (query.exec()) {
        if (query.next()) {
            // ID valide, afficher un message
            cardIDLabel->setText("Carte valide.");
        } else {
            // ID non valide
            cardIDLabel->setText("Carte valide.");
        }
    } else {
        qDebug() << "Erreur SQL: " << query.lastError();
    }
}



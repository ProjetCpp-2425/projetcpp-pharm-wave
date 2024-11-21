#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPdfWriter>
#include <QPainter>
#include <QTextDocument>
#include <QtNetwork/QSslSocket>
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>
#include <QTableView>
#include <QTextEdit>
#include <QScreen>
#include <QMainWindow>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QScreen>
#include <QGuiApplication>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>



#include "smtp.h"


#include "patient.h"


MainWindow::MainWindow(QWidget *patient)
    : QMainWindow(patient)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(p.afficher());

    // Connect buttons to respective slots
    connect(ui->projet, &QPushButton::clicked, this, &MainWindow::on_projet_clicked);
    connect(ui->employer, &QPushButton::clicked, this, &MainWindow::on_employer_clicked);
    connect(ui->materielle, &QPushButton::clicked, this, &MainWindow::on_materielle_clicked);
    connect(ui->fournisseur, &QPushButton::clicked, this, &MainWindow::on_fournisseur_clicked);
    connect(ui->facture, &QPushButton::clicked, this, &MainWindow::on_facture_clicked);
   connect(ui->ajout, &QPushButton::clicked, this, &MainWindow::on_ajout_clicked);
    connect(ui->supprimer, &QPushButton::clicked, this, &MainWindow::on_supprimer_clicked);
   connect(ui->PDF, &QPushButton::clicked, this, &MainWindow::on_PDF_clicked);
   connect(ui->stat, &QPushButton::clicked, this, &MainWindow::showPatientStatistics);
   connect(ui->recherche1, &QPushButton::clicked, this, &MainWindow::on_recherche1_clicked);





}

MainWindow::~MainWindow()
{
    delete ui;
}
// Slot to switch to "projet_3" widget
void MainWindow::on_projet_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);  // Assuming "projet_3" widget is at index 0
}

// Slot to switch to "employer_3" widget
void MainWindow::on_employer_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);  // Assuming "employer_3" widget is at index 1
}

// Slot to switch to "materielle_3" widget
void MainWindow::on_materielle_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);  // Assuming "materielle_3" widget is at index 2
}

// Slot to switch to "fournisseur_3" widget
void MainWindow::on_fournisseur_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);  // Assuming "fournisseur_3" widget is at index 3
}

// Slot to switch to "facture_3" widget
void MainWindow::on_facture_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);  // Assuming "facture_3" widget is at index 4
}


void MainWindow::on_ajout_clicked()
{
    qDebug() << "on_ajout_clicked() called";  // Message de débogage
    int AGE= ui->AGE_2->text().toInt();
    QString ADRESSE = ui->ADRESSE_2->text();
    int NUM_SOCIALE = ui->NUM_SOCIALE->text().toInt();
    QString NOM = ui->NOM->text();
    QString PRENOM = ui->PRENOM_2->text();
    QString E_MAIL = ui->E_MAIL_2->text();
    QString GENRE = ui->GENRE_2->text();
    int TELEPHONE = ui->TELEPHONE_2->text().toInt();
    // Déclaration des expressions régulières
    QRegularExpression nomPrenomRegex("^[A-Za-zÀ-ÖØ-öø-ÿ\\-\\s]+$");  // Lettres avec accents, tirets et espaces
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");  // Format e-mail plus précis
    QRegularExpression telephoneRegex("^(\\d{8}|\\d{2} \\d{3} \\d{3})$");  // Numéro tunisien (8 chiffres ou 2 3 3)

    // Validation des champs
        if (AGE <= 0 || AGE > 120) {
            QMessageBox::warning(this, "Erreur de saisie", "L'âge doit être un nombre positif entre 1 et 120.");
            return;
        }
        if (!nomPrenomRegex.match(NOM).hasMatch() || NOM.isEmpty()) {
            QMessageBox::warning(this, "Erreur de saisie", "Le nom doit contenir uniquement des lettres.");
            return;
        }
        if (!nomPrenomRegex.match(PRENOM).hasMatch() || PRENOM.isEmpty()) {
            QMessageBox::warning(this, "Erreur de saisie", "Le prénom doit contenir uniquement des lettres.");
            return;
        }
        if (GENRE != "homme" && GENRE != "femme") {
            QMessageBox::warning(this, "Erreur de saisie", "Le genre doit être 'homme' ou 'femme'.");
            return;
        }
        if (NUM_SOCIALE <= 0 || QString::number(NUM_SOCIALE).length() != 8) {
            QMessageBox::warning(this, "Erreur de saisie", "Le numéro social doit être un nombre de 8 chiffres.");
            return;
        }
        if (!telephoneRegex.match(QString::number(TELEPHONE)).hasMatch()) {
            QMessageBox::warning(this, "Erreur de saisie", "Le téléphone doit être un nombre de 8 chiffres.");
            return;
        }

        if (!emailRegex.match(E_MAIL).hasMatch()) {
            QMessageBox::warning(this, "Erreur de saisie", "L'adresse e-mail est invalide.");
            return;
        }

    class patient p(AGE, NUM_SOCIALE, TELEPHONE, NOM, PRENOM, GENRE, ADRESSE, E_MAIL);
    bool test = p.ajout();

    if (test)
    {
        // Actualisez le modèle de la vue si l'ajout est réussi
        ui->tableView->setModel(p.afficher()); // Assurez-vous que la fonction afficher() existe pour patient
        QMessageBox::information(nullptr, QObject::tr("Ajout réussi"),
                                 QObject::tr("Le patient a été ajouté avec succès.\n"
                                             "Cliquez sur Annuler pour fermer."), QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Ajout échoué"),
                              QObject::tr("Erreur lors de l'ajout du patient.\n"
                                          "Cliquez sur Annuler pour fermer."), QMessageBox::Cancel);
    }
}



void MainWindow::on_supprimer_clicked()
{
    // Récupérez le numéro social depuis l'interface utilisateur
    int numSociale = ui->NUM_SOCIALE->text().toInt();

    // Vérifiez si le numéro social existe dans la base
    if (p.rechercher(numSociale)) {
        // Supprime toutes les entrées avec le même numéro social
        bool test = p.supprimer(numSociale);

        if (test) {
            // Actualisez le modèle de la vue pour refléter les changements
            ui->tableView->setModel(p.afficher());
            QMessageBox::information(this, tr("Suppression réussie"),
                                     tr("Tous les enregistrements avec ce numéro social ont été supprimés."));
        } else {
            QMessageBox::critical(this, tr("Erreur"),
                                  tr("La suppression a échoué."));
        }
    } else {
        QMessageBox::warning(this, tr("Non trouvé"),
                             tr("Aucun enregistrement trouvé avec ce numéro social."));
    }
}

void MainWindow::on_Modifier_clicked()
{
    int AGE = ui->AGE_8->text().toInt();
    QString ADRESSE = ui->ADRESSE_8->text();
    int NUM_SOCIALE = ui->NUM_SOCIALE_8->text().toInt();
    QString NOM = ui->NOM_9->text();
    QString PRENOM = ui->PRENOM_8->text();
    QString E_MAIL = ui->E_MAIL_8->text();
    QString GENRE = ui->GENRE_8->text();
    int TELEPHONE = ui->TELEPHONE_8->text().toInt();

    // Déclaration des expressions régulières
    QRegularExpression nomPrenomRegex("^[A-Za-zÀ-ÖØ-öø-ÿ\\-\\s]+$");
    QRegularExpression E_MaillRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");  // Format e-mail plus précis
    QRegularExpression telephoneRegex("^(\\d{8}|\\d{2} \\d{3} \\d{3})$");

    // Validation des champs
    if (AGE <= 0 || AGE > 120) {
        QMessageBox::warning(this, "Erreur de saisie", "L'âge doit être un nombre positif entre 1 et 120.");
        return;
    }
    if (!nomPrenomRegex.match(NOM).hasMatch() || NOM.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le nom doit contenir uniquement des lettres.");
        return;
    }
    if (!nomPrenomRegex.match(PRENOM).hasMatch() || PRENOM.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le prénom doit contenir uniquement des lettres.");
        return;
    }
    if (GENRE != "homme" && GENRE != "femme") {
        QMessageBox::warning(this, "Erreur de saisie", "Le genre doit être 'homme' ou 'femme'.");
        return;
    }
    if (NUM_SOCIALE <= 0 || QString::number(NUM_SOCIALE).length() != 8) {
        QMessageBox::warning(this, "Erreur de saisie", "Le numéro social doit être un nombre de 8 chiffres.");
        return;
    }
    if (!telephoneRegex.match(QString::number(TELEPHONE)).hasMatch()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le téléphone doit être un nombre de 8 chiffres.");
        return;
    }

    if (!E_MaillRegex.match(E_MAIL).hasMatch()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'adresse e-mail est invalide.");
        return;
    }

    // Appeler la méthode update pour modifier les informations du patient
    bool test = p.update(NUM_SOCIALE, AGE, TELEPHONE, NOM, PRENOM, GENRE, ADRESSE, E_MAIL);

    if (test) {
        ui->tableView->setModel(p.afficher());  // Actualisez l'affichage de la table
        QMessageBox::information(this, tr("Modification réussie"),
                                 tr("Le patient a été modifié avec succès."));
    } else {
        QMessageBox::critical(this, tr("Erreur"),
                              tr("La modification a échoué."));
    }
}
void MainWindow::on_PDF_clicked()
{
   // int numSociale = ui->NUM_SOCIALE->text().toInt();  // Récupération du numéro social

   // if (p.rechercher(numSociale)) {  // Vérifiez si le patient existe
        p.genererPDF();  // Génère le PDF du patient
        //QMessageBox::information(this, "PDF généré", "Le PDF du profil du patient a été généré avec succès.");
   // } else {
        //QMessageBox::warning(this, "Patient non trouvé", "Le numéro social saisi ne correspond à aucun patient.");
    //}
}



void MainWindow::on_trier_clicked() {

    class patient s;
    if (ui->lowHigh->currentText() == "Low-To-High") {  // Sorting in ascending order
        if (ui->age->currentText() == "AGE") {  // Sort by AGE
            ui->tableView->setModel(s.trier_age_asc());
            QMessageBox::information(this, QObject::tr("Update Success"),
                                     QObject::tr("Record updated successfully.\nClick Cancel to exit."),
                                     QMessageBox::Cancel);
        //} else if (ui->age->currentText() == "DUREE") {  // Sort by DUREE
            //ui->tableView->setModel(s.trier_DUREE_ASC());
            //QMessageBox::information(this, QObject::tr("Update Success"),
                                    // QObject::tr("Record updated successfully.\nClick Cancel to exit."),
                                     //QMessageBox::Cancel);
        //} else {  // Error if none of the expected options are matched
           // QMessageBox::critical(this, QObject::tr("Update Failed"),
                                 // QObject::tr("Error: Could not update the record.\nClick Cancel to exit."),
                                  //QMessageBox::Cancel);
        }
    } else if (ui->lowHigh->currentText() == "High-To-Low") {  // Sorting in descending order
        if (ui->age->currentText() == "AGE") {  // Sort by AGE
            ui->tableView->setModel(s.trier_age_desc());
            QMessageBox::information(this, QObject::tr("Update Success"),
                                     QObject::tr("Record updated successfully.\nClick Cancel to exit."),
                                     QMessageBox::Cancel);

    }
    }
}



void MainWindow::on_mail_clicked()
{
    // Create a new QWidget for the email input dialog
    QWidget *emailDialog = new QWidget(this);
    emailDialog->setWindowTitle("Send Email");

    // Create input fields for the email details
    QVBoxLayout *layout = new QVBoxLayout(emailDialog);

    QLabel *fromLabel = new QLabel("From:", emailDialog);
    QLineEdit *fromLineEdit = new QLineEdit(emailDialog);

    QLabel *toLabel = new QLabel("To:", emailDialog);
    QLineEdit *toLineEdit = new QLineEdit(emailDialog);

    QLabel *subjectLabel = new QLabel("Subject:", emailDialog);
    QLineEdit *subjectLineEdit = new QLineEdit(emailDialog);

    QLabel *messageLabel = new QLabel("Message:", emailDialog);
    QTextEdit *messageTextEdit = new QTextEdit(emailDialog);

    QPushButton *sendButton = new QPushButton("Send Email", emailDialog);

    layout->addWidget(fromLabel);
    layout->addWidget(fromLineEdit);
    layout->addWidget(toLabel);
    layout->addWidget(toLineEdit);
    layout->addWidget(subjectLabel);
    layout->addWidget(subjectLineEdit);
    layout->addWidget(messageLabel);
    layout->addWidget(messageTextEdit);
    layout->addWidget(sendButton);

    emailDialog->setLayout(layout);
    emailDialog->resize(400, 300);
    emailDialog->show();

    // Connect the Send button to the function that sends the email
     setupMailInterface();
    connect(sendButton, &QPushButton::clicked, [this, fromLineEdit, toLineEdit, subjectLineEdit, messageTextEdit, emailDialog](){
        // Get the input values
        QString from = fromLineEdit->text();
        QString to = toLineEdit->text();
        QString subject = subjectLineEdit->text();
        QString message = messageTextEdit->toPlainText();

        // Send the email using SmtpClient
        smtp smtpClient;
        smtpClient.sendEmail(from, to, subject, message);

        // Show success message
        QMessageBox::information(this, "Email Sent", "Your email has been sent successfully!");

        // Close the email dialog
        emailDialog->close();
    });

}



void MainWindow::setupMailInterface()
{
    // Widget principal pour l'interface
    QDialog *mailWidget = new QDialog(this);
    mailWidget->setWindowTitle("Mailing Interface");
    mailWidget->resize(600, 500);

    // Couleur de fond et style global
    mailWidget->setStyleSheet("background-color: #f4f4f4; color: #333; font-family: Arial; font-size: 14px;");

    // Disposition principale
    QVBoxLayout *mainLayout = new QVBoxLayout(mailWidget);

    // Titre
    QLabel *titleLabel = new QLabel("Envoyer un Email", mailWidget);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #4CAF50; margin-bottom: 20px;");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Champs pour l'adresse de l'expéditeur
    QLabel *fromLabel = new QLabel("De :", mailWidget);
    QLineEdit *fromLineEdit = new QLineEdit(mailWidget);
    fromLineEdit->setPlaceholderText("Adresse email de l'expéditeur");

    // Champs pour l'adresse du destinataire
    QLabel *toLabel = new QLabel("À :", mailWidget);
    QLineEdit *toLineEdit = new QLineEdit(mailWidget);
    toLineEdit->setPlaceholderText("Adresse email du destinataire");

    // Champs pour l'objet
    QLabel *subjectLabel = new QLabel("Objet :", mailWidget);
    QLineEdit *subjectLineEdit = new QLineEdit(mailWidget);
    subjectLineEdit->setPlaceholderText("Sujet de l'email");

    // Champ pour le corps du message
    QLabel *messageLabel = new QLabel("Message :", mailWidget);
    QTextEdit *messageTextEdit = new QTextEdit(mailWidget);
    messageTextEdit->setPlaceholderText("Entrez votre message ici...");

    // Bouton d'envoi
    QPushButton *sendButton = new QPushButton("Envoyer", mailWidget);
    sendButton->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; padding: 10px; border-radius: 5px;");

    // Ajout des widgets à la disposition principale
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(fromLabel);
    mainLayout->addWidget(fromLineEdit);
    mainLayout->addWidget(toLabel);
    mainLayout->addWidget(toLineEdit);
    mainLayout->addWidget(subjectLabel);
    mainLayout->addWidget(subjectLineEdit);
    mainLayout->addWidget(messageLabel);
    mainLayout->addWidget(messageTextEdit);
    mainLayout->addWidget(sendButton);

    mailWidget->setLayout(mainLayout);

    // Centrer la fenêtre au milieu
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry(); // Obtenir la géométrie de l'écran principal
    int x = (screenGeometry.width() - mailWidget->width()) / 2;
    int y = (screenGeometry.height() - mailWidget->height()) / 2;
    mailWidget->move(x, y); // Positionner au centre

    // Assurez-vous que la fenêtre est au premier plan et focalisée
    mailWidget->raise();
    mailWidget->activateWindow();

    mailWidget->show();

    // Connexion du bouton d'envoi
    connect(sendButton, &QPushButton::clicked, [this, fromLineEdit, toLineEdit, subjectLineEdit, messageTextEdit] {
        QString from = fromLineEdit->text();
        QString to = toLineEdit->text();
        QString subject = subjectLineEdit->text();
        QString message = messageTextEdit->toPlainText();

        // Validation des champs
        if (from.isEmpty() || to.isEmpty() || subject.isEmpty() || message.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis !");
            return;
        }

        // Envoyer l'email
        smtp smtpClient;
        bool success = smtpClient.sendEmail(from, to, subject, message);

        if (success) {
            QMessageBox::information(this, "Succès", "L'email a été envoyé avec succès !");
        } else {
            QMessageBox::critical(this, "Échec", "L'envoi de l'email a échoué. Vérifiez les détails et réessayez.");
        }
    });
}



void MainWindow::showPatientStatistics() {
    // Instance de la classe `patient`
    class patient pt;

    // Générer les statistiques pour les patients
    QPieSeries *series = pt.generatePatientStatistics();
    if (!series || series->slices().isEmpty()) {
        QMessageBox::warning(this, "Statistiques des Patients", "Aucune donnée disponible pour les statistiques.");
        return;
    }

    // Créer le graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des Diagnostiques des Patients");
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Vérifiez si un ancien graphique existe déjà dans `frame_2` et le supprimez
    if (chartView) {
        delete chartView;
        chartView = nullptr;
    }

    // Créer une vue pour le graphique
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Attacher la vue au `frame_2` existant
    chartView->setParent(ui->widget_2);
    chartView->resize(ui->widget_2->size());
    chartView->show();
}
void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    if (chartView) {
        chartView->resize(ui->widget_2->size()); // Adapter la taille du graphique
    }
}
void MainWindow::afficherPatientParNumSocial()
{
    // Récupérer le numéro social depuis le QLineEdit (assurez-vous que le champ est rempli)
    int numSocial = ui->lineEdit_110->text().toInt();

    // Appeler la méthode rechercherParNumSocial et obtenir le modèle
    class patient p;
    QSqlQueryModel *model = p.rechercherParNumSocial(numSocial);

    if (model->rowCount() > 0) {
        // Associer le modèle à la vue de tableau
        ui->tableView->setModel(model);
    } else {
        qDebug() << "Aucun patient trouvé avec ce numéro social.";
    }
}

void MainWindow::on_recherche1_clicked()
{
    // Récupérer le numéro social depuis le champ d'entrée
       QString numSocial = ui->lineEdit_110->text();

       if (numSocial.isEmpty()) {
           QMessageBox::warning(this, "Recherche", "Veuillez entrer un numéro social !");
           return;
       }

       // Appeler la méthode de recherche
       class patient p;
       QSqlQueryModel *model = p.rechercherParNumSocial(numSocial.toInt());

       if (model->rowCount() > 0) {
           // Associer le modèle à la vue TableView
           ui->tableView->setModel(model);
       } else {
           QMessageBox::information(this, "Résultat", "Aucun patient trouvé avec ce numéro social.");
       }
}


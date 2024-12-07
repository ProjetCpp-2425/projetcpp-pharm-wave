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
#include "qrcodegen.hpp"  // The correct include based on the relative path from the project directory
#include <QPrintDialog>
#include <QPainter>
#include <QFormLayout>
#include <QPrinter>
#include <QPixmap>
#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <QMessageBox>
#include <QImage>




#include "smtp.h"


#include "patient.h"


MainWindow::MainWindow(QWidget *patient)
    : QMainWindow(patient)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int ret=A.connect_arduino(); // lancer la connexion à arduino
    switch(ret){
    case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
        break;
    case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
       break;
    case(-1):qDebug() << "arduino is not available";
    }
     QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
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
   connect(ui->QR, &QPushButton::clicked, this, &MainWindow::on_QR_clicked);






}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::update_label() {
    QByteArray data = A.getserial()->readAll();

    // Check if data contains "1"
    if (!data.isEmpty() && data.trimmed() == "1") {
        ui->label_20->setText("bienvenue  chez pharmwave");
    } else {
        ui->label_20->setText("Aucune personne");
    }
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



/*void MainWindow::on_trier_clicked() {

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
}*/
// LKJLJ?JJO

/*void MainWindow::on_trier_clicked()
{
    class patient s;

    if (ui->lowHigh->currentText() == "Low-To-High") {
        ui->tableView->setModel(s.trier_age_asc());
        QMessageBox::information(this, QObject::tr("Tri réussi"),
                                 QObject::tr("Tri par âge croissant réussi."),
                                 QMessageBox::Ok);
    } else if (ui->lowHigh->currentText() == "High-To-Low") {
        ui->tableView->setModel(s.trier_age_desc());
        QMessageBox::information(this, QObject::tr("Tri réussi"),
                                 QObject::tr("Tri par âge décroissant réussi."),
                                 QMessageBox::Ok);
    } else if (ui->lowHigh->currentText() == "homme") {
        ui->tableView->setModel(s.filtrer_par_genre("homme"));
        QMessageBox::information(this, QObject::tr("Filtre appliqué"),
                                 QObject::tr("Filtré par genre : homme."),
                                 QMessageBox::Ok);
    } else if (ui->lowHigh->currentText() == "femme") {
        ui->tableView->setModel(s.filtrer_par_genre("femme"));
        QMessageBox::information(this, QObject::tr("Filtre appliqué"),
                                 QObject::tr("Filtré par genre : femme."),
                                 QMessageBox::Ok);
    } else {
        QMessageBox::critical(this, QObject::tr("Erreur"),
                              QObject::tr("Aucun critère de tri valide sélectionné."),
                              QMessageBox::Cancel);
    }
}*/
void MainWindow::on_trier_clicked()
{
    class patient s;

    // Tri par âge croissant
    if (ui->lowHigh->currentText() == "Low-To-High") {
        if (ui->age->currentText() == "AGE") {
            ui->tableView->setModel(s.trier_age_asc());
            QMessageBox::information(this, QObject::tr("Tri réussi"),
                                     QObject::tr("Tri par âge croissant réussi."),
                                     QMessageBox::Ok);
        } else if (ui->age->currentText() == "Nom") {
            ui->tableView->setModel(s.trier_nom_asc());  // Tri par nom croissant
            QMessageBox::information(this, QObject::tr("Tri réussi"),
                                     QObject::tr("Tri par nom croissant réussi."),
                                     QMessageBox::Ok);
        } else if (ui->age->currentText() == "Prénom") {
            ui->tableView->setModel(s.trier_prenom_asc());  // Tri par prénom croissant
            QMessageBox::information(this, QObject::tr("Tri réussi"),
                                     QObject::tr("Tri par prénom croissant réussi."),
                                     QMessageBox::Ok);
        } else {
            QMessageBox::critical(this, QObject::tr("Erreur"),
                                  QObject::tr("Aucun critère de tri valide sélectionné."),
                                  QMessageBox::Cancel);
        }
    }
    // Tri par âge décroissant
    else if (ui->lowHigh->currentText() == "High-To-Low") {
        if (ui->age->currentText() == "AGE") {
            ui->tableView->setModel(s.trier_age_desc());
            QMessageBox::information(this, QObject::tr("Tri réussi"),
                                     QObject::tr("Tri par âge décroissant réussi."),
                                     QMessageBox::Ok);
        } else if (ui->age->currentText() == "Nom") {
            ui->tableView->setModel(s.trier_nom_desc());  // Tri par nom décroissant
            QMessageBox::information(this, QObject::tr("Tri réussi"),
                                     QObject::tr("Tri par nom décroissant réussi."),
                                     QMessageBox::Ok);
        } else if (ui->age->currentText() == "Prénom") {
            ui->tableView->setModel(s.trier_prenom_desc());  // Tri par prénom décroissant
            QMessageBox::information(this, QObject::tr("Tri réussi"),
                                     QObject::tr("Tri par prénom décroissant réussi."),
                                     QMessageBox::Ok);
        } else {
            QMessageBox::critical(this, QObject::tr("Erreur"),
                                  QObject::tr("Aucun critère de tri valide sélectionné."),
                                  QMessageBox::Cancel);
        }
    }
    // Filtrer par genre "homme"
    else if (ui->lowHigh->currentText() == "homme") {
        ui->tableView->setModel(s.filtrer_par_genre("homme"));
        QMessageBox::information(this, QObject::tr("Filtre appliqué"),
                                 QObject::tr("Filtré par genre : homme."),
                                 QMessageBox::Ok);
    }
    // Filtrer par genre "femme"
    else if (ui->lowHigh->currentText() == "femme") {
        ui->tableView->setModel(s.filtrer_par_genre("femme"));
        QMessageBox::information(this, QObject::tr("Filtre appliqué"),
                                 QObject::tr("Filtré par genre : femme."),
                                 QMessageBox::Ok);
    } else {
        QMessageBox::critical(this, QObject::tr("Erreur"),
                              QObject::tr("Aucun critère de tri valide sélectionné."),
                              QMessageBox::Cancel);
    }
}




/*void MainWindow::on_mail_clicked()
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

}*/
void MainWindow::on_mail_clicked()
{
    // Créer une nouvelle fenêtre pour l'interface de mail
    QWidget *emailDialog = new QWidget(this);
    emailDialog->setWindowTitle("Envoyer un Email");

    // Créer un layout vertical
    QVBoxLayout *mainLayout = new QVBoxLayout(emailDialog);

    // Titre de la fenêtre
    QLabel *titleLabel = new QLabel("Envoyer un Email", emailDialog);
    titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #388e3c; margin-bottom: 20px;");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Champs pour l'adresse de l'expéditeur
    QLabel *fromLabel = new QLabel("De :", emailDialog);
    QLineEdit *fromLineEdit = new QLineEdit(emailDialog);
    fromLineEdit->setPlaceholderText("Adresse email de l'expéditeur");

    // Champs pour l'adresse du destinataire
    QLabel *toLabel = new QLabel("À :", emailDialog);
    QLineEdit *toLineEdit = new QLineEdit(emailDialog);
    toLineEdit->setPlaceholderText("Adresse email du destinataire");

    // Champs pour l'objet
    QLabel *subjectLabel = new QLabel("Objet :", emailDialog);
    QLineEdit *subjectLineEdit = new QLineEdit(emailDialog);
    subjectLineEdit->setPlaceholderText("Sujet de l'email");

    // Champ pour le corps du message
    QLabel *messageLabel = new QLabel("Message :", emailDialog);
    QTextEdit *messageTextEdit = new QTextEdit(emailDialog);
    messageTextEdit->setPlaceholderText("Entrez votre message ici...");

    // Bouton d'envoi
    QPushButton *sendButton = new QPushButton("Envoyer", emailDialog);
    sendButton->setStyleSheet("background-color: #388e3c; color: white; font-weight: bold; padding: 10px; border-radius: 5px;");

    // Bouton quitter
    QPushButton *quitButton = new QPushButton("Quitter", emailDialog);
    quitButton->setStyleSheet("background-color: #f44336; color: white; font-weight: bold; padding: 10px; border-radius: 5px;");

    // Ajouter les widgets au layout principal
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(fromLabel);
    mainLayout->addWidget(fromLineEdit);
    mainLayout->addWidget(toLabel);
    mainLayout->addWidget(toLineEdit);
    mainLayout->addWidget(subjectLabel);
    mainLayout->addWidget(subjectLineEdit);
    mainLayout->addWidget(messageLabel);
    mainLayout->addWidget(messageTextEdit);

    // Ajouter les boutons à un layout horizontal
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(sendButton);
    buttonLayout->addWidget(quitButton);

    // Ajouter le layout des boutons au layout principal
    mainLayout->addLayout(buttonLayout);

    // Appliquer des styles pour le fond et le texte
    emailDialog->setStyleSheet("background-color: #e8f5e9; color: #388e3c; font-family: Arial; font-size: 14px;");
    sendButton->setStyleSheet("background-color: #388e3c; color: white; font-weight: bold; padding: 10px; border-radius: 5px;");
    quitButton->setStyleSheet("background-color: #f44336; color: white; font-weight: bold; padding: 10px; border-radius: 5px;");

    emailDialog->resize(600, 350);  // Ajuster la taille selon vos préférences

    // Centrer la fenêtre de l'email
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();  // Obtenir la géométrie de l'écran
    int x = (screenGeometry.width() - emailDialog->width()) / 2;
    int y = (screenGeometry.height() - emailDialog->height()) / 2;
    emailDialog->move(x, y);  // Positionner la fenêtre au centre

    emailDialog->show();

    // Connexion du bouton "Envoyer"
    connect(sendButton, &QPushButton::clicked, [this, fromLineEdit, toLineEdit, subjectLineEdit, messageTextEdit, emailDialog](){
        QString from = fromLineEdit->text();
        QString to = toLineEdit->text();
        QString subject = subjectLineEdit->text();
        QString message = messageTextEdit->toPlainText();

        // Validation des champs
        if (from.isEmpty() || to.isEmpty() || subject.isEmpty() || message.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Tous les champs doivent être remplis !");
            return;
        }

        // Envoyer l'email via le client SMTP
        smtp smtpClient;
        bool success = smtpClient.sendEmail(from, to, subject, message);

        if (success) {
            QMessageBox::information(this, "Succès", "L'email a été envoyé avec succès !");
        } else {
            QMessageBox::critical(this, "Échec", "L'envoi de l'email a échoué. Vérifiez les détails et réessayez.");
        }

        emailDialog->close();
    });

    // Connexion du bouton "Quitter"
    connect(quitButton, &QPushButton::clicked, [emailDialog](){
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

/*void MainWindow::on_recherche1_clicked()
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
}*/
void MainWindow::on_recherche1_clicked()
{
    // Récupérer le contenu du champ d'entrée
    QString input = ui->lineEdit_110->text();

    if (input.isEmpty()) {
        QMessageBox::warning(this, "Recherche", "Veuillez entrer un numéro social ou une adresse e-mail !");
        return;
    }

    // Vérifier si c'est un numéro ou un e-mail
    bool isNumber;
    int numSocial = input.toInt(&isNumber); // Conversion en nombre
    QSqlQueryModel *model = nullptr;
    class patient p;

    if (isNumber) {
        // Recherche par numéro social
        model = p.rechercherParNumSocial(numSocial);
    } else {
        // Validation de l'adresse e-mail
        QRegularExpression emailRegex("^[\\w\\.]+@[\\w\\.]+\\.[a-z]{2,}$");
        if (!emailRegex.match(input).hasMatch()) {
            QMessageBox::warning(this, "Recherche", "Veuillez entrer une adresse e-mail valide !");
            return;
        }

        // Recherche par e-mail
        model = p.rechercherParEmail(input);
    }

    // Afficher les résultats
    if (model && model->rowCount() > 0) {
        ui->tableView->setModel(model);
    } else {
        QMessageBox::information(this, "Résultat", "Aucun patient trouvé avec les critères fournis.");
    }
}



QImage MainWindow::generateQRCodeImage(const QString &data)
{
    const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(data.toStdString().c_str(), qrcodegen::QrCode::Ecc::LOW);

       int size = qr.getSize();
       QImage image(size, size, QImage::Format_RGB32);

       image.fill(Qt::white);

       for (int y = 0; y < size; ++y) {
           for (int x = 0; x < size; ++x) {
               if (qr.getModule(x, y)) {
                   image.setPixelColor(x, y, Qt::black);
               }
           }
       }
       return image.scaled(300, 300, Qt::KeepAspectRatio);
   }
/*QString MainWindow::fetchRepairData(const QString &idReparation)
{
    // Assuming you have a QSqlDatabase connection already established.
    QSqlQuery query;

    // Updated query to join ENREGISTRER and ACHETER, removing duplicates for VIGNETTE
    query.prepare(
        "SELECT DISTINCT A.VIGNETTE, E.ID_TRAN "
        "FROM ENREGISTRER E "
        "INNER JOIN ACHETER A ON E.VIGNETTE = A.VIGNETTE "
        "WHERE E.ID_TRAN = :idReparation"
    );
    query.bindValue(":idReparation", idReparation);

    if (!query.exec()) {
        qDebug() << "Database query failed: " << query.lastError();
        return "";
    }

    if (query.next()) {
        QString vignette = query.value("VIGNETTE").toString();
        QString idTran = query.value("ID_TRAN").toString();

        // Format the fetched data as a string
        QString repairData = "VIGNETTE: " + vignette + "\n"
                             "ID_TRAN: " + idTran;

        return repairData;
    }

    return "";  // Return an empty string if no data is found
}*/
QString MainWindow::fetchPatientData(const QString &idPatient) {
    QSqlQuery query;
    query.prepare("SELECT * FROM PATIENT WHERE NUM_SOCIALE = :id");
    query.bindValue(":id", idPatient);

    if (!query.exec()) {
        qDebug() << "Database error: " << query.lastError().text();
        return "";
    }

    if (query.next()) {
        // Construire une chaîne contenant les informations du patient
        QString patientData = QString("Nom: %1\nPrénom: %2\nÂge: %3\nGenre: %4\nNuméro sociale: %5\nTéléphone: %6\nAdresse: %7\nE-mail: %8")
            .arg(query.value("NOM").toString())
            .arg(query.value("PRENOM").toString())
            .arg(query.value("AGE").toString())
            .arg(query.value("GENRE").toString())
            .arg(query.value("NUM_SOCIALE").toString())
            .arg(query.value("TELEPHONE").toString())
            .arg(query.value("ADRESSE").toString())
            .arg(query.value("E_MAIL").toString());

        return patientData;
    }

    return "";  // Si aucun patient n'est trouvé
}


void MainWindow::generatePDF(const QString &filePath, const QString &repairData, const QImage &qrCodeImage)
{
    // Fetch the repair data using the fetchRepairData function
    // QString repairData = fetchRepairData(idReparation);

     // If no data is found for the given repair ID
     if (repairData.isEmpty()) {
         QMessageBox::warning(this, "Error", "No data found for the given Reparation ID.");
         return;
     }

     // Create a PDF writer and painter to generate the PDF
     QPdfWriter pdfWriter(filePath);
     QPainter painter(&pdfWriter);
     pdfWriter.setPageSize(QPageSize(QPageSize::A4));
     pdfWriter.setPageOrientation(QPageLayout::Portrait);

     // Set background color for the top section
     painter.fillRect(QRect(0, 0, pdfWriter.width(), 1000), QColor(0, 255, 0));  // Background color (green)

     // Title font and text
     QFont titleFont("Arial", 24, QFont::Bold);
     painter.setFont(titleFont);
     painter.drawText(100, 150, "PATIENT Report");

     // Content font for Repair Data
     QFont contentFont("Arial", 12);
     painter.setFont(contentFont);

     // Define initial Y-position for the repair data
     int yPos = 400;  // Starting position for the repair details
     int lineSpacing = 1000;  // Increase space between each line (increased from 20 to 40)

     // Split the repair data into lines and draw each line vertically
     QStringList repairLines = repairData.split("\n");
     for (const QString &line : repairLines) {
         painter.drawText(100, yPos, line);
         yPos += lineSpacing;  // Move down for the next line
     }

     // Generate a URL or text for the QR code with the repair data
     QString qrCodeData = "Repair Details:\n" + repairData;



     // Adjust the Y position before drawing the QR code
     yPos += 80;  // Add more space before the QR code (increased from 50 to 80)

     // Draw QR Code (this is the link to the PDF file or repair info)
     QRect qrRect(100, yPos, 150, 150);  // Positioned QR code
   //  painter.drawImage(qrRect, qrImage);

     // Add clickable link (assuming this URL leads to the repair details or PDF)
     painter.setFont(QFont("Arial", 10));
     QRect linkRect(100, yPos + 180, 300, 20);  // Define the clickable area
     painter.drawText(linkRect, Qt::AlignLeft, "Click here to view the repair details");

     // Set the link in the PDF (the link will open a URL when clicked)
//     pdfWriter.addAction(QAction("Open repair details", this), linkRect);

     // Optionally, add a description below the QR code
     painter.setFont(QFont("Arial", 10));
    // painter.drawText(100, yPos + 200, "Scan the QR code to view or download the repair details");

     // End the painter
     painter.end();

     // Show success message after PDF generation
     QMessageBox::information(this, "Success", "PDF generated successfully.");
}
void MainWindow:: on_QR_clicked()
{
    QWidget *qrWidget = new QWidget(this);
        qrWidget->setWindowTitle("QR Code Generator");
        qrWidget->setFixedSize(400, 600);

        // Center the widget on the screen
        qrWidget->move((this->width() - qrWidget->width()) / 2, (this->height() - qrWidget->height()) / 2);

        // Layout for widget
        QVBoxLayout *layout = new QVBoxLayout(qrWidget);
        layout->setContentsMargins(20, 20, 20, 20);
        layout->setSpacing(15);

        // Style for the widget
        qrWidget->setStyleSheet(
            "QWidget {"
            "background-color: #f0f0f0;"
            "border-radius: 10px;"
            "border: 1px solid #ccc;"
            "}");

        // Input for ID Reparation
        QLineEdit *idPatientLineEdit = new QLineEdit(qrWidget);
            idPatientLineEdit->setPlaceholderText("Enter Patient Social Number");
            idPatientLineEdit->setFont(QFont("Arial", 14));
            idPatientLineEdit->setFixedHeight(40);
            idPatientLineEdit->setStyleSheet(
            "QLineEdit {"
            "background-color: white;"
            "border: 1px solid #ccc;"
            "border-radius: 5px;"
            "padding-left: 10px;"
            "}");
         layout->addWidget(idPatientLineEdit);

        // Button to generate QR Code
        QPushButton *generateQRCodeButton = new QPushButton("Generate QR Code", qrWidget);
        generateQRCodeButton->setStyleSheet(
            "QPushButton {"
            "background-color: #4CAF50;"
            "color: white;"
            "border-radius: 5px;"
            "padding: 10px 20px;"
            "font-size: 14px;"
            "border: none;"
            "}"
            "QPushButton:hover {"
            "background-color: #45a049;"
            "}");
        layout->addWidget(generateQRCodeButton);

        // Display QR code
        QLabel *qrCodeLabel = new QLabel(qrWidget);
        qrCodeLabel->setAlignment(Qt::AlignCenter);
        qrCodeLabel->setFixedSize(300, 300);
        qrCodeLabel->setStyleSheet(
            "QLabel {"
            "border: 1px solid #ccc;"
            "background-color: white;"
            "border-radius: 5px;"
            "}");
        layout->addWidget(qrCodeLabel);

        // Button to generate PDF
        QPushButton *generatePDFButton = new QPushButton("Generate PDF", qrWidget);
        generatePDFButton->setStyleSheet(
            "QPushButton {"
            "background-color: #2196F3;"
            "color: white;"
            "border-radius: 5px;"
            "padding: 10px 20px;"
            "font-size: 14px;"
            "border: none;"
            "}"
            "QPushButton:hover {"
            "background-color: #1e88e5;"
            "}");
        layout->addWidget(generatePDFButton);

        // Print QR Code Button
        QPushButton *printButton = new QPushButton("Print QR Code", qrWidget);
        printButton->setStyleSheet(
            "QPushButton {"
            "background-color: #FFC107;"
            "color: white;"
            "border-radius: 5px;"
            "padding: 10px 20px;"
            "font-size: 14px;"
            "border: none;"
            "}"
            "QPushButton:hover {"
            "background-color: #ffb300;"
            "}");
        layout->addWidget(printButton);

        // Quit button
        QPushButton *quitButton = new QPushButton("Quit", qrWidget);
        quitButton->setStyleSheet(
            "QPushButton {"
            "background-color: #f44336;"
            "color: white;"
            "border-radius: 5px;"
            "padding: 10px 20px;"
            "font-size: 14px;"
            "border: none;"
            "}"
            "QPushButton:hover {"
            "background-color: #e53935;"
            "}");
        layout->addWidget(quitButton);

        // Connect Generate QR Code button
       connect(generateQRCodeButton, &QPushButton::clicked, this, [this, idPatientLineEdit, qrCodeLabel]() {
              QString idPatient = idPatientLineEdit->text();
               if (idPatient.isEmpty()) {
                QMessageBox::warning(this, "Input Error", "Please enter a valid Patient Social Number.");
                return;
            }


               QString patientData = fetchPatientData(idPatient);
               if (patientData.isEmpty()) {
                   QMessageBox::warning(this, "Erreur", "Aucune donnée trouvée pour ce numéro de patient.");
                   return;
               }

            // Generate the QR code with the repair data
             QImage qrCodeImage = generateQRCodeImage(patientData);  // Use patient data here
              qrCodeLabel->setPixmap(QPixmap::fromImage(qrCodeImage).scaled(300, 300, Qt::KeepAspectRatio));
        });

        // Connect Generate PDF button
         connect(generatePDFButton, &QPushButton::clicked, this, [this, idPatientLineEdit]() {
            QString idPatient = idPatientLineEdit->text();
            if (idPatient.isEmpty()) {
                QMessageBox::warning(this, "Input Error", "Please enter a valid Reparation ID.");
                return;
            }

            // Fetch repair data from the database using the ID Reparation
            QString repairData = fetchPatientData(idPatient);  // Fetch repair details

            if (repairData.isEmpty()) {
                QMessageBox::warning(this, "Error", "No repair data found for this ID.");
                return;
            }

            // Generate the PDF file
            QString filePath = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");
            if (filePath.isEmpty())
                return;

            // Create the PDF with repair data and QR Code
            QImage qrCodeImage = generateQRCodeImage(repairData);  // Use the repair data for QR code
            generatePDF(filePath, repairData, qrCodeImage);  // Pass the repair data and QR code here
        });

        // Print QR Code
        connect(printButton, &QPushButton::clicked, this, [this, qrCodeLabel]() {
            QPrinter printer;
            QPrintDialog printDialog(&printer, this);
            if (printDialog.exec() == QDialog::Accepted) {
                QPainter painter(&printer);
                QRect rect = painter.viewport();
                int width = qrCodeLabel->pixmap().width();
                int height = qrCodeLabel->pixmap().height();
                painter.drawPixmap((rect.width() - width) / 2, (rect.height() - height) / 2, qrCodeLabel->pixmap());
            }
        });

        // Quit Button - Close the QR Widget
        connect(quitButton, &QPushButton::clicked, qrWidget, &QWidget::close);

        qrWidget->show();  // Show the widget
}

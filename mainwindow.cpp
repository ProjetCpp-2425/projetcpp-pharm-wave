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

#include "smtp.h"


#include "patient.h"


MainWindow::MainWindow(QWidget *patient)
    : QMainWindow(patient)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(p.afficher());

    // Connect buttons to respective slots
  //  connect(ui->projet, &QPushButton::clicked, this, &MainWindow::on_projet_clicked);
    //connect(ui->employer, &QPushButton::clicked, this, &MainWindow::on_employer_clicked);
    //connect(ui->materielle, &QPushButton::clicked, this, &MainWindow::on_materielle_clicked);
    //connect(ui->fournisseur, &QPushButton::clicked, this, &MainWindow::on_fournisseur_clicked);
    //connect(ui->facture, &QPushButton::clicked, this, &MainWindow::on_facture_clicked);
   // connect(ui->ajout, &QPushButton::clicked, this, &MainWindow::on_ajout_clicked);
    //connect(ui->supprimer, &QPushButton::clicked, this, &MainWindow::on_supprimer_clicked);
   // connect(ui->PDF, &QPushButton::clicked, this, &MainWindow::on_PDF_clicked);

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
//
/*void MainWindow::on_delete_2_clicked()
{
    int NUM_SOCIALE = ui->NUM_SOCIALE->text().toInt();

    bool test = p.supprimer(NUM_SOCIALE);
    if(test)
    {
        ui->tableView->setModel(p.afficher());  // Actualise l'affichage de la table
        QMessageBox::information(nullptr, QObject::tr("Suppression réussie"),
                                 QObject::tr("L'élément a été supprimé avec succès.\n"
                                             "Cliquez sur Annuler pour fermer."), QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Échec de la suppression"),
                              QObject::tr("Erreur lors de la suppression.\n"
                                          "Cliquez sur Annuler pour fermer."), QMessageBox::Cancel);
    }
}*/



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
        //} else if (ui->age->currentText() == "DUREE") {  // Sort by DUREE
            //ui->tableView->setModel(s.trier_DUREE_dsc());
            //QMessageBox::information(this, QObject::tr("Update Success"),
                                    // QObject::tr("Record updated successfully.\nClick Cancel to exit."),
                                    // QMessageBox::Cancel);
        //} else {  // Error if none of the expected options are matched
            //QMessageBox::critical(this, QObject::tr("Update Failed"),
                                 // QObject::tr("Error: Could not update the record.\nClick Cancel to exit."),
                                 // QMessageBox::Cancel);
       // }
   // } else {  // Error if none of the sorting options are matched
       // QMessageBox::critical(this, QObject::tr("Update Failed"),
                             // QObject::tr("Error: Could not update the record.\nClick Cancel to exit."),
                             // QMessageBox::Cancel);
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

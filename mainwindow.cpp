#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "finance.h"
#include <QMessageBox>
#include <QSqlError>
#include <QRegularExpressionValidator>
#include <QValidator>
#include <QDate>
#include <QRegularExpression>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QAbstractItemModel>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QVector>
#include <QPieSeries>
#include <QPieSlice>
#include <QChart>
#include <QChartView>
#include <QTableView>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QRandomGenerator>
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
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include "employe.h"
#include "arduino.h"


#include "smtp.h"


#include "patient.h"

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent), ui(new Ui::MainWindow), chartView(nullptr),
     arduino(new Arduino(this))

{

    ui->setupUi(this);
    ui->tableView_2->setModel(p.afficher());
    networkManager = new QNetworkAccessManager(this);
    ui->tableViewtr->setModel(f.afficher());
    // Initialize QNetworkAccessManager
    connect(ui->employer, &QPushButton::clicked, this, &MainWindow::on_employer_clicked);

connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onNetworkReply);
  // Connect buttons to respective slots
    connect(ui->employe, &QPushButton::clicked, this, &MainWindow::on_employe_clicked);
    connect(ui->employer, &QPushButton::clicked, this, &MainWindow::on_employer_clicked);
    connect(ui->materielle, &QPushButton::clicked, this, &MainWindow::on_materielle_clicked);
    connect(ui->fournisseur, &QPushButton::clicked, this, &MainWindow::on_fournisseur_clicked);
    connect(ui->facture, &QPushButton::clicked, this, &MainWindow::on_facture_clicked);
    connect(ui->ajouter_f, &QPushButton::clicked, this, &MainWindow::on_pushButton_ajouter_clicked);
    connect(ui->supprimer_f, &QPushButton::clicked, this, &MainWindow::on_pushButton_supprimer_clicked);
    connect(ui->modifier_f, &QPushButton::clicked, this, &MainWindow::on_pushButton_modifierer_clicked);
    connect(ui->actu, &QPushButton::clicked, this, &MainWindow::on_ACTUALISER_clicked);
    connect(ui->comboBox_trii, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_comboBox_trii_changed);
    connect(ui->modifier_f, &QPushButton::clicked, this, &MainWindow::on_pushButton_9_clicked);
    connect(ui->generatePieChartButton, &QPushButton::clicked, this, &MainWindow::on_generatePieChartButton_clicked);
    //aziz
    connect(ui->ajout, &QPushButton::clicked, this, &MainWindow::on_ajout_clicked);
     connect(ui->supprimer, &QPushButton::clicked, this, &MainWindow::on_supprimer_clicked);
    connect(ui->PDF, &QPushButton::clicked, this, &MainWindow::on_PDF_clicked);
    connect(ui->stat, &QPushButton::clicked, this, &MainWindow::showPatientStatistics);
    connect(ui->recherche1, &QPushButton::clicked, this, &MainWindow::on_recherche1_clicked);
    connect(ui->QR, &QPushButton::clicked, this, &MainWindow::on_QR_clicked);

    ///////////////////////////////////// tasnim

    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::on_pushButton_ajouterr_clicked);
     connect(ui->spprimer, &QPushButton::clicked, this, &MainWindow::on_spprimer_clicked);

     connect(ui->pdf, &QPushButton::clicked, this, &MainWindow::on_pdf_clicked);


}




MainWindow::~MainWindow()
{
     delete ui;
     delete chartView;
}
void MainWindow::on_send_clicked()
{
    QString phoneNumber = ui->numero->text();  // Phone number from the line edit
    QString id_t = ui->text->text();  // Additional text from the line edit

    QSqlQuery query;
    query.prepare("SELECT CATEGORIE, MONTANT FROM FINANCE WHERE ID_T = :id_t");
    query.bindValue(":id_t", id_t);

    if (!query.exec()) {
        qDebug() << "Database query failed:" << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to query the database.");
        return;
    }

    if (query.next()) {
        QString categorie = query.value("CATEGORIE").toString();
        double montant = query.value("MONTANT").toDouble();

        // Check if the category is "Achat" (case-sensitive)
        if (categorie == "Achat") {
            QString message = QString("The amount for ID %1 is: %2")
                                .arg(id_t)
                                .arg(QString::number(montant, 'f', 2));

            // Send SMS with the message
            sendSMS(phoneNumber, message);
            qDebug() << "SMS sent successfully to" << phoneNumber << "with message:" << message;
        } else {
            // Log the actual category value if it's not 'Achat'
            qDebug() << "The CATEGORIE for ID_T" << id_t << "is not 'Achat' but '" << categorie << "'. No SMS sent.";
        }
    } else {
        qDebug() << "No record found for ID_T:" << id_t;
    }
}

void MainWindow::sendSMS(QString num, QString text)
{
      // Generate random key
    QString messageBody = "Your PHARMWAVE Achat: "  + text;

    QString recipient = "+216" + num;  // Format the recipient phone number
    QUrl url("https://api.twilio.com/2010-04-01/Accounts/ACeca0a69f8a04a526ccfa8c2712824b23/Messages.json");  // Replace with your new SID
    QNetworkRequest request(url);

    QString accountSid = "ACeca0a69f8a04a526ccfa8c2712824b23";  // Replace with your new Account SID
    QString authToken = "06ecf675117d6a743c37418e8177251c";   // Replace with your new Auth Token
    QString credentials = accountSid + ":" + authToken;
    QString authHeader = "Basic " + credentials.toUtf8().toBase64();

    request.setRawHeader("Authorization", authHeader.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("To", recipient);
    params.addQueryItem("From", "+14797778127");  // Replace with your new Twilio number
    params.addQueryItem("Body", messageBody);

    networkManager->post(request, params.toString(QUrl::FullyEncoded).toUtf8());
}


void MainWindow::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QMessageBox::information(this, "Success", "SMS sent successfully!");
    } else {
        QMessageBox::critical(this, "Error", "Failed to send SMS: " + reply->errorString());
    }
    reply->deleteLater();
}


// Slot to switch to "projet_3" widget
void MainWindow::on_employe_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);  // Assuming "projet_3" widget is at index 0
    int index = ui->tabWidget_3->indexOf(ui->ListeDesEmployes_2);

    if (index != -1) {
        // Passer à la page ListeDesEmployes
        ui->tabWidget_3->setCurrentIndex(index);
    }
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
//aziz
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
        ui->tableView_2->setModel(p.afficher()); // Assurez-vous que la fonction afficher() existe pour patient
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
            ui->tableView_2->setModel(p.afficher());
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
        ui->tableView_2->setModel(p.afficher());  // Actualisez l'affichage de la table
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
            ui->tableView_2->setModel(s.trier_age_asc());
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
            ui->tableView_2->setModel(s.trier_age_desc());
            QMessageBox::information(this, QObject::tr("Update Success"),
                                     QObject::tr("Record updated successfully.\nClick Cancel to exit."),
                                     QMessageBox::Cancel);

    }
    }
}
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
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (chartView) {  // Use the member variable
        chartView->resize(ui->widget_2->size());  // Adjust the size of the chart
    }
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
    if (chartView) {  // Vérifie si `chartView` n'est pas un pointeur nul
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


void MainWindow::afficherPatientParNumSocial()
{
    // Récupérer le numéro social depuis le QLineEdit (assurez-vous que le champ est rempli)
    int numSocial = ui->lineEdit_110->text().toInt();

    // Appeler la méthode rechercherParNumSocial et obtenir le modèle
    class patient p;
    QSqlQueryModel *model = p.rechercherParNumSocial(numSocial);

    if (model->rowCount() > 0) {
        // Associer le modèle à la vue de tableau
        ui->tableView_2->setModel(model);
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
           ui->tableView_2->setModel(model);
       } else {
           QMessageBox::information(this, "Résultat", "Aucun patient trouvé avec ce numéro social.");
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
     painter.fillRect(QRect(0, 0, pdfWriter.width(), 1000), QColor(255, 140, 0));  // Background color (orange)

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

     // Generate the QR code from the data (you will need a library like Qt's QZXing or similar for generating the QR)
   //  QImage qrImage = generateQRCode(qrCodeData);  // Assume generateQRCode is a function to create the QR image

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

            // Fetch repair data from the database using the ID Reparation
             /*  QString patientData = fetchPatientData(idPatient);
             // Fetch repair details

             if (patientData.isEmpty()) {
                QMessageBox::warning(this, "Error", "No patient data found for this Social Number.");
                return;
            }*/
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



void MainWindow::on_pushButton_ajouter_clicked()
{
    // Récupérer les informations saisies dans les champs
    QString ID_T = ui->lineEdit_id->text();
    QString DATE_TRANSACTION = ui->lineEdit_date->text();
    int MONTANT = ui->lineEdit_montant->text().toInt();
    QString TYPE = ui->lineEdit_tp->text();
    QString CATEGORIE = ui->lineEdit_cat->text();
    QString MODE_PAIEMENT = ui->lineEdit_mode->text();

    // Validation de l'ID_T (Doit commencer par T et suivi de 1 à 3 chiffres)
    QRegularExpression idRegex("^T\\d{1,3}$");
    QRegularExpressionValidator idValidator(idRegex);
    int pos = 0;

    if (idValidator.validate(ID_T, pos) != QValidator::Acceptable) {
        QMessageBox::warning(this, "Erreur", "L'ID doit être sous la forme T1, T12, ou T123.");
        return;
    }

    // Validation de la Date (Format dd/MM/yyyy)
    if (!QDate::fromString(DATE_TRANSACTION, "dd/MM/yyyy").isValid()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une date valide au format dd/MM/yyyy.");
        return;
    }

    // Validation du Montant (entre 1 et 999)
    if (MONTANT < 1 || MONTANT > 999) {
        QMessageBox::warning(this, "Erreur", "Le montant doit être un nombre entre 1 et 999.");
        return;
    }

    // Validation du Type (Doit être Revenu ou Dépense)
    if (TYPE != "Revenu" && TYPE != "Dépense") {
        QMessageBox::warning(this, "Erreur", "Le type doit être 'Revenu' ou 'Dépense'.");
        return;
    }

    // Validation de la Categorie (Doit être Achat ou Ventes)
    if (CATEGORIE != "Achat" && CATEGORIE != "Ventes") {
        QMessageBox::warning(this, "Erreur", "La catégorie doit être 'Achat' ou 'Ventes'.");
        return;
    }

    // Validation du Mode de Paiement (Doit être Espèce ou Chèque)
    if (MODE_PAIEMENT != "Espèce" && MODE_PAIEMENT != "Chèque") {
        QMessageBox::warning(this, "Erreur", "Le mode de paiement doit être 'Espèce' ou 'Chèque'.");
        return;
    }

    // Vérifier la relation entre la catégorie et le type
    if ((CATEGORIE == "Achat" && TYPE != "Dépense") || (CATEGORIE == "Ventes" && TYPE != "Revenu")) {
        QMessageBox::warning(this, "Erreur", "Si la catégorie est 'Achat', le type doit être 'Dépense'. Si la catégorie est 'Ventes', le type doit être 'Revenu'.");
        return;
    }

    // Instancier l'objet de la classe finance
    finance f(ID_T, DATE_TRANSACTION, MONTANT, TYPE, CATEGORIE, MODE_PAIEMENT);

    // Tenter d'ajouter l'élément dans la base de données
    bool test = f.ajouter();

    // Vérifier si l'ajout a réussi
    if (test) {
        ui->tableViewtr->setModel(f.afficher());
        QMessageBox::information(this, "Succès", "Ajout effectué avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Ajout non effectué.");
    }
}

void MainWindow::on_pushButton_supprimer_clicked()
{
    QString ID_T = ui->supp->text();

    // Vérification : si l'ID est vide, afficher un message d'erreur
    if (ID_T.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID.");
        return;
    }

    // Préparer la requête SQL pour vérifier si l'ID existe dans la base de données
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM FINANCE WHERE ID_T = :id");
    query.bindValue(":id", ID_T);
    if (!query.exec() || !query.next() || query.value(0).toInt() == 0) {
        QMessageBox::warning(this, "Erreur", "ID non trouvé dans la base de données.");
        return;
    }

    bool test = f.supprimer(ID_T);
    if (test) {
        ui->tableViewtr->setModel(f.afficher());
        QMessageBox::information(this, "Succès", "Suppression effectuée avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Suppression non effectuée.");
    }
}

void MainWindow::on_pushButton_modifierer_clicked()
{
    QString ID_T = ui->lineEdit_2->text();

    // Vérification si l'ID est vide
    if (ID_T.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID.");
        return;
    }

    QString attribut = ui->comboBox_MOD->currentText();
    QString nouvelleValeur = ui->lineEdit_3->text();

    if (nouvelleValeur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une nouvelle valeur.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM FINANCE WHERE ID_T = :id");
    query.bindValue(":id", ID_T);
    if (!query.exec() || !query.next() || query.value(0).toInt() == 0) {
        QMessageBox::warning(this, "Erreur", "L'ID spécifié n'existe pas.");
        return;
    }

    QString requete = QString("UPDATE FINANCE SET %1 = :valeur WHERE ID_T = :id").arg(attribut);
    query.prepare(requete);
    query.bindValue(":valeur", nouvelleValeur);
    query.bindValue(":id", ID_T);

    if (query.exec()) {
        ui->tableViewtr->setModel(f.afficher());
        QMessageBox::information(this, "Succès", "Modification réussie !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification : " + query.lastError().text());
    }
}
void MainWindow::on_pushButton_modifier_f_clicked() {
    // Récupérer l'ID depuis le champ de texte
    QString ID_T = ui->lineEdit_2->text();

    // Vérifier si l'ID est vide
    if (ID_T.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID.");
        return;
    }

    // Récupérer l'attribut à modifier et la nouvelle valeur
    QString attribut = ui->comboBox_MOD->currentText();  // Attribut à modifier
    QString nouvelleValeur = ui->lineEdit_3->text();  // Nouvelle valeur

    // Vérifier si la nouvelle valeur est vide
    if (nouvelleValeur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une nouvelle valeur.");
        return;
    }

    // Appeler la fonction modifier
    bool result = f.modifier(ID_T, attribut, nouvelleValeur);

    if (result) {
        //  modification est réussie actualiser la table
        ui->tableViewtr->setModel(f.afficher());
        QMessageBox::information(this, "Succès", "Modification effectuée avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "La modification a échoué.");
    }
}

void MainWindow::on_ACTUALISER_clicked()
{
     ui->tableViewtr->setModel(f.afficher());
}


void MainWindow::on_comboBox_trii_changed(int index)
{
    QString selectedFilter = ui->comboBox_trii->currentText();

    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    if (selectedFilter == "Montant") {
        // Tri par Montant en ordre croissant
        query.prepare("SELECT  MONTANT, TYPE, CATEGORIE, MODE_PAIEMENT FROM FINANCE ORDER BY MONTANT ASC");
    } else if (selectedFilter == "Type") {
        // Tri par Type
        query.prepare("SELECT ID_T, DATE_TRANSACTION, MONTANT, TYPE, CATEGORIE, MODE_PAIEMENT FROM FINANCE ORDER BY TYPE");
    }

    if (query.exec()) {
        model->setQuery(query);

        // Définir les en-têtes des colonnes
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Date Transaction"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Montant"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Catégorie"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Mode Paiement"));

        ui->tableViewtr->setModel(model);
        ui->tableViewtr->resizeColumnsToContents();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'application du filtre : " + query.lastError().text());
    }
}

void MainWindow::on_lineEdit_15_cursorPositionChanged(const QString& text)
{
    // Here you can adjust the search criteria. Let's assume we are searching by ID_T (which is VARCHAR)
    QString referenceRecherche = text;

    QSqlQueryModel* model = new QSqlQueryModel();

    QSqlQuery query;

    // Modify this query to search the FINANCE table
    // If you want to search by ID_T (referance in your case), you can use this
    query.prepare("SELECT ID_T, DATE_TRANSACTION, MONTANT, TYPE, CATEGORIE, MODE_PAIEMENT FROM FINANCE WHERE ID_T = :id");
    query.bindValue(":id", referenceRecherche);

    // Execute the query and update the table model
    if (query.exec())
    {
        model->setQuery(query);

        // Set headers based on your FINANCE table's structure
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_T"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Date Transaction"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Montant"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Catégorie"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Mode Paiement"));

        // Display the results in your tableViewtr
        ui->tableViewtr->setModel(model);
        ui->tableViewtr->resizeColumnsToContents();
    }
    else
    {
        // Show an error message if the query fails
        QMessageBox::critical(this, "Erreur", "Échec de la recherche : " + query.lastError().text());
    }
}
void MainWindow::on_pushButton_9_clicked() {
    QString searchID = ui->CHER->text(); // Assuming you have a QLineEdit for entering the ID

    if (searchID.isEmpty()) {
        QMessageBox::warning(this, "Recherche", "Veuillez entrer un ID pour effectuer la recherche.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM FINANCE WHERE ID_T = :id");
    query.bindValue(":id", searchID);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Échec de l'exécution de la requête : " + query.lastError().text());
        return;
    }

    // Set up a model to display the results in a QTableView
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(query);

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Résultat", "Aucun enregistrement trouvé avec cet ID.");
        delete model; // Clean up the model since it's not used
        return;
    }

    ui->tableViewtr->setModel(model); // Assuming you have a QTableView named tableViewtr
    ui->tableViewtr->resizeColumnsToContents(); // Optional: resize columns for better display
}



void MainWindow::on_generatePieChartButton_clicked() {
    finance finance;  // Create an instance of the Finance class

    // Generate the pie chart data
    QPieSeries* series = finance.generateFinanceStatistics();

    // Create a chart and add the pie series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Finance Statistics (Ventes vs Achats)");
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Create a QChartView to display the chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Embed the chart into the UI (inside frame_3)
    chartView->setParent(ui->frame_3);
    chartView->resize(ui->frame_3->size());
    chartView->show();
}

void MainWindow::on_pdf_clicked()
{
    finance f;  // Assuming 'Finance' is the class where 'genererPDF' is defined
    f.genererPDF();  // Call the PDF generation function
}

void MainWindow::on_question_returnPressed() {
    finance f;  // Assuming 'Finance' is the class where 'genererPDF' is defined

    QString userMessage = ui->question->text().trimmed();
    if (userMessage.isEmpty())
        return;
    ui->textBrowser->append("<span style='color: blue; font-weight: bold;'>Vous :</span> " + userMessage);
    QString botResponse = f.chatbot(userMessage);  // Use the finance chatbot now
    ui->textBrowser->append("<span style='color: green; font-weight: bold;'>Chatbot :</span> " + botResponse);
    ui->question->clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////tasnimmmm
///
void MainWindow::on_pushButton_ajouterr_clicked()
{
    // Récupération des informations saisies dans les champs
    QString nom = ui->lineEdit_nom->text();                // Nom du médicament
    QString famille = ui->lineEdit_famille->text();        // Famille du médicament
    QString date_fabrication = ui->lineEdit_date_fabrication->text();  // Date de fabrication
    QString date_expiration = ui->lineEdit_date_expiration->text();    // Date d'expiration
    QString seuil_min = ui->lineEdit_seuil_min->text();    // Seuil minimum
    QString vignetteStr = ui->lineEdit_vignette->text();   // Vignette du médicament
    QString dosageStr = ui->lineEdit_dosage->text();       // Dosage du médicament
    QString prixStr = ui->lineEdit_prix->text();           // Prix du médicament
    QString stockStr = ui->lineEdit_stock->text();         // Stock du médicament

    // Expression régulière pour vérifier que la vignette, dosage, prix et stock sont des entiers ou décimaux valides
    QRegularExpression nombreRegex("^\\d+$"); // Entiers positifs
    QRegularExpression prixRegex("^\\d+(\\.\\d{1,2})?$"); // Prix avec deux décimales max

    // Contrôle de saisie : Vérification que la vignette est un nombre entier
    if (!nombreRegex.match(vignetteStr).hasMatch()) {
        QMessageBox::warning(this, tr("Contrôle de saisie"),
                             tr("La vignette doit être un nombre entier positif."));
        return;
    }

    // Contrôle de saisie : Vérification que le dosage est un nombre entier
    if (!nombreRegex.match(dosageStr).hasMatch()) {
        QMessageBox::warning(this, tr("Contrôle de saisie"),
                             tr("Le dosage doit être un nombre entier positif."));
        return;
    }

    // Contrôle de saisie : Vérification que le prix est un nombre valide
    if (!prixRegex.match(prixStr).hasMatch()) {
        QMessageBox::warning(this, tr("Contrôle de saisie"),
                             tr("Le prix doit être un nombre valide (ex: 15.99)."));
        return;
    }

    // Contrôle de saisie : Vérification que le stock est un nombre entier
    if (!nombreRegex.match(stockStr).hasMatch()) {
        QMessageBox::warning(this, tr("Contrôle de saisie"),
                             tr("Le stock doit être un nombre entier positif."));
        return;
    }

    // Conversion des chaînes en entier après validation
    int vignette = vignetteStr.toInt();
    int dosage = dosageStr.toInt();
    double prix = prixStr.toDouble(); // Conversion en double pour le prix
    int stock = stockStr.toInt();     // Conversion en entier pour le stock

    // Contrôle de saisie : Vérifier que les champs obligatoires ne sont pas vides
    if (nom.isEmpty() || famille.isEmpty() || date_fabrication.isEmpty() ||
        date_expiration.isEmpty() || seuil_min.isEmpty() || vignette <= 0 || dosage <= 0 || prix <= 0 || stock < 0)
    {
        QMessageBox::warning(this, tr("Contrôle de saisie"),
                             tr("Veuillez remplir tous les champs correctement et ne laissez aucun champ vide."));
        return; // Arrêter la fonction si une erreur de saisie est détectée
    }

    // Nettoyer les dates pour éliminer les espaces supplémentaires
    date_fabrication = date_fabrication.trimmed();
    date_expiration = date_expiration.trimmed();



    // Contrôle de saisie : Vérification du format des dates (yyyy-MM-dd)


    // Contrôle de saisie : Vérification des formats de date
    QDate dateFab = QDate::fromString(date_fabrication, "yyyy-MM-dd");
    QDate dateExp = QDate::fromString(date_expiration, "yyyy-MM-dd");




    // Contrôle de saisie : Vérification de la date d'expiration
    if (dateExp < dateFab) {
        QMessageBox::warning(this, tr("Contrôle de saisie"),
                             tr("La date d'expiration doit être après la date de fabrication."));
        return;
    }

    // Instancier un objet Medicament avec les données saisies
    medicament med(nom, vignette, date_fabrication, date_expiration, seuil_min, dosage, famille,"", prix, stock);

    // Essayer d'ajouter l'objet Medicament à la base de données
    bool test = med.ajouter(); // Ajouter le médicament à la base de données

    if (test) { // Si ajout réussi
        // Mettre à jour l'affichage avec la nouvelle liste de médicaments
        ui->tableView->setModel(med.afficher());
        QMessageBox::information(nullptr, QObject::tr("Succès"),
                                 QObject::tr("Médicament ajouté avec succès.\nClick Cancel to exit."), QMessageBox::Cancel);
        QFile file("C:/Users/tasn8/Desktop/tasnim/ficher.txt");

                    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
                        qDebug() << "Impossible d'ouvrir le fichier.";
                        return;
                                  }
                    // Obtenir la date et l'heure actuelles
                               QDateTime currentDateTime = QDateTime::currentDateTime();
                               QString formattedDateTime = currentDateTime.toString("dd.MM.yyyy hh:mm:ss");

                               // Écrire la date et l'heure actuelles suivies du texte dans le fichier
                               QTextStream out(&file);
                               out<< "<font color=\"green\">Le medicament avec le nom : "<<nom <<"a ete ajoute a " << formattedDateTime<< "</font> \n" ; // endl ajoute un saut de ligne
                               file.close();
    } else { // Si l'ajout échoue
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Échec de l'ajout du médicament.\nClick Cancel to exit."), QMessageBox::Cancel);
    }
}








/*

// Méthode pour modifier un médicament
void MainWindow::on_pushButton_modifier_clicked()
{
    // Récupérer les données saisies par l'utilisateur
    int vignette = ui->lineEdit_2->text().toInt();  // L'ID du médicament (vignette)
    QString attribut = ui->comboBox_MOD->currentText();  // L'attribut à modifier (nom, dosage, etc.)
    QString nouvelleValeur = ui->lineEdit_3->text();  // La nouvelle valeur


    // Vérifier que l'ID (vignette) n'est pas vide
    if (vignette == 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID valide pour le médicament.");
        return;
    }

    // Vérifier que la nouvelle valeur n'est pas vide
    if (nouvelleValeur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer la nouvelle valeur pour l'attribut.");
        return;
    }

    // Vérifier si le médicament existe déjà dans la base de données
    medicament m;
    if (!m.existe(vignette)) {
        QMessageBox::warning(this, "Erreur", "Aucun médicament trouvé avec cette vignette.");
        return;
    }

    // Appeler la méthode modifier de la classe medicament pour effectuer la modification
    QVariant nouvelleValeurVariant = QVariant(nouvelleValeur);
    bool success = m.modifier(vignette, attribut, nouvelleValeurVariant);


    if (success) {
        // Si la modification réussit, afficher un message et rafraîchir la vue
        QMessageBox::information(this, "Succès", "Le médicament a été modifié avec succès.");
        QFile file("C:/Users/tasn8/Desktop/tasnim/ficher.txt");
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
                        qDebug() << "Impossible d'ouvrir le fichier.";
                        return;
                                  }
                    // Obtenir la date et l'heure actuelles
                               QDateTime currentDateTime = QDateTime::currentDateTime();
                               QString formattedDateTime = currentDateTime.toString("dd.MM.yyyy hh:mm:ss");

                               // Écrire la date et l'heure actuelles suivies du texte dans le fichier
                               QTextStream out(&file);
                               out << "<font color=\"yellow\">Le medicament avec l'ID : " << vignette << " a ete modifie le " << formattedDateTime << "</font> \n" ; // endl ajoute un saut de ligne

                              file.close();
        ui->tableView->setModel(m.afficher());  // Mettre à jour l'affichage des médicaments
    } else {
        // Si la modification échoue, afficher un message d'erreur
        QMessageBox::critical(this, "Erreur", "Échec de la modification du médicament.");
    }
}
*/

void MainWindow::on_pdf1_clicked()
{
    mt.exportDataToPDF();
}

void MainWindow::on_stats_clicked()
{
    QLayoutItem* item;
   while ((item = ui->stats_3->layout()->takeAt(0)) != nullptr) {
         delete item->widget();
        delete item;
    }




   QString selectedOption = ui->comboBox_3->currentText();
   QChartView *chartView = nullptr;

   if (selectedOption == "Prix") {
       // Create the pie chart based on 'etat'
       chartView = mt.createPieChartPrix();
   } else if (selectedOption == "Stock") {
       // Create the pie chart based on 'poids'
       chartView = mt.createBarChartByFamily();
   }

   // Add the chart view to the layout if it was successfully created
   if (chartView != nullptr) {
       ui->stats_3->layout()->addWidget(chartView);
   }





}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{

}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    QSqlQueryModel* sortedModel = mt.trier(arg1);

       // Assuming you have a QTableView named tableView in your MainWindow UI to display the sorted data
       ui->tableView->setModel(sortedModel);
}



void MainWindow::on_le_recherche_textChanged(const QString &arg1)
{
    QString selectedOption = ui->comboBox_2->currentText(); // Get the selected search option

    if (arg1.isEmpty() && ui->le_recherche->hasFocus() && !ui->le_recherche->hasSelectedText()) {
        ui->tableView->setModel(mt.afficher());
        ui->le_recherche->setToolTip("Entrez un critère de recherche");
        return; // Exit early if there's no input
    }

    // Call the rechercher function with the selected option and input value
    ui->tableView->setModel(mt.rechercher(selectedOption, arg1));
    ui->tableView->clearSelection();
}

void MainWindow::on_histo_clicked()
{
    QDialog *dialog = new QDialog(this);
       dialog->resize(600, 600);
       QVBoxLayout *layout = new QVBoxLayout(dialog);
        QTextEdit *textEdit = new QTextEdit(dialog);

       // Ouvrir le fichier en lecture seule
       QFile file("C:/Users/tasn8/Desktop/tasnim/ficher.txt");

       if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
           // Lire le contenu du fichier ligne par ligne et l'ajouter à QTextEdit
           QTextStream in(&file);
           while (!in.atEnd()) {
               QString line = "evenement : "+in.readLine();
               textEdit->append(line);
           }
           // Fermer le fichier
           file.close();
       } else {
           // Afficher un message d'erreur si le fichier n'a pas pu être ouvert
           qDebug() << "Erreur lors de l'ouverture du fichier.";
       }
       layout->addWidget(textEdit);
       dialog->exec();

}

void MainWindow::on_map_2_clicked()
{

}

void MainWindow::on_modifier_f_clicked()
{
    // Récupérer les données saisies par l'utilisateur
    int vignette = ui->lineEdit_2->text().toInt();  // L'ID du médicament (vignette)
    QString attribut = ui->comboBox_MOD->currentText();  // L'attribut à modifier (nom, dosage, etc.)
    QString nouvelleValeur = ui->lineEdit_3->text();  // La nouvelle valeur


    // Vérifier que l'ID (vignette) n'est pas vide
    if (vignette == 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID valide pour le médicament.");
        return;
    }

    // Vérifier que la nouvelle valeur n'est pas vide
    if (nouvelleValeur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer la nouvelle valeur pour l'attribut.");
        return;
    }

    // Vérifier si le médicament existe déjà dans la base de données
    medicament m;
    if (!m.existe(vignette)) {
        QMessageBox::warning(this, "Erreur", "Aucun médicament trouvé avec cette vignette.");
        return;
    }

    // Appeler la méthode modifier de la classe medicament pour effectuer la modification
    QVariant nouvelleValeurVariant = QVariant(nouvelleValeur);
    bool success = m.modifier(vignette, attribut, nouvelleValeurVariant);


    if (success) {
        // Si la modification réussit, afficher un message et rafraîchir la vue
        QMessageBox::information(this, "Succès", "Le médicament a été modifié avec succès.");
        QFile file("C:/Users/tasn8/Desktop/tasnim/ficher.txt");
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
                        qDebug() << "Impossible d'ouvrir le fichier.";
                        return;
                                  }
                    // Obtenir la date et l'heure actuelles
                               QDateTime currentDateTime = QDateTime::currentDateTime();
                               QString formattedDateTime = currentDateTime.toString("dd.MM.yyyy hh:mm:ss");

                               // Écrire la date et l'heure actuelles suivies du texte dans le fichier
                               QTextStream out(&file);
                               out << "<font color=\"yellow\">Le medicament avec l'ID : " << vignette << " a ete modifie le " << formattedDateTime << "</font> \n" ; // endl ajoute un saut de ligne

                              file.close();
        ui->tableView->setModel(m.afficher());  // Mettre à jour l'affichage des médicaments
    } else {
        // Si la modification échoue, afficher un message d'erreur
        QMessageBox::critical(this, "Erreur", "Échec de la modification du médicament.");
    }
}


void MainWindow::on_pushButton_3_clicked()
{

}


void MainWindow::on_upload_image_2_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Sélectionnez une image", "", "Images (*.png *.jpg *.jpeg)");
    if (filePath.isEmpty()) { return; // Aucun fichier sélectionné
    }
    // Lire le fichier image et l'encoder en base64
        QFile file(filePath); if (!file.open(QIODevice::ReadOnly)) { QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir l'image sélectionnée.");
            return;
        }
        QByteArray imageData = file.readAll();
        QString encodedImage = QString(imageData.toBase64()); // Affecter l'image encodée à un objet medicament
        QString nom = ui->lineEdit_nom->text();
        int vignette = ui->lineEdit_vignette->text().toInt();
        QString famille = ui->lineEdit_famille->text();
        QString date_fabrication = ui->lineEdit_date_fabrication->text();
        QString date_expiration = ui->lineEdit_date_expiration->text();
        QString seuil_min = ui->lineEdit_seuil_min->text();
        int dosage = ui->lineEdit_dosage->text().toInt();
        int prix = ui->lineEdit_prix->text().toDouble();
        int stock = ui->lineEdit_stock->text().toInt();
        medicament med(nom, vignette, date_fabrication, date_expiration, seuil_min, dosage, famille,encodedImage, prix, stock);
        if (med.ajouter()) {

            QMessageBox::information(this, "Succès", "Médicament ajouté avec image.");
            ui->tableView->setModel(med.afficher());
        } else {

            QMessageBox::critical(this, "Erreur", "Échec de l'ajout du médicament.");
        }
    }


// ************* encoder image base 64 *********************


QString MainWindow::encodeImageToBase64(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir l'image.");
        return "";
    }

    QByteArray imageData = file.readAll(); // stocker les datas en binaires

    file.close();
    return imageData.toBase64(); // Convertir en Base64
}


// ********** afficher image ***************

void MainWindow::on_afficher_image_clicked()
{
    int id = ui->id_image->text().toInt(); // Récupère l'ID depuis un champ d'entrée (par exemple, un QLineEdit)

    medicament e;
    QString base64Data = e.getImageById(id); // Récupère l'image en Base64 depuis la base de données

    if (base64Data.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucune image trouvée pour cet ID.");
        return;
    }

    QByteArray imageData = QByteArray::fromBase64(base64Data.toUtf8()); // Décodage Base64
    QPixmap pixmap;
    if (!pixmap.loadFromData(imageData)) {
        QMessageBox::critical(this, "Erreur", "Échec du chargement de l'image.");
        return;
    }

    ui->imm->setPixmap(pixmap.scaled(ui->imm->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation)); // Affiche l'image
}

//ajouter employe

void MainWindow::on_ajouter_4_clicked()
{
    int CIN = ui->cin_3->text().toInt();
    QString NOM_ET_PRENOM = ui->nom_3->text();
    QString ADRESSE_EMP = ui->ad_3->text();
    int TELEPHONE_EMP = ui->te_3->text().toInt();
    QString POSTE = ui->po_3->text();
    int SALAIRE = ui->sal_3->text().toInt();
    int DATE_EMBAUCHE = ui->de_3->text().toInt();
    int NUM_CONTRAT = ui->nc_3->text().toInt();


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

    ui->afficher_2->setModel(emp.afficher());
}









void MainWindow::on_supprimer_2_clicked()
{
    int CIN = ui->cin_3->text().toInt();

    // Créer une instance de la classe employe pour accéder aux méthodes
    employe E;

    // Appeler la fonction de suppression avec le CIN
    bool test = E.supprimer(CIN);

    // Vérifier si la suppression a réussi
    if (test) {
        QMessageBox::information(this, "Succès", "Suppression effectuée");

        // Mettre à jour le modèle de données pour recharger l'affichage
        QSqlQueryModel *model = E.afficher();  // Recharger les données
        ui->afficher_2->setModel(model);         // Réassigner le modèle au QTableView

        // Optionnel : Vous pouvez également actualiser d'autres vues ou éléments si nécessaire
        ui->cin_3->clear(); // Effacer les champs, par exemple
    } else {
        QMessageBox::critical(this, "Erreur", "Suppression non effectuée");
    }
    ui->afficher_2->setModel(E.afficher());
}




void MainWindow::on_modifier_4_clicked() {
    // Récupérer les valeurs des champs de l'interface
    int CIN = ui->cin_4->text().toInt();
    QString NOM_ET_PRENOM = ui->nom_4->text();
    QString ADRESSE_EMP = ui->ad_4->text();
    int TELEPHONE_EMP = ui->te_4->text().toInt();
    QString POSTE = ui->po_4->text();
    int SALAIRE = ui->sal_4->text().toInt();
    int DATE_EMBAUCHE = ui->de_3->text().toInt();
    int NUM_CONTRAT = ui->nc_3->text().toInt();

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








void MainWindow::on_pdf_4_clicked()
{
    // Vérifiez si le modèle de la table "afficher" est valide
    QAbstractItemModel *model = ui->afficher_2->model();
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


void MainWindow::on_tabWidget_3_currentChanged(int index) {
    // Vérifier si l'onglet actuellement sélectionné est celui où vous souhaitez afficher les employés
    if (index == 1) {  // Par exemple, l'onglet à l'index 1 est celui où vous voulez afficher les employés
        employe emp;
        QSqlQueryModel *model = emp.afficher();

        // Associer le modèle de données au QTableView
        ui->afficher_2->setModel(model);
    }
}

void MainWindow::on_rechercher_2_clicked()
{
    QString recherche = ui->recherche_2->text().trimmed();

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
            ui->afficher_2->setModel(model);

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




void MainWindow::on_stat_2_clicked()
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
        while ((item = ui->verticalLayout_3->takeAt(0)) != nullptr) {
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
        ui->verticalLayout_3->addWidget(chartView); // Assurez-vous que ce layout existe dans votre UI
}

void MainWindow::on_connect_2_clicked()
{
    QString CIN = ui->id_2->text().trimmed();
    QString MDP = ui->mo_2->text().trimmed();

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
            ui->tabWidget_3->setVisible(true);

            // Restaurez les onglets cachés
            ui->tabWidget_3->addTab(ui->ListeDesEmployes_2, "Liste des Employés");
            ui->tabWidget_3->addTab(ui->AjouterEmploye_2, "Ajouter Employé");
            ui->tabWidget_3->addTab(ui->ModifierEmploye_2, "Modifier Employé");
            ui->tabWidget_3->addTab(ui->Statistique_2, "Statistiques");
            ui->tabWidget_3->addTab(ui->Tri_2, "Tri");
            ui->tabWidget_3->addTab(ui->Calendrier_2, "Calendrier");
            ui->tabWidget_3->addTab(ui->cardID_2, "cardID");

            // Passer à la page "Liste des Employés"
            ui->tabWidget_3->setCurrentWidget(ui->ListeDesEmployes_2);

        } else {
            QMessageBox::warning(this, "Erreur", "Nom d'utilisateur ou mot de passe incorrect.");
        }
    } else {
        qDebug() << "Erreur SQL : " << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "Une erreur s'est produite lors de l'authentification.");
    }
}










void MainWindow::on_tri_3_clicked()
{

        employe e;
        QSqlQueryModel* model = e.afficherTriParDateEmbauche();

        // Vérifiez si le modèle a bien été créé
        if (model && model->rowCount() > 0) {
            // Remplir le QTableView avec les employés triés par date d'embauche
            ui->trie_2->setModel(model);
            QMessageBox::information(this, tr("Tri par Date d'Embauche"), tr("Les employés ont été triés par date d'embauche."), QMessageBox::Ok);
        } else {
            // Si le modèle est vide ou l'erreur s'est produite, afficher un message d'erreur
            QMessageBox::critical(this, tr("Erreur"), tr("Impossible de trier les employés ou aucun employé trouvé."));
        }


}

void MainWindow::on_tri_4_clicked()
{
    employe E;
    QSqlQueryModel* model = E.afficherTriParNom();

    // Vérifiez si le modèle a bien été créé
    if (model && model->rowCount() > 0) {
        // Remplir le QTableView avec les employés triés par nom
        ui->trie_2->setModel(model);
        QMessageBox::information(this, tr("Tri par Nom"), tr("Les employés ont été triés par ordre alphabétique."), QMessageBox::Ok);
    } else {
        // Si le modèle est vide ou l'erreur s'est produite, afficher un message d'erreur
        QMessageBox::critical(this, tr("Erreur"), tr("Impossible de trier les employés ou aucun employé trouvé."));
    }
}









void MainWindow::on_ajouter_3_clicked()
{
    int index = ui->tabWidget_3->indexOf(ui->AjouterEmploye_2); // Assurez-vous que "tab_2" est le nom exact de la page

    if (index != -1) {
        ui->tabWidget_3->setCurrentIndex(index);
    } else {
        qDebug() << "La page 'AjouterEmploye' est introuvable dans le QTabWidget.";
    }
}

void MainWindow::on_connectButton_2_clicked() {
    QString portName = "COM5";
    qint32 baudRate = 9600;

    if (arduino.connectToArduino(portName, baudRate)) {
        qDebug() << "Connexion réussie à l'Arduino !";
    } else {
        qDebug() << "Erreur de connexion à l'Arduino : " << arduino.getErrorString();
    }
}

void MainWindow::on_modifier_3_clicked()
{
    int index = ui->tabWidget_3->indexOf(ui->ModifierEmploye_2); // Vérifiez que "tab_3" est le bon nom de la page

    if (index != -1) {
        ui->tabWidget_3->setCurrentIndex(index);
    } else {
        qDebug() << "La page 'ModifierEmploye' est introuvable dans le QTabWidget.";
    }
}




void MainWindow::on_affiche_2_clicked()
{
    // Si le nom de l'objet est "tab_10"
    int index = ui->tabWidget_3->indexOf(ui->ListeDesEmployes_2);

    if (index != -1) {
        // Passer à la page ListeDesEmployes
        ui->tabWidget->setCurrentIndex(index);
    } else {
        qDebug() << "La page 'ListeDesEmployes' est introuvable dans le QTabWidget.";
    }
}


void MainWindow::on_ajouterc_2_clicked()
{
    QString cin = ui->ci_2->text().trimmed();
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer le CIN de l'employé.");
        return;
    }

    QString congeDebut = ui->congeD_2->text().trimmed();
    QString congeFin = ui->congeF_2->text().trimmed();

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

    // Vérification de la durée maximale de 15 jours
    int dureeConge = dateDebut.daysTo(dateFin) + 1; // Inclure le jour de début
    if (dureeConge > 15) {
        QMessageBox::warning(this, "Erreur", "La durée du congé ne peut pas dépasser 15 jours.");
        return;
    }

    // Vérification qu'aucun autre employé n'a les mêmes dates de congé
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT CIN FROM Employe WHERE "
                       "(CONGE_D <= :conge_fin AND CONGE_F >= :conge_debut)");
    checkQuery.bindValue(":conge_debut", congeDebut);
    checkQuery.bindValue(":conge_fin", congeFin);

    if (!checkQuery.exec()) {
        QMessageBox::critical(this, "Erreur", "Une erreur s'est produite lors de la vérification des congés.");
        qDebug() << "Erreur SQL : " << checkQuery.lastError().text();
        return;
    }

    // Si un autre employé a déjà pris ces dates
    while (checkQuery.next()) {
        QString autreCIN = checkQuery.value(0).toString();
        if (autreCIN != cin) {
            QMessageBox::warning(this, "Conflit de dates",
                                 QString("L'employé avec le CIN %1 a déjà pris ces dates.").arg(autreCIN));
            return;
        }
    }

    // Mettre à jour la base de données avec les dates de congé
    QSqlQuery query;
    query.prepare("UPDATE Employe SET CONGE_D = :conge_debut, CONGE_F = :conge_fin WHERE CIN = :cin");
    query.bindValue(":conge_debut", congeDebut);
    query.bindValue(":conge_fin", congeFin);
    query.bindValue(":cin", cin);

    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Les dates de congé ont été enregistrées.");
    } else {
        QMessageBox::critical(this, "Erreur", "Une erreur s'est produite lors de l'ajout du congé.");
        qDebug() << "Erreur SQL : " << query.lastError().text();
        qDebug() << "Requête exécutée : " << query.lastQuery();
    }
}














void MainWindow::readFromArduino(const QString &data) {
    qDebug() << "Donnée brute reçue depuis Arduino :" << data;

    QString cardID = data.trimmed();
    qDebug() << "ID après nettoyage :" << cardID;

    if (cardID.isEmpty()) {
        qDebug() << "Erreur : aucune carte détectée.";
        ui->cardIDLabel->setText("Aucune carte détectée.");
        return;
    }

    qDebug() << "Affichage ID dans le label.";
    displayCardID(cardID);
}

void MainWindow::displayCardID(const QString &data) {
    qDebug() << "Affichage ID dans le label : " << data;
    ui->cardIDLabel->setText("ID carte détectée : " + data);

    qDebug() << "Vérification de l'ID dans la base de données.";
    verifyCardID(data);
}

void MainWindow::verifyCardID(const QString &data) {
    QSqlQuery query;
    query.prepare("SELECT * FROM employe WHERE carteID = :id");
    query.bindValue(":id", data);

    qDebug() << "Exécution de la requête SQL pour ID :" << data;
    if (query.exec()) {
        if (query.next()) {
            QString employeeName = query.value("nom").toString();
            qDebug() << "ID valide. Employé trouvé :" << employeeName;
            ui->cardIDLabel->setText("ID valide : " + data + " (Employé : " + employeeName + ")");
        } else {
            qDebug() << "ID non valide :" << data;
            ui->cardIDLabel->setText("Carte non reconnue. ID : " + data);
        }
    } else {
        qDebug() << "Erreur SQL :" << query.lastError();
        ui->cardIDLabel->setText("Erreur lors de la vérification de la carte.");
    }
}

void MainWindow::onDataReceived(const QString &data) {
    qDebug() << "Signal reçu dans MainWindow :" << data;

    // Utiliser QMetaObject::invokeMethod pour garantir un accès depuis le thread principal
    QMetaObject::invokeMethod(this, [=]() {
        ui->cardIDLabel->setText("Message Arduino : " + data);
    });
}

void MainWindow::on_spprimer_clicked()
{
    // Récupérer l'ID du médicament à supprimer
    QString idStr = ui->supp->text().trimmed();  // Récupère l'ID et enlève les espaces
    bool ok;
    int ID_T = idStr.toInt(&ok);  // Convertir la chaîne en entier

    // Contrôle de saisie : Vérifier si l'ID est valide et supérieur à 0
    if (idStr.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "L'ID ne peut pas être vide.");

        return;
    }

    if (!ok || ID_T <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID valide et supérieur à 0.");
        return;
    }

    // Vérifier si le médicament avec cet ID existe dans la base de données
    medicament m;  // Créer un objet de type medicament
    if (!m.existe(ID_T)) {  // Suppose que la méthode existe(ID_T) retourne true si l'ID existe
        QMessageBox::warning(this, "Erreur", "Aucun médicament trouvé avec cet ID.");
        return;
    }

    // Si l'ID est valide, procéder à la suppression
    bool test = m.supprimer(ID_T);

    // Si la suppression est réussie
    if (test) {
        // Mettre à jour l'affichage des médicaments dans la table
        ui->tableView->setModel(m.afficher());  // Rafraîchir la vue du tableau

        // Message de succès
        QMessageBox::information(this, "Succès", "Médicament supprimé avec succès!");
        QFile file("C:/Users/tasn8/Desktop/tasnim/ficher.txt");
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
                        qDebug() << "Impossible d'ouvrir le fichier.";
                        return;
                                  }
                    // Obtenir la date et l'heure actuelles
                               QDateTime currentDateTime = QDateTime::currentDateTime();
                               QString formattedDateTime = currentDateTime.toString("dd.MM.yyyy hh:mm:ss");

                               // Écrire la date et l'heure actuelles suivies du texte dans le fichier
                               QTextStream out(&file);
                               out << "<font color=\"red\">Le medicament avec le id : " << idStr << " a ete supprime le " << formattedDateTime << "</font> \n"; // endl ajoute un saut de ligne
                              file.close();
    } else {
        // Message d'erreur si la suppression échoue
        QMessageBox::critical(this, "Erreur", "Échec de la suppression du médicament.");
    }
}


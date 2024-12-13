#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "employe.h"
#include "finance.h"
#include "patient.h"
#include "arduino.h"
#include <QMainWindow>
#include <QTableView>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "medicament.h"
#include <QtCharts/QChartView>
#include "qchartview.h"
#include <QMainWindow>
#include <QTableView>
#include <QPainter>
#include <QTableView>
#include <QHeaderView>
#include <QFileDialog>
#include <QtCharts/QPieSlice>
#include "qserialport.h"
#include "ui_mainwindow.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
        finance f;
        void on_pushButton_ajouterr_clicked();
       void on_pushButton_supprimerr_clicked();
protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private slots:
        void on_send_clicked();
        void onNetworkReply(QNetworkReply *reply);
    // Slots for each button click
    void on_employe_clicked();
    void on_employer_clicked();
    void on_materielle_clicked();
    void on_fournisseur_clicked();
    void on_facture_clicked();
  void  on_pushButton_ajouter_clicked();
  void  on_pushButton_supprimer_clicked();
  void  on_pushButton_modifierer_clicked();
  void on_pushButton_modifier_f_clicked();
  void on_ACTUALISER_clicked();
  void on_comboBox_trii_changed(int index);
  void on_lineEdit_15_cursorPositionChanged(const QString& text);
  void on_pushButton_9_clicked();
  void on_generatePieChartButton_clicked();
  void on_pdf_clicked();
  void on_question_returnPressed();


  void on_supprimer_clicked();
 void setupMailInterface();
 void on_ajout_clicked();





 void on_Modifier_clicked();



 void on_PDF_clicked();



 void on_trier_clicked();



 void on_mail_clicked();
void  on_QR_clicked();




  //tasnimm<
  void on_pdf1_clicked();

  void on_stats_clicked();

  void on_comboBox_currentTextChanged(const QString &arg1);

  void on_comboBox_currentIndexChanged(const QString &arg1);

  void on_le_recherche_textChanged(const QString &arg1);

  void on_histo_clicked();

  void on_map_2_clicked();

  void on_modifier_f_clicked();

  void on_pushButton_3_clicked();

  void on_upload_image_2_clicked();

  void on_afficher_image_clicked();


  // Slot for handling network responses
  //esra
  void on_ajouter_4_clicked();

  void on_supprimer_2_clicked();



  void verifyCardID(const QString &data);  // Vérifier l'ID dans la base de données

  void readFromArduino(const QString &data);
  void displayCardID(const QString &data);

  void on_modifier_4_clicked();

  void on_pdf_4_clicked();
  void on_tabWidget_3_currentChanged(int index);

  void on_rechercher_2_clicked();

  void on_stat_2_clicked();
  void on_connect_2_clicked();

  void on_tri_3_clicked();
  void on_tri_4_clicked();

  //void on_employe_clicked();

  void on_ajouter_3_clicked();

  void on_modifier_3_clicked();

  void on_affiche_2_clicked();

  void on_ajouterc_2_clicked();


  void on_connectButton_2_clicked();

  //void on_ard_clicked();

  void showPatientStatistics();

  void afficherPatientParNumSocial();

  void on_recherche1_clicked();
 // QString fetchRepairData(const QString &idReparation);
  void generatePDF(const QString &filePath, const QString &repairData, const QImage &qrCodeImage);
  QImage generateQRCodeImage(const QString &data);
  QString fetchPatientData(const QString &idPatient);


  void on_spprimer_clicked();

private:

  Ui::MainWindow *ui;
    QNetworkAccessManager *networkManager;
    medicament mt;
     class patient p;
      QChartView *chartView;




    // Helper function to send an SMS
    void sendSMS(QString num, QString text);
    QString encodeImageToBase64(const QString &filePath); // Fonction pour encoder une image en Base64

    //esra

    employe Emp;
    Arduino arduino;
    QSerialPort *serialPort;
    QSqlDatabase db;
    void onDataReceived(const QString &data);

    // Helper function to send an SMS


};
#endif // MAINWINDOW_H

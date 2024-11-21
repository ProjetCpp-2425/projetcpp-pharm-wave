#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "patient.h"
#include "qchartview.h"
#include <QMainWindow>
#include <QTableView>
#include <QPainter>
#include <QTableView>
#include <QHeaderView>
#include <QFileDialog>
#include <QtCharts/QPieSlice>
#include "ui_mainwindow.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *patient = nullptr);
    QChartView *chartView = nullptr;

    ~MainWindow();



private slots:
    // Slots for each button click

    void on_projet_clicked();
    void on_employer_clicked();
    void on_materielle_clicked();
    void on_fournisseur_clicked();
    void on_facture_clicked();
    void on_supprimer_clicked();
   void setupMailInterface();





    void on_ajout_clicked();





    void on_Modifier_clicked();



    void on_PDF_clicked();



    void on_trier_clicked();



    void on_mail_clicked();
   void  on_QR_clicked();





  //  void on_stat_clicked();
    void showPatientStatistics();
    void resizeEvent(QResizeEvent *event);
    void afficherPatientParNumSocial();

    void on_recherche1_clicked();
   // QString fetchRepairData(const QString &idReparation);
    void generatePDF(const QString &filePath, const QString &repairData, const QImage &qrCodeImage);
    QImage generateQRCodeImage(const QString &data);
    QString fetchPatientData(const QString &idPatient);

private:
    Ui::MainWindow *ui;
    class patient p;
};
#endif // MAINWINDOW_H

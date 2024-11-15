#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "patient.h"
#include <QMainWindow>
#include <QTableView>
#include <QPainter>
#include <QTableView>
#include <QHeaderView>
#include <QFileDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *patient = nullptr);
    ~MainWindow();

private slots:
    // Slots for each button click

    void on_projet_clicked();
    void on_employer_clicked();
    void on_materielle_clicked();
    void on_fournisseur_clicked();
    void on_facture_clicked();
    void on_supprimer_clicked();




    void on_ajout_clicked();





    void on_Modifier_clicked();



    void on_PDF_clicked();



    void on_trier_clicked();

private:
    Ui::MainWindow *ui;
    class patient p;
};
#endif // MAINWINDOW_H

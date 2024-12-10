#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "arduino.h"
#include "employe.h"
#include "qlabel.h"
#include <QMainWindow>
#include <QTabWidget>
#include <QCalendarWidget>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ajouter_2_clicked();

    void on_supprimer_clicked();



    void verifyCardID(const QString &data);  // Vérifier l'ID dans la base de données

    void readFromArduino(const QString &data);
    void displayCardID(const QString &data);

    void on_modifier_2_clicked();

    void on_pdf_clicked();
    void on_tabWidget_currentChanged(int index);

    void on_rechercher_clicked();

    void on_stat_clicked();
    void on_connect_clicked();

    void on_tri_2_clicked();
    void on_tri_clicked();

    void on_employe_clicked();

    void on_ajouter_clicked();

    void on_modifier_clicked();

    void on_affiche_clicked();

    void on_ajouterc_clicked();

    void on_connectButton_clicked();

    //void on_ard_clicked();

private:
    Ui::MainWindow *ui;
    //QCalendarWidget *Calendrier;
    Arduino arduino;
    QLabel *cardIDLabel;  // Label pour afficher l'ID de la cart
    QSerialPort *serialPort;
    QSqlDatabase db;
    void onDataReceived(const QString &data);

    //void checkCardInDatabase(const QString &cardID);
    //void sendCardID(const QString& cardID);
    //void setupSerialPort();

    employe Emp ; //ajouter un attribut a la classe mainwindow correspondant a un objet de la classe employe pour faire appel a la methode supprimer
};
#endif // MAINWINDOW_H

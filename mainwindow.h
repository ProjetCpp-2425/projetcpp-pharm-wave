#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "employe.h"
#include <QMainWindow> 
#include <QTabWidget>

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



    void on_modifier_2_clicked();

    void on_pdf_clicked();
    void on_tabWidget_currentChanged(int index);

    void on_rechercher_clicked();

    void on_stat_clicked();

private:
    Ui::MainWindow *ui;


    employe Emp ; //ajouter un attribut a la classe mainwindow correspondant a un objet de la classe employe pour faire appel a la methode supprimer
};
#endif // MAINWINDOW_H

#ifndef PATIENT_H
#define PATIENT_H
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QtCharts/QPieSeries>
#include "ui_mainwindow.h"




class patient
{
    int AGE, NUM_SOCIALE, TELEPHONE;
    QString NOM, PRENOM, GENRE, ADRESSE, E_MAIL;

public:
    patient(){};
    patient(int, int, int, QString, QString, QString, QString, QString);  // Parameterized constructor

    // Getter and Setter functions
    QString getNOM() { return NOM; }
    QString getPRENOM() { return PRENOM; }
    int getAGE() { return AGE; }
    int getTELEPHONE() { return TELEPHONE; }
    QString getGENRE() { return GENRE; }
    QString getADRESSE() { return ADRESSE; }
    QString getMAIL() { return E_MAIL; }
    int getNUM_SOCIALE() { return NUM_SOCIALE; }
    void setNOM(QString n) { NOM = n; }
    void setAGE(int &a) { AGE = a; }
    void setNUM_SOCIALE(int &s) { NUM_SOCIALE = s; }
    void setTELEPHONE(int &t) { TELEPHONE = t; }
    void setPRENOM(QString p) { PRENOM = p; }
    void setGENRE(QString g) { GENRE = g; }
    void setADRESSE(QString m) { ADRESSE = m; }
    void setE_MAIL(QString e) { E_MAIL = e; }
    bool ajout();
     QSqlQueryModel* afficher();
    bool supprimer(int );
    bool rechercher(int);
    bool update(int, int, int, QString, QString, QString, QString, QString);
    void genererPDF();
    QSqlQueryModel* trier_nom_asc();
    QSqlQueryModel* trier_nom_desc();
    QSqlQueryModel* trier_prenom_asc();
    QSqlQueryModel* trier_prenom_desc();
    QSqlQueryModel *trier_dateNaissance_asc();
    QSqlQueryModel *trier_dateNaissance_desc();
    QSqlQueryModel *trier_adresse_asc();
    QSqlQueryModel *trier_adresse_desc();
    QSqlQueryModel *trier_tel_asc();
    QSqlQueryModel *trier_tel_desc();
    QSqlQueryModel *trier_sexe_asc();
    QSqlQueryModel *trier_sexe_desc();
    QSqlQueryModel *trier_numSocial_asc();
    QSqlQueryModel *trier_numSocial_desc();
    QSqlQueryModel *trier_age_asc();
    QSqlQueryModel *trier_age_desc();

    QSqlQueryModel *trier_genre_asc();
    QSqlQueryModel *trier_genre_desc();
    QSqlQueryModel *filtrer_par_genre(const QString &genre);

  QPieSeries *generatePatientStatistics();
 QSqlQueryModel*rechercherParNumSocial(int numSocial);
 QSqlQueryModel *rechercherParEmail(const QString& email);






};

#endif // PATIENT_H

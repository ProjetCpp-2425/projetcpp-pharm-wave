#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>


class employe
{
    QString  NOM_ET_PRENOM, ADRESSE, POSTE;
    int CIN, TELEPHONE,DATE_EMBAUCHE,SALAIRE,NUM_CONTRAT;
public:
    //constructeurs
    employe(int,QString,QString,int,QString,int,int,int);
    employe(){}


    //getters
    int getCIN(){return CIN;}
    QString getNOM_ET_PRENOM(){return NOM_ET_PRENOM;}
    QString getADRESSE(){return ADRESSE;}
    int getTELEPHONE(){return TELEPHONE;}
    QString getPOSTE(){return POSTE;}
    int getSALAIRE(){return SALAIRE;}
    int getDATE_EMBAUCHE(){return DATE_EMBAUCHE;}
    int getNUM_CONTRAT(){return NUM_CONTRAT;}

    //setters
    void setCIN(int CIN){this->CIN=CIN;}
    void setNOM_ET_PRENOM(QString np){NOM_ET_PRENOM=np;}
    void setADRESSE(QString ad){ADRESSE=ad;}
    void setTÉLÉPHONE(int TELEPHONE){this->TELEPHONE=TELEPHONE;}
    void setPOSTE(QString po){POSTE=po;}
    void setSALAIRE(int SALAIRE){this->SALAIRE=SALAIRE;}
    void setDATE_EMBAUCHE(int DATE_EMBAUCHE){this->DATE_EMBAUCHE=DATE_EMBAUCHE;}
    void setNumContrat(int NUM_CONTRAT){this->NUM_CONTRAT=NUM_CONTRAT;}


    //fonctionnalité de base
    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(int);
    bool modifier();
    void exporterPDF(const QString &filePath);
    QSqlQueryModel *rechercher(int CIN);
    QMap<QString, int> statistiquesParPoste();
};

#endif // EMPLOYE_H

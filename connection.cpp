#include "connection.h"

#include <QSqlDatabase>



Connection::Connection()
{

}

bool Connection::createconnection()
{
db = QSqlDatabase::addDatabase("QODBC");
bool test=false;

db.setDatabaseName("Projet");//inserer le nom de la source de données
db.setUserName("PharmWave");//inserer nom de l'utilisateur
db.setPassword("oracle");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;

return  test;
}
void Connection::closeConnection() {db.close();}

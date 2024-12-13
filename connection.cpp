#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");


db.setDatabaseName("CPP_PROJET");//inserer le nom de la source de données --
db.setUserName("tasnim");//inserer nom de l'utilisateur --
db.setPassword("000");//inserer mot de passe de cet utilisateur -- 000

if (db.open())
test=true;





    return  test;
}

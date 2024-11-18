#ifndef SMTP_H
#define SMTP_H

#include <QObject>
#include <QSslSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QAbstractSocket>

class smtp : public QObject
{
    Q_OBJECT

public:
    explicit smtp(QObject *parent = nullptr);
    ~smtp();

    //void sendEmail( QString from,  QString to, const QString subject,  QString message);
    bool sendEmail( QString &from,  QString &to,  QString &subject,  QString &message);
private:
    bool sendCommand(QSslSocket &socket, const QString &command, const QString &expectedResponse);
};
#endif // SMTP_H

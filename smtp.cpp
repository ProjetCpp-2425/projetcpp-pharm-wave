#include "smtp.h"

smtp::smtp(QObject *parent)
    : QObject(parent)
{
}

smtp::~smtp()
{
}
bool smtp::sendEmail(QString &from, QString &to, QString &subject, QString &message)
{
    QString smtpHost = "smtp.gmail.com";
    int smtpPort = 465;

    QSslSocket socket;
    socket.connectToHostEncrypted(smtpHost, smtpPort);

    if (!socket.waitForConnected(5000)) {
        qDebug() << "Failed to connect to the SMTP server.";
        return false;
    }

    // Read the initial server greeting (220)
    if (!socket.waitForReadyRead(5000)) {
        qDebug() << "No server greeting.";
        return false;
    }
    QString serverGreeting = socket.readAll();
    qDebug() << "Server greeting:" << serverGreeting;
    if (!serverGreeting.startsWith("220")) {
        qDebug() << "Unexpected greeting response:" << serverGreeting;
        return false;
    }

    // Send EHLO command
    if (!sendCommand(socket, "EHLO localhost\r\n", "250")) {
        return false;
    }

    // Start AUTH LOGIN
    if (!sendCommand(socket, "AUTH LOGIN\r\n", "334")) {
        return false;
    }

    // Send Base64-encoded username (email address)
    QString base64Username = QString(from).toUtf8().toBase64();
    if (!sendCommand(socket, base64Username + "\r\n", "334")) {
        return false;
    }

    // Send Base64-encoded app password
    QString base64Password = QString("onfy cjcq ditd sjoe").toUtf8().toBase64();// Replace with actual App Password
    if (!sendCommand(socket, base64Password + "\r\n", "235")) {
        return false;
    }

    // Send MAIL FROM
    if (!sendCommand(socket, QString("MAIL FROM:<%1>\r\n").arg(from), "250")) {
        return false;
    }

    // Send RCPT TO
    if (!sendCommand(socket, QString("RCPT TO:<%1>\r\n").arg(to), "250")) {
        return false;
    }

    // Send DATA command
    if (!sendCommand(socket, "DATA\r\n", "354")) {
        return false;
    }

    // Send email content
    QString emailContent = QString("From: %1\r\nTo: %2\r\nSubject: %3\r\n\r\n%4\r\n.\r\n")
        .arg(from, to, subject, message);
    if (!sendCommand(socket, emailContent, "250")) {
        return false;
    }

    // Send QUIT command
    if (!sendCommand(socket, "QUIT\r\n", "221")) {
        return false;
    }

    return true;
}

bool smtp::sendCommand(QSslSocket &socket, const QString &command, const QString &expectedResponse)
{
    socket.write(command.toUtf8());

    if (!socket.waitForBytesWritten(5000)) {
        qDebug() << "Failed to write command:" << command;
        return false;
    }

    if (!socket.waitForReadyRead(5000)) {
        qDebug() << "No response for command:" << command;
        return false;
    }

    QString response = socket.readAll();
    qDebug() << "Server response:" << response;

    if (!response.startsWith(expectedResponse)) {
        qDebug() << "Unexpected response:" << response;
        return false;
    }

    return true;
}

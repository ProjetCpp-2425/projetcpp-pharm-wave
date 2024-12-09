#include "arduino.h"
#include <QDebug>

Arduino::Arduino(QObject *parent) : QObject(parent), serialPort(new QSerialPort(this)), connected(false) {
    connect(serialPort, &QSerialPort::readyRead, this, &Arduino::readFromArduino);
}

Arduino::~Arduino() {
    disconnectFromArduino();
}

bool Arduino::connectToArduino(const QString &portName, qint32 baudRate) {
    if (serialPort->isOpen()) {
        serialPort->close();
    }

    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);

    if (serialPort->open(QIODevice::ReadWrite)) {
        connected = true;
        qDebug() << "Connected to Arduino on port" << portName;
        return true;
    } else {
        qDebug() << "Failed to connect to Arduino:" << serialPort->errorString();
        connected = false;
        return false;
    }
}

void Arduino::disconnectFromArduino() {
    if (serialPort->isOpen()) {
        serialPort->close();
    }
    connected = false;
    qDebug() << "Disconnected from Arduino.";
}

void Arduino::sendCommand(const QString &command) {
    if (serialPort->isOpen() && connected) {
        serialPort->write(command.toUtf8() + '\n');  // Ajouter un saut de ligne pour terminer la commande
        qDebug() << "Command sent to Arduino:" << command;
    } else {
        qDebug() << "Failed to send command. Arduino not connected.";
    }
}

bool Arduino::isConnected() const {
    return connected;
}

void Arduino::readFromArduino() {
    if (serialPort->canReadLine()) {
        QString data = QString::fromUtf8(serialPort->readLine()).trimmed();
        emit dataReceived(data);  // Émettre un signal avec les données reçues
        qDebug() << "Data received from Arduino:" << data;
    }
}


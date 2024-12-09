#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>
#include <QString>

class Arduino : public QObject {
    Q_OBJECT

public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();

    bool connectToArduino(const QString &portName, qint32 baudRate = 9600);
    void disconnectFromArduino();
    void sendCommand(const QString &command);
    bool isConnected() const;

        //bool isConnected() const; // Vérifie si le port série est ouvert

signals:
    void dataReceived(const QString &data);  // Signal pour transmettre les données reçues

private slots:
    void readFromArduino();  // Slot pour lire les données de l'Arduino

private:
    QSerialPort *serialPort;
    bool connected;
};

#endif // ARDUINO_H


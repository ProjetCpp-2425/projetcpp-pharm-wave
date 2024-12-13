#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>

class Arduino : public QObject {
    Q_OBJECT

public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();
    Arduino();
    QString getErrorString() const;

    bool connectToArduino(const QString &portName, qint32 baudRate = 9600);
    void disconnectFromArduino();
    void sendCommand(const QString &command);
    bool isConnected() const;

        //bool isConnected() const; // Vérifie si le port série est ouvert
    //khaled
    int connect_arduino(); // permet de connecter le PC à Arduino
        int close_arduino(); // permet de femer la connexion
        int read_number_from_arduino();
        void write_to_arduino(const QString &d); // envoyer des données vers arduino
        QByteArray read_from_arduino();  //recevoir des données de la carte Arduino
        QSerialPort* getserial();   // accesseur
        QString getarduino_port_name();

        //aziz
        int connect_arduinoA(); // permet de connecter le PC à Arduino
            int close_arduinoA(); // permet de femer la connexion
            int read_number_from_arduinoA();
            void write_to_arduinoA(const QString &d); // envoyer des données vers arduino
            QByteArray read_from_arduinoA();  //recevoir des données de la carte Arduino
            QSerialPort* getserialA();   // accesseur
            QString getarduino_port_nameA();


signals:
    void dataReceived(const QString &data);  // Signal pour transmettre les données reçues

private slots:
    void readFromArduino();  // Slot pour lire les données de l'Arduino

private:
    QSerialPort *serialPort;
    bool connected;
    Arduino *arduino;
    //khaled
    QSerialPort * serial;
    static const quint16 arduino_uno_vendor_id=0x1A86;
    static const quint16 arduino_uno_producy_id=0x7523;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray data;
    //aziz
    QSerialPort * serialA; //Cet objet rassemble des informations (vitesse, bits de données, etc.)
    //et des fonctions (envoi, lecture de réception,…) sur ce qu’est une voie série pour Arduino.
    static const quint16 arduino_uno_vendor_idA=9025;
    static const quint16 arduino_uno_producy_idA=67;
    QString arduino_port_nameA;
    bool arduino_is_availableA;
    QByteArray dataA;
    // contenant les données lues à partir d'Arduino
};

#endif // ARDUINO_H


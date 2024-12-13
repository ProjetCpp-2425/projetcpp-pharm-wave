
#include "arduino.h"
#include "qglobal.h"
#include "qobject.h"
#include "qserialport.h"
#include <QDebug>

Arduino::Arduino(QObject *parent) : QObject(parent), serialPort(new QSerialPort(this)), connected(false) {
    connect(serialPort, &QSerialPort::readyRead, this, &Arduino::readFromArduino);

    //khaled
    data="";
    arduino_port_name="";
    arduino_is_available=false;
    serial=new QSerialPort;

    //aziz
    data="";
        arduino_port_nameA="";
        arduino_is_availableA=false;
        serialA=new QSerialPort;
}



bool Arduino::connectToArduino(const QString &portName, qint32 baudRate)
{
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
        qDebug() << "Données reçues depuis Arduino :" << data;
    }
}

QString Arduino::getErrorString() const {
    return serialPort->errorString();
}
Arduino::~Arduino() {
    disconnectFromArduino();
}
//khaled


QString Arduino::getarduino_port_name()
{
    return arduino_port_name;
}

QSerialPort *Arduino::getserial()
{
   return serial;
}
int Arduino::connect_arduino()
{   // recherche du port sur lequel la carte arduino identifée par  arduino_uno_vendor_id
    // est connectée
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()){
           if(serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()){
               if(serial_port_info.vendorIdentifier() == arduino_uno_vendor_id && serial_port_info.productIdentifier()
                       == arduino_uno_producy_id) {
                   arduino_is_available = true;
                   arduino_port_name=serial_port_info.portName();
               } } }
        qDebug() << "arduino_port_name is :" << arduino_port_name;
        if(arduino_is_available){ // configuration de la communication ( débit...)
            serial->setPortName(arduino_port_name);
            if(serial->open(QSerialPort::ReadWrite)){
                serial->setBaudRate(QSerialPort::Baud9600); // débit : 9600 bits/s
                serial->setDataBits(QSerialPort::Data8); //Longueur des données : 8 bits,
                serial->setParity(QSerialPort::NoParity); //1 bit de parité optionnel
                serial->setStopBits(QSerialPort::OneStop); //Nombre de bits de stop : 1
                serial->setFlowControl(QSerialPort::NoFlowControl);
                return 0;
            }
            return 1;
        }
        return -1;
}

int Arduino::close_arduino()

{

    if(serial->isOpen()){
            serial->close();
            return 0;
        }
    return 1;


}

int Arduino::read_number_from_arduino() {
    if (serial->isReadable()) {
        QByteArray rawData = serial->readAll(); // Read all available data
        QString dataString = QString::fromUtf8(rawData); // Convert to QString

        // Ensure the data is properly formatted
        dataString = dataString.trimmed(); // Remove whitespace or newline characters

        // Convert to integer
        bool ok;
        int number = dataString.toInt(&ok); // Convert to int, set ok flag

        if (ok) {
            return number; // Return the valid number
        } else {
            qDebug() << "Received data is not a valid number:" << dataString;
            return -1; // Return -1 if not a valid number
        }
    }

    qDebug() << "No data available to read.";
    return -1; // Return -1 if no data is available
}

 QByteArray Arduino::read_from_arduino()
{
    if(serial->isReadable()){
         data=serial->readAll(); //récupérer les données reçues

         return data;
    }
 }

 void Arduino::write_to_arduino(const QString &d)
 {
     if (serial->isWritable()) {

         QByteArray data = d.toUtf8();


         serial->write(data);
     } else {
         qDebug() << "ma7abechh yt7allll ";

     }
 }




 //aziz
QString Arduino::getarduino_port_nameA()
{
     return arduino_port_nameA;
}

QSerialPort *Arduino::getserialA()
{
    return serialA;
}
int Arduino::connect_arduinoA()
{   // recherche du port sur lequel la carte arduino identifée par  arduino_uno_vendor_id
     // est connectée
     foreach (const QSerialPortInfo &serial_port_infoA, QSerialPortInfo::availablePorts()){
            if(serial_port_infoA.hasVendorIdentifier() && serial_port_infoA.hasProductIdentifier()){
                if(serial_port_infoA.vendorIdentifier() == arduino_uno_vendor_idA && serial_port_infoA.productIdentifier()
                        == arduino_uno_producy_idA) {
                    arduino_is_availableA = true;
                    arduino_port_nameA=serial_port_infoA.portName();
                } } }
         qDebug() << "arduino_port_name is :" << arduino_port_nameA;
         if(arduino_is_availableA){ // configuration de la communication ( débit...)
             serialA->setPortName(arduino_port_nameA);
             if(serialA->open(QSerialPort::ReadWrite)){
                 serialA->setBaudRate(QSerialPort::Baud115200); // débit : 9600 bits/s
                 serialA->setDataBits(QSerialPort::Data8); //Longueur des données : 8 bits,
                 serialA->setParity(QSerialPort::NoParity); //1 bit de parité optionnel
                 serialA->setStopBits(QSerialPort::OneStop); //Nombre de bits de stop : 1
                 serialA->setFlowControl(QSerialPort::NoFlowControl);
                 return 0;
             }
             return 1;
         }
         return -1;
 }

 int Arduino::close_arduinoA()

 {

     if(serialA->isOpen()){
             serialA->close();
             return 0;
         }
     return 1;


 }

 int Arduino::read_number_from_arduinoA() {
     if (serialA->isReadable()) {
         QByteArray rawData = serialA->readAll(); // Read all available data
         QString dataString = QString::fromUtf8(rawData); // Convert to QString

         // Ensure the data is properly formatted
         dataString = dataString.trimmed(); // Remove whitespace or newline characters

         // Convert to integer
         bool ok;
         int number = dataString.toInt(&ok); // Convert to int, set ok flag

         if (ok) {
             return number; // Return the valid number
         } else {
             qDebug() << "Received data is not a valid number:" << dataString;
             return -1; // Return -1 if not a valid number
         }
     }

     qDebug() << "No data available to read.";
     return -1; // Return -1 if no data is available
 }

  QByteArray Arduino::read_from_arduinoA()
 {
     if(serialA->isReadable()){
          dataA=serialA->readAll(); //récupérer les données reçues

          return dataA;
     }
  }

 //hnee nb3thou f stringgggggggggggggggggggggggggg///
  void Arduino::write_to_arduinoA(const QString &d)
  {
      if (serialA->isWritable()) {

          QByteArray dataA = d.toUtf8();


          serialA->write(dataA);
      } else {
          qDebug() << "ma7abechh yt7allll ";
      }
  }

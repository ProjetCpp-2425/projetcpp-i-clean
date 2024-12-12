#include "arduino.h"
#include <QDebug>

Arduino::Arduino(QObject *parent)
    : QObject(parent),
      serial(new QSerialPort(this)),
      arduinoIsAvailable(false)
{
    connect(serial, &QSerialPort::readyRead, this, &Arduino::handleReadyRead);
}

Arduino::~Arduino()
{
    closeArduino();
}

int Arduino::connectArduino()
{
    arduinoPortName = "";

    // Search for the Arduino port
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if (info.hasVendorIdentifier() && info.hasProductIdentifier() &&
            info.vendorIdentifier() == arduinoUnoVendorId &&
            info.productIdentifier() == arduinoUnoProductId)
        {
            arduinoPortName = info.portName();
            arduinoIsAvailable = true;
            break;
        }
    }

    if (!arduinoIsAvailable)
    {
        qDebug() << "Error: Arduino not found.";
        return -1; // Arduino not available
    }

    serial->setPortName(arduinoPortName);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (!serial->open(QIODevice::ReadWrite))
    {
        qDebug() << "Error: Could not open serial port:" << arduinoPortName;
        arduinoIsAvailable = false;
        return 1; // Connection failed
    }

    qDebug() << "Successfully connected to Arduino on port:" << arduinoPortName;
    return 0; // Connection successful
}

int Arduino::closeArduino()
{
    if (serial->isOpen())
    {
        serial->close();
        qDebug() << "Disconnected from Arduino.";
        arduinoIsAvailable = false;
        return 0; // Successfully closed
    }
    return 1; // Port was not open
}

QByteArray Arduino::readFromArduino()
{
    if (serial->isReadable()) {
        return serial->readAll();
    }
    return QByteArray();
}

int Arduino::writeToArduino(const QString &data)
{
    if (serial->isWritable())
    {
        serial->write(data.toUtf8());
        return 0; // Successfully written
    }

    qDebug() << "Error: Serial port is not writable.";
    return 1; // Write failed
}

QString Arduino::getArduinoPortName() const
{
    return arduinoPortName;
}

bool Arduino::isAvailable() const
{
    return arduinoIsAvailable && serial->isOpen();
}

void Arduino::handleReadyRead()
{
    QByteArray data = serial->readAll();
    if (!data.isEmpty()) {
        emit dataReceived(data);
    }
}

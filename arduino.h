#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class Arduino : public QObject
{
    Q_OBJECT
public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();

    int connectArduino();
    int closeArduino();
    QByteArray readFromArduino();
    int writeToArduino(const QString &data);
    QString getArduinoPortName() const;
    bool isAvailable() const;

signals:
    void dataReceived(const QByteArray &data);

private slots:
    void handleReadyRead();

private:
    QSerialPort *serial;
    QString arduinoPortName;
    bool arduinoIsAvailable;

    static const quint16 arduinoUnoVendorId = 9025;
    static const quint16 arduinoUnoProductId = 67;
};

#endif // ARDUINO_H

#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSerialPortInfo>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Initialize UI components
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Port selection
    QHBoxLayout *portLayout = new QHBoxLayout();
    portComboBox = new QComboBox(this);
    connectButton = new QPushButton("Connect", this);
    portLayout->addWidget(new QLabel("Select Port:", this));
    portLayout->addWidget(portComboBox);
    portLayout->addWidget(connectButton);
    mainLayout->addLayout(portLayout);

    // Populate the combo box with available serial ports
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        portComboBox->addItem(info.portName());
    }

    // Status label for serial port
    statusLabel = new QLabel("Serial Port: Disconnected", this);
    statusLabel->setStyleSheet("font-weight: bold; color: red;");
    mainLayout->addWidget(statusLabel);

    // Data display labels
    tempLabel = new QLabel("Temperature: -- °C", this);
    horiLabel = new QLabel("Horizontal Servo: -- °", this);
    vertLabel = new QLabel("Vertical Servo: -- °", this);

    mainLayout->addWidget(tempLabel);
    mainLayout->addWidget(horiLabel);
    mainLayout->addWidget(vertLabel);

    // Spacer to push content to top
    mainLayout->addStretch();

    // Initialize serial port
    serial = new QSerialPort(this);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    // Connect signals
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::connectToSerialPort);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
    connect(serial, &QSerialPort::errorOccurred, this, &MainWindow::handleSerialError);

    // Set window properties
    setWindowTitle("LDR Tracker Monitor");
    resize(300, 200);
}

MainWindow::~MainWindow() {
    if (serial->isOpen()) {
        serial->close();
    }
}

void MainWindow::connectToSerialPort() {
    if (serial->isOpen()) {
        serial->close();
        connectButton->setText("Connect");
        statusLabel->setText("Serial Port: Disconnected");
        statusLabel->setStyleSheet("font-weight: bold; color: red;");
        return;
    }

    QString selectedPort = portComboBox->currentText();
    serial->setPortName(selectedPort);

    if (serial->open(QIODevice::ReadOnly)) {
        statusLabel->setText("Serial Port: Connected");
        statusLabel->setStyleSheet("font-weight: bold; color: green;");
        connectButton->setText("Disconnect");
    } else {
        statusLabel->setText("Serial Port: Failed to connect");
        statusLabel->setStyleSheet("font-weight: bold; color: red;");
        connectButton->setText("Connect");
    }
}

void MainWindow::readSerialData() {
    while (serial->canReadLine()) {
        QByteArray data = serial->readLine().trimmed();
        QString dataStr = QString::fromUtf8(data);
        qDebug() << "Received data:" << dataStr; // Debug output
        // Expected format: TEMP:<temp>,HORI:<horiz>,VERT:<vert>
        if (dataStr.contains("TEMP") && dataStr.contains("HORI") && dataStr.contains("VERT")) {
            // Parse the data
            QStringList parts = dataStr.split(",");
            if (parts.size() == 3) {
                // Extract temperature
                QString tempPart = parts[0].mid(parts[0].indexOf("TEMP:") + 5);
                bool tempOk;
                float temp = tempPart.toFloat(&tempOk);

                // Extract horizontal servo angle
                QString horiPart = parts[1].mid(parts[1].indexOf("HORI:") + 5);
                bool horiOk;
                int hori = horiPart.toInt(&horiOk);

                // Extract vertical servo angle
                QString vertPart = parts[2].mid(parts[2].indexOf("VERT:") + 5);
                bool vertOk;
                int vert = vertPart.toInt(&vertOk);

                // Update UI if parsing successful
                if (tempOk && horiOk && vertOk) {
                    tempLabel->setText(QString("Temperature: %1 °C").arg(temp, 0, 'f', 2));
                    horiLabel->setText(QString("Horizontal Servo: %1 °").arg(hori));
                    vertLabel->setText(QString("Vertical Servo: %1 °").arg(vert));
                }
            }
        }
    }
}

void MainWindow::handleSerialError(QSerialPort::SerialPortError error) {
    if (error != QSerialPort::NoError) {
        statusLabel->setText("Serial Port: Error occurred");
        statusLabel->setStyleSheet("font-weight: bold; color: red;");
        if (serial->isOpen()) {
            serial->close();
            connectButton->setText("Connect");
        }
    }
}

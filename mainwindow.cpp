#include "mainwindow.h"
#include "ui_mainwindow.h"

// Конструктор класса MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
      ui->setupUi(this);

    ui->startRegisterLineEdit->setValidator     (new QIntValidator(0, 65535, this));
    ui->numberOfRegistersLineEdit->setValidator (new QIntValidator(1, 125, this));

    ui->comboBoxFormat->addItem("Dec", 0);
    ui->comboBoxFormat->addItem("Hex", 1);
    ui->comboBoxFormat->addItem("Bin", 2);

    populateSerialPortList();
    populateBaudRateList();
    populateDataBitsList();
    populateParityList();
    populateStopBitsList();


    connect(ui->startRegisterLineEdit,      &QLineEdit::textChanged, this, &MainWindow::onStartRegisterChanged);
    connect(ui->numberOfRegistersLineEdit,  &QLineEdit::textChanged, this, &MainWindow::onNumberOfRegistersChanged);
    connect(ui->startRegisterLineEdit, &QLineEdit::textChanged,
            this, &MainWindow::on_startRegisterLineEdit_textChanged);

    connect(ui->numberOfRegistersLineEdit, &QLineEdit::textChanged,
            this, &MainWindow::on_numberOfRegistersLineEdit_textChanged);
    connect(&serialPort, &QSerialPort::readyRead, this, &MainWindow::onSerialPortReadyRead);

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::refreshDataTable);
}

// Деструктор класса MainWindow
MainWindow::~MainWindow() {
    delete ui;
}

// Обработчик события для элемента меню "Открыть"
void MainWindow::on_actionOpen_triggered() {
    // Код для открытия файла
}

// Обработчик события для элемента меню "Сохранить"
void MainWindow::on_actionSave_triggered() {
    // Код для сохранения файла
}

// Обработчик события для элемента меню "Выход"
void MainWindow::on_actionExit_triggered() {
    QApplication::quit();
}

// Метод для заполнения списка доступных последовательных портов
void MainWindow::populateSerialPortList() {
    ui->comboBoxPort->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->comboBoxPort->addItem(info.portName());
    }
}

// Метод для заполнения списка скорости передачи данных
void MainWindow::populateBaudRateList() {
    ui->comboBoxBaudRate->clear();
    // Добавляем стандартные скорости передачи данных для последовательного порта
    ui->comboBoxBaudRate->addItem("9600", QSerialPort::Baud9600); // Добавляем скорость 9600 бод
    // Добавьте другие стандартные скорости передачи данных, если это необходимо
    // Например, ui->comboBoxBaudRate->addItem("19200", QSerialPort::Baud19200);
}

// Метод для заполнения списка битов данных
void MainWindow::populateDataBitsList() {
    ui->comboBoxDataBits->clear();
    // Добавляем стандартные опции для количества бит данных в последовательном порту
    ui->comboBoxDataBits->addItem("8", QSerialPort::Data8); // Добавляем 8 бит данных
    // Добавьте другие опции, если это необходимо
    // Например, ui->comboBoxDataBits->addItem("7", QSerialPort::Data7);
}

// Метод для заполнения списка проверки на четность
void MainWindow::populateParityList() {
    ui->comboBoxParity->clear();
    // Здесь добавьте доступные варианты проверки на четность в комбо-бокс
    ui->comboBoxParity->addItem("None Parity");
    ui->comboBoxParity->addItem("Even Parity");
    ui->comboBoxParity->addItem("Odd Parity");
    // Добавьте другие варианты, если необходимо
}

// Метод для заполнения списка стоповых битов
void MainWindow::populateStopBitsList() {
    ui->comboBoxStopBits->clear();
    // Добавляем стандартные опции для количества стоповых бит
    ui->comboBoxStopBits->addItem("1", QSerialPort::OneStop); // Один стоповый бит
    ui->comboBoxStopBits->addItem("2", QSerialPort::TwoStop); // Два стоповых бита
    // Добавьте другие опции, если это необходимо
}

// Метод для открытия последовательного порта
void MainWindow::openPort() {
    QString portName = ui->comboBoxPort->currentText();
    int baudRate = ui->comboBoxBaudRate->currentText().toInt();
    int dataBits = ui->comboBoxDataBits->currentText().toInt();
    int stopBits = ui->comboBoxStopBits->currentText().toInt();
    int parity = ui->comboBoxParity->currentIndex();

    serialPort.setPortName(portName);
    serialPort.setBaudRate(static_cast<QSerialPort::BaudRate>(baudRate));
    serialPort.setDataBits(static_cast<QSerialPort::DataBits>(dataBits));
    serialPort.setStopBits(static_cast<QSerialPort::StopBits>(stopBits));
    serialPort.setParity(static_cast<QSerialPort::Parity>(parity));

    if (serialPort.open(QIODevice::ReadWrite)) {
        // Выводим текущие настройки порта в консоль
        qDebug() << "Port opened with settings:";
        qDebug() << "Port Name:" << portName;
        qDebug() << "Baud Rate:" << baudRate;
        qDebug() << "Data Bits:" << dataBits;
        qDebug() << "Stop Bits:" << stopBits;
        qDebug() << "Parity:" << parity;

        ui->buttonOpenPort->setEnabled(false);
        ui->buttonClosePort->setEnabled(true);
        ui->comboBoxPort->setEnabled(false);
        ui->comboBoxBaudRate->setEnabled(false);
        ui->comboBoxDataBits->setEnabled(false);
        ui->comboBoxStopBits->setEnabled(false);
        ui->comboBoxParity->setEnabled(false);
        updateTimer->start(500);
    } else {
        QMessageBox::critical(this, "Error", QString("Cannot open port! Error: %1").arg(serialPort.errorString()));
    }
}


// Обработчик события для кнопки "Открыть порт"
void MainWindow::on_buttonOpenPort_clicked() {
    openPort();
}

// Обработчик события для кнопки "Закрыть порт"
void MainWindow::on_buttonClosePort_clicked() {

    // Включаем кнопку "Открыть порт"
    ui->buttonOpenPort->setEnabled(true);

    serialPort.close();

    ui->comboBoxPort    ->setEnabled(true);
    ui->comboBoxBaudRate->setEnabled(true);
    ui->comboBoxDataBits->setEnabled(true);
    ui->comboBoxStopBits->setEnabled(true);
    ui->comboBoxParity  ->setEnabled(true);


    // Выключаем кнопку "Закрыть порт"
    ui->buttonClosePort->setEnabled(false);

    updateTimer->stop(); // Останавливаем таймер при закрытии порта

}

// Функция для отправки запроса Modbus RTU
void MainWindow::sendModbusRequest(quint8 slaveAddress, quint8 functionCode) {
    QByteArray request;

    // Добавляем адрес устройства и код функции
    request.append(slaveAddress);
    request.append(functionCode);

    // Добавляем начальный адрес и количество регистров, используя переменные класса
    request.append((startRegister >> 8) & 0xFF);    // Старший байт адреса
    request.append(startRegister & 0xFF);           // Младший байт адреса
    request.append((numberOfRegisters >> 8) & 0xFF);// Старший байт количества
    request.append(numberOfRegisters & 0xFF);       // Младший байт количества

    // Расчёт CRC
    quint16 crc = calculateCRC(request);
    request.append(crc & 0xFF);               // Младший байт CRC
    request.append((crc >> 8) & 0xFF);        // Старший байт CRC

    // Отправляем запрос
    serialPort.write(request);
}

// Функция для расчета CRC
quint16 MainWindow::calculateCRC(const QByteArray &data) {
    quint16 crc = 0xFFFF;
    for (auto byte : data) {
        crc ^= byte;
        for (uint8_t i = 0; i < 8; i++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

void MainWindow::onStartRegisterChanged(const QString &text) {
    bool ok;
    int value = text.toInt(&ok);

    // Проверяем, является ли ввод числом и находится ли он в допустимом диапазоне
    if (ok && value >= 0 && value <= 65535) {
        startRegister = static_cast<quint16>(value);
    } else {
        // Обработка неверного ввода: выводим сообщение об ошибке и сбрасываем значение
        QMessageBox::warning(this, "Invalid Input", "Start register must be a number between 0 and 65535.");
        ui->startRegisterLineEdit->setText(QString::number(startRegister));
    }
}

void MainWindow::onNumberOfRegistersChanged(const QString &text) {
    bool ok;
    int value = text.toInt(&ok);
    if (ok) {
        numberOfRegisters = static_cast<quint16>(value);
    } else {
        // Обработка ошибки, если ввод не является числом
    }
}

bool MainWindow::isValidModbusRTUResponse(const QByteArray &data) {
    const int       minimum_length = 5;
    const quint8    expected_address = 1;            // Адрес, который используется
    const quint8    expected_code_function = 4;     // Функция чтения входных регистров

    if (data.size() < minimum_length) return false;

    quint8 deviceAddress = data[0];
    quint8 functionCode = data[1];
    if (deviceAddress != expected_address || functionCode != expected_code_function) {
        return false;
    }

    quint16 receivedCrc = (static_cast<quint8>(data[data.size() - 2]) |
                            static_cast<quint8>(data[data.size() - 1]) << 8);
    QByteArray dataForCrc = data.left(data.size() - 2);
    quint16 calculatedCrc = calculateCRC(dataForCrc);

    return receivedCrc == calculatedCrc;
}


void MainWindow::on_startRegisterLineEdit_textChanged(const QString &arg1) {
    bool ok;
    int value = arg1.toInt(&ok);
    if (ok) {
        startRegister = static_cast<quint16>(value);
        // Теперь startRegister содержит новое значение
    } else {
        // Обработка неверного ввода или сброс значения
        // Здесь вы можете, например, очистить значение или установить значение по умолчанию
        startRegister = 0; // или другое значение по умолчанию
    }
}


void MainWindow::on_numberOfRegistersLineEdit_textChanged(const QString &arg1) {
    bool ok;
    int value = arg1.toInt(&ok);
    if (ok) {
        numberOfRegisters = static_cast<quint16>(value);
        // Теперь numberOfRegisters содержит новое значение
    } else {
        // Обработка неверного ввода или сброс значения
        numberOfRegisters = 0; // или другое значение по умолчанию
    }
}

QVector<quint16> MainWindow::unpackModbusRTUData(const QByteArray &data) {
    QVector<quint16> result;

    if (data.size() < 5) { // Минимальная длина: адрес (1), функция (1), количество байт (1), CRC (2)
        qDebug() << "Insufficient data length";
        return result;
    }

    QDataStream stream(data);
    stream.setByteOrder(QDataStream::BigEndian); // Modbus использует BigEndian

    quint8 deviceAddress, functionCode, byteCount;
    stream >> deviceAddress >> functionCode >> byteCount;

    if (3 + byteCount + 2 != data.size()) { // Проверка соответствия длины данных
        qDebug() << "Data length mismatch";
        return result;
    }

    for (int i = 0; i < byteCount / 2; ++i) {
        quint16 value;
        stream >> value;
        result.push_back(value);
    }

    return result;
}

void MainWindow::updateDataTable(const QVector<quint16> &data) {
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(data.size());

    for (int i = 0; i < data.size(); ++i) {
        QTableWidgetItem *addressItem = new QTableWidgetItem(QString::number(startRegister + i));
        QTableWidgetItem *valueItem;

        switch (displayFormat) {
        case 0: // десятичный
            valueItem = new QTableWidgetItem(QString::number(data[i]));
            break;
        case 1: // шестнадцатеричный
            valueItem = new QTableWidgetItem(QString::number(data[i], 16).toUpper());
            break;
        case 2: // двоичный
            valueItem = new QTableWidgetItem(QString::number(data[i], 2));
            break;
        default:
            valueItem = new QTableWidgetItem(QString::number(data[i]));
        }

        ui->tableWidget->setItem(i, 0, addressItem);
        ui->tableWidget->setItem(i, 1, valueItem);
    }
}

void MainWindow::onSerialPortReadyRead() {

    // Проверяем, открыт ли порт перед чтением данных
    if (!serialPort.isOpen()) {
        qDebug() << "The port is closed. Waiting for the port to open.";
        return;
    }

    QThread::msleep(100); // Это может быть необходимо для стабилизации потока данных
    QByteArray responseData = serialPort.readAll();

    // Проверяем, не пустой ли ответ
    if (responseData.isEmpty()) {
        qDebug() << "No data to read.";
        return;
    }

    QVector<quint16> unpackedData = unpackModbusRTUData(responseData);

    // Проверяем, что данные были успешно распакованы
    if (unpackedData.isEmpty()) {
        qDebug() << "Error unpacking data.";
        return;
    }

    // Обновляем currentData и таблицу
    currentData = unpackedData;
    updateDataTable(currentData);
}

void MainWindow::on_sendRequestButton_clicked()
{
    quint8 slaveAddress = 1; // Пример адреса устройства Modbus
    quint8 functionCode = 0x04; // Код функции для чтения регистров хранения

    sendModbusRequest(slaveAddress, functionCode);

    // Показываем сообщение с информацией о запросе
    QMessageBox::information(this, "Modbus Request Sent",
                             QString("Request with start register %1 and number of registers %2 has been sent.")
                                 .arg(startRegister).arg(numberOfRegisters));
}

void MainWindow::on_formatChanged(int format) {
    displayFormat = format;
}

void MainWindow::on_comboBoxFormat_currentIndexChanged(int index) {
    displayFormat = index;
    updateDataTable(currentData);
}

void MainWindow::refreshDataTable() {
    quint8 slaveAddress = 1;
    quint8 functionCode = 0x04;
    sendModbusRequest(slaveAddress, functionCode);

    updateDataTable(currentData);
}

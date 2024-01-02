#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QMessageBox>
#include <QTimer>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// Объявление главного класса MainWindow, который наследует QMainWindow
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
     // Конструктор класса MainWindow
    MainWindow(QWidget *parent = nullptr);
     // Деструктор класса MainWindow
    ~MainWindow();

    // Функция для отправки запроса Modbus RTU
    void sendModbusRequest(quint8 slaveAddress, quint8 functionCode);
    // Функция для расчета CRC
    quint16 calculateCRC(const QByteArray &data);

private:
    // Интерфейс главного окна
    Ui::MainWindow *ui;
    // Последовательный порт
    QSerialPort serialPort;

    // Методы для заполнения списков настроек порта
    void populateSerialPortList();
    void populateBaudRateList();
    void populateDataBitsList();
    void populateParityList();
    void populateStopBitsList();

    quint16 startRegister;
    quint16 numberOfRegisters;
    int displayFormat;

    QVector<quint16> currentData; // Хранит текущие данные для таблицы
    QTimer *updateTimer;

private slots:
    // Обработчики событий для элементов меню
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionExit_triggered();

    // Метод для открытия порта
    void openPort();

    // Методы для кнопок
    void on_buttonOpenPort_clicked();
    void on_buttonClosePort_clicked();

    // Обработчики событий для стартового и количества регистров
    void onStartRegisterChanged(const QString &text);
    void onNumberOfRegistersChanged(const QString &text);

    bool isValidModbusRTUResponse(const QByteArray &data);
    void updateDataTable(const QVector<quint16> &data);
    void refreshDataTable(); // Слот для обновления данных
    void on_formatChanged(int format);
    void onSerialPortReadyRead();

    QVector<quint16> unpackModbusRTUData(const QByteArray &data);

    void on_startRegisterLineEdit_textChanged(const QString &arg1);
    void on_numberOfRegistersLineEdit_textChanged(const QString &arg1);
    void on_sendRequestButton_clicked();
    void on_comboBoxFormat_currentIndexChanged(int index);
};
#endif // MAINWINDOW_H

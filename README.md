# Руководство пользователя для приложения ModbusMaster

Приложение ModbusMaster разработано для работы с внешними устройствами через последовательный порт RS-232, используя протокол Modbus RTU. Оно позволяет настраивать параметры порта, отправлять запросы к устройствам и отображать полученные данные. Разработан на QT 6 версии.

#1. Начало работы
Открытие порта:

Запустите приложение.
Выберите необходимый последовательный порт из списка в разделе "Список портов".
Настройте параметры порта (скорость обмена, число бит в байте, тип проверки на четность, число стоповых бит).
Нажмите кнопку "Open Port" для открытия порта.

#2. Настройка запроса:

Введите номер стартового регистра в поле "Стартовый регистр".
Укажите количество считываемых регистров в поле "Количество регистров".
Отправка запроса:

Нажмите кнопку "Send Request" для отправки запроса.(Если порт был уже открыт, то данные в таблице будут обновляться автоматически).
После отправки запроса, данные будут автоматически считаны и отображены в таблице.

#3. Работа с данными
Просмотрите данные в таблице, где отображаются номера запрошенных регистров и их значения.
Вы можете изменить формат отображения данных (двоичный, шестнадцатеричный, десятичный) с помощью выпадающего списка "Формат отображения".
Закрытие порта

#4. Для закрытия порта нажмите кнопку "Close Port". Это прекратит обмен данными и освободит порт для других приложений.
Дополнительные функции
В меню "Файл" доступны опции "Открыть" и "Сохранить" для работы с файлами(но не реализованы), а также опция "Выход" для закрытия приложения.


п.с. Программа тестировалась на витруальных Modbus Slave. 

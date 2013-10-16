## n3reader

N3Reader is a CLI tool written as Alti-2 Neptune 3 binary protocol implementation.
Coded in C++/Qt from Alexey Lobanov reverse engineered Altimaster N-series communication
protocol specification (http://sites.google.com/site/lobanovsoftware/).

usage: n3reader --port=&lt;port_name&gt; &lt;command&gt;

Currently, commands are supported:
- get_device_info - Read device model, s/n, s/w and h/w versions<br/>
- read_jumps_summary - Reads jumps summary data
- read_jumps_details - Reads the entire logbook data.

## n3reader
N3Reader - утилита командной строки, написанная в качестве реализации шифрованного
бинарного протокола обмена устройств Altimaster N-series. При разработке использована
спецификация протокола от Алексея Лобанова (http://sites.google.com/site/lobanovsoftware/).

Использование:
- Загрузите и установите [Altimaster USB Driver] (http://forum.altimaster.com/downloads.php?do=file&id=2)
- Загрузите текущую сборку [n3reader] (https://github.com/ffkirill/n3reader/releases/download/v0.0.1/n3reader.exe) 
в удобное для вас расположение (например, "Мои документы")
- Запуститу приглашение командной строки и перейдите в каталог, где сохранена n3reader (например, cd "Мои документы")
- Подключите "Нептун" к USB-порту. Если ввести n3reader без параметров, будет выведена информация о требуемых
параметрах командной строки, допустимых командах, и доступных в системе портах. Если не знаете точно, на
каком виртуальном COM-порту находится ваш Нептун, просмотрите свойства устройства "USB Serial port driver
for Altimaster N3" в диспетчере устройств Windows.
- Для чтения информации из устройства используйте команду c параметрами: "n3reader --port=ИМЯ_ПОРТА команда"
 
Доступные команды:
- get_device_info - Информация о модели устройства, серийном номере, версии ПО.
- read_jumps_summary - Итоговая информация о записанных прыжках (всего, общее время свободного падения,
время под куполом и т.д.)
- read_jumps_details - Чтение логбука

Вывод утилиты может быть перенаправлен в файл, например, следующая команда считает логбук c Neptune 3
на порту COM4, и запишет ее в файл log.txt:  
n3reader --port=COM4 read_jumps_details >> log.txt

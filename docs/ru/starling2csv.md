Использование: starling2csv [НАЗВАНИЕ ФАЙЛА .dbf] (НАСТРОЙКИ...)

    -c              Убрать мусор из всех данных (просто соединяет настройки -D, -T, и -S).
    -d              Изменить разделитель данных в файле CSV (без определении, то используется запятая). Длинность разделителя не ограничена и он может включать в себе всякий символ UTF-8.
    -D              Пропустить символы табуляции, новые линии и запятые в данных, чтобы конфликтов с часто используемыми разделителями CSV не бывало.
    -i НАЗВАНИЕ     Включить в результатах данные из файла .inf, находящегося в НАЗВАНИИ.
    -I              Включить в результатах данные из автоматически обнаруженного файла .inf (работает, если есть так же названный файл .inf в той же папке, как файл с базой данных), то есть названия поль, которые человек умеет читать, и описание базы данных.
    -l              Использовать названия поль для устраивания рядов, вместо колонн (то есть, напечатать названия поль в их собственную колонну).
    -m              Напечатать деталированные метаданные, не печатая таблицу на stdout. Возможно соединять эту настройку с -o, если тоже хочется создать файл CSV.
    -M              Вместо печатания таблицы содержания базы данных, напечатать таблицу с данными о полях базы данных (как -m, но в формате CSV). -o работает, чтобы напечатать результаты в файл.
    -o НАЗВАНИЕ     Напечатать CSV в определенный файл (всякий существующий файл под данным названием в данной папке будет удален!).
    -q              Остановить печатание в stdout метаданные базы данных (чтобы только напечатана была сама таблица данных).
    -s              Сделать маленькими буквы в первом ряде CSV (поскольку названия поль в файлах Starling обычно бывают заглавными и были бы так напечатаны).
    -S              Убрать необязательные пробелы (удалить пробелы до и после текста записей, заменить большое количество пробелов в ряде единственным пробелом).
    -T              Убрать из результатов теги формата Starling (наверное хорошая идея, если ничего необычного не сделаете с CSV).
    -v НАЗВАНИЕ     Название файла .VAR, ассоциированного с .DBF, который вы декодируете (не надо определить, если файл .VAR находится в той же папке, как .DBF, и имеет то же название, например japet.dbf и japet.var).
    -V              Пропустить автоматическое обнаружение файла .VAR, если не определена настройка -v (то есть, если не хочется декодировать japet.dbf с помощью japet.var *или другого файла .VAR*). Не надо, если определена настройка -v с действующим названием файла.
    -z              Вступить в режим «debug» (игнорировать защиту против загрузок баз данных, которые сломят программу).

Можно соединить настройки в одно слово (как -lqI).

Рекомендуемое использование в самом вероятном случае (декодировании обычной этимологической базы данных, которая находится в той же папке, как файл .var под тем же названием, на таблицу для использования в LibreOffice Calc, R или другой программе такого вида):

    starling2csv пример.dbf -cIo образец.csv

Объяснение:

* `пример.dbf`: база данных для декодирования
* `-c`: краткая форма настроек `-DTS`
    - `-D`: убрать символы, имеющие конфликты с популярными разделителями
    - `-T`: убрать теги Starling (в записях часто появляются теги для формата, как `\Iчто-то\i` — они безполезные в файле CSV, так программа пропустит такие теги)
    - `-S`: убрать бесполезные пробелы из таблицы (обычно бывают начальные и конечные пробелы в файлах Starling и обычно некоторые пробелы вместе; эта настройка исправит такие случаи)
* `-I`: использовать «человечные» названия колонн из файла .inf, включенного с базой данных, и показать информацию о базе данных на терминале
* `-o образец.csv`: напечатать результат в файл `образец.csv`
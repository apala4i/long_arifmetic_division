## Описание условия задачи
Смоделировать операцию деления действительного числа
на действительное число в форме +-m.n Е +-K, где
суммарная длина мантиссы (m+n) - до 30 значащих цифр, а
величина порядка K - до 5 цифр. Результат выдать в форме
+-0.m1 Е +-K1, где m1 - до 30 значащих цифр, а K1 - до 5 цифр
## Описание ТЗ
### Описание исходных данных:
на вход поступают две строки в формате +/-XX.XX**E**+/-AAA

X - цифры, содержащиеся в мантиссе

A - цифры, содержащиеся в порядке

E - обозначения порядка

Ограничения:

- Количество цифр в мантиссе не должно превышать 30.

- Количество цифр в порядке не должно
превышать 5.
  
- Для корректной работы строки не должны содержать некорректные символы, которые нельзя преобразовать в
вещественное число. Далее речь пойдёт о допустимых символах.
  - Первым символом допускается указание знака вещественного числа. Если знак отсутствует, то по
    умолчанию число считается положительным. Это замечание касается и знака порядка.
    
    Несколько знаков + или - в мантиссе, либо в порядке считаются некорректным вводом
    ``` Python
    +123 - Допустимый ввод
    -123 - Допустимый ввод
    123 - Допустимый ввод
    +123E+123 - Допустимый ввод
    +-123 - Недопустимый ввод
    -12-3 - Недопустимый ввод
    +123+E+123 - Недопустимый ввод
    ```
  - Указание порядка возможно с помощью символов **.** и **E**.
    
    Комбинировать эти символы разрешено, так же как и не указывать их.
    
    Дублирование этих символов рассматривается как некорректный ввод.
    ``` Python
    123.2 - Допустимый ввод, число будет обработано как (0.1232E3)
    123.2E12 - Допустимый ввод, число будет обработано как (0.1232E15)
    1232E12 - Допустимый ввод, число будет обработано как (0.1232E16)
    123 - Допустимый ввод, число будет обработано как (0.123E3)
    12E12E12 - Недопустимый ввод
    12.12.12 - Недопустимый ввод
    ```
  - Допустимо использование незначащих нулей в записи мантиссы и экспоненты, они будут
    обработаны верно, но на этапе ввода строки могут вызвать ошибку переполнения ввода.
    
    Знак следует указывать до незначащих нулей.
    ``` Python
    000000123 - Допустимый ввод, число будет обработано как (0.123E3)
    000000000000000000000000000000000012 - Недопустимый ввод, общая длина строки
    превышает максимально допустимую (STRING_SIZE)
    +0012 - Допустимый ввод, число будет обработано как (0.12E2)
    00+12 - Неопустимый ввод.
    ```
  - Максимальным значением порядка является 99999, минимальным -99999
    
  - Для указания порядка используется символ **E**, использование символа **e** не
    предусмотрено, и вызовет ошибочную ситуацию.
  - Использование нечисловых символов, кроме **.** и **E** Рассматривается как некорректный
    ввод
    ``` Python
    123kjjkasd - Недопустимый ввод
    asd..asdasdE - Недопустимый ввод
    ```
**Описание результатов программы:**

В результате выполнения программы будет выведено вещественное число в формате
  
  +/-XX.XX**E**+/-AAA

X - цифры, содержащиеся в мантиссе

A - цифры, содержащиеся в порядке

E - обозначения порядка

- Если порядок вещественного числа равен нулю, то символ **E** выводится не будет.
- Если порядок имеет знак +, то этот знак выводится не будет.

- Вещественное число имеет максимальный размер мантиссы 30 и максимальное значение
порядка 99999 (минимальное -99999). Если результат деления будет иметь более 30 цифр
  в мантиссе, это значение округляется (Если 31 цифра >= 5, то значение 30 цифры
  увеличивается на 1, иначе 31 цифра игнорируется).
  
### Описание задачи, реализуемой в программе
Программа реализует деление вещественного числа на вещественное число.

Алгоритм решения схож с алгоритмом деления "в столбик"

### Способ обращения к программе
Обращение к программе происходит через консоль, путём запуска exe файла.

исполняемый файл имеет расширение .exe, что предусматривает запуск программе в

ОС Windows

### Описание возможных аварийных ситуаций и ошибок пользователя
Часть ошибочных ситуаций описана в заголовке Описание исходных данных

Возможный вывод программы в случае ошибки:
- Error: Input error - произошёл некорректный ввод вещественного числа
- Error: exp overflow - порядок результата деления выходит за границы допустимого
- Error: division by zero - Произошло деление на ноль

## Описание внутренних структур данных

Программа содержит в себе структуру, представляющую вещественное число
``` C
typedef struct
{
    bool sign; ///< знак
    char mantis[MANTIS_SIZE]; ///< мантисса
    unsigned short mantis_len; ///< текущая длина мантиссы
    int exp; ///< значение порядка экспоненты
} float_value;
```
**sign** - является логической переменной, указывающей на знак числа.

Если значение sign == true, то число является положительным.

Если значение sign == false, то число является отрицательным.

**mantis** - является массивом символьных переменных длинной 30. 

Используется для хранения элементов мантиссы

**mantis_len** - является беззнаковой целочисленной переменной, указывающей на

текущий размер мантиссы

**exp** - является целочисленной переменной. Используется для хранения порядка.

  

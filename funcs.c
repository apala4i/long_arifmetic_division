/*!
 * @file
 * @brief файл содержит реализацию функций для операций с длинной арифметикой
 *
 * вещественные числа представлены в виде типа float_value
 */

#include "funcs.h"

/*!
 * @brief Проверка на валидность строки, которая будет транслироваться в вещественное число
 *
 * @param[in] string Строка, которая будет проверяться
 * @return true - если строка содержит корректные символы, false - если строка некорректна
 */
bool check_float_validity(char *string)
{
    // Удаление символа новой строки, если он присутствует
    if (string[strlen(string) - 1] == '\n')
    {
        string[strlen(string) - 1] = 0;
    }

    // Размер строки
    size_t size;
    size = strlen(string);

    // Обработка пустой строки
    if (size == 0)
        return false;

    // Количество знаков экспонент
    short exp_count = 0;

    // Количество знаков точки
    short dot_count = 0;

    // Допустимые символы
    char valid_symbols[] = "0123456789+-Ee.";

    for (size_t i = 0; i < size; ++i)
    {
        if (string[i] == '.')
            dot_count++;
        if (string[i] == 'E')
        {
            exp_count++;
        }
        // Проверка текущего символа не принадлежность допустимым символам
        if (strchr(valid_symbols, string[i]) == NULL)
        {
            return false;
        }

    // Проверка количества знаков точки
    }
    if (dot_count > 1)
        return false;

    // Проверка количества знаков экспоненты
    if (exp_count > 1)
        return false;
    return true;
}


/*!
 * @brief Вывод вещественного числа
 *
 * @param[in] val структура типа float_value, содержащая вещественное число
 */
void print_float(const float_value *val)
{
    // Проверка на то, что результат равен нулю
    if (val->mantis_len == 0)
    {
        printf("0");
        return;
    }

    // Определение знака результата
    if (val->sign)
        printf("+");
    else
        printf("-");

    // Вывод результата
    printf("0.");
    for (int i = 0; i < val->mantis_len; ++i)
    {
        printf("%c", val->mantis[i]);
    }

    // Вывод экспоненты, если она не равна нулю
    if (val->exp != 0)
        printf("E%d\n", val->exp);
}


/*!
 * @brief Проверка того, что в мантиссе находятся одни нули
 *
 * @param[in] arr Указатель на мантиссу
 * @param[in] size Размер мантиссы
 * @return true - если мантисса содержит одни нули, false - если это не так
 */
bool zero_result(const char* arr, const size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        if (arr[i] != '0' && arr[i] != '.')
            return false;
    }
    return true;
}


/*!
 * @brief Сравнение двух вещественных чисел исключительно по мантиссе
 *
 * @param[in] first - вещественное число типа float_value
 * @param[out] second - вещественное число типа float_value
 * @return true - если мантисса first >= мантиссы second, false - если это не так
 */
bool bigger_or_equal(const float_value *first, const float_value *second)
{
    // Сравнение двух мантисс
    if (first->mantis_len > second->mantis_len)
        return true;
    if (first->mantis_len < second->mantis_len)
        return false;

    // Пропуск одинаковых цифр
    short i = 0;
    while(first->mantis[i] == second->mantis[i] && i < first->mantis_len)
        i++;

    // i будет равно длине мантиссы только в случае, если обе мантиссы равны
    if (i == first->mantis_len)
        return true;

    if (first->mantis[i] > second->mantis[i])
        return true;
    else
        return false;
}

/*!
 * @brief Инициализация вещественного числа
 *
 * @param[in,out] val вещественное число типа float_value
 */
void float_init(float_value *val)
{
    for (size_t i = 0; i < MANTIS_SIZE; ++i)
    {
        val->mantis[i] = 0;
    }
    val->exp = 0;
    val->mantis_len = 0;
    val->sign = "+";
}

/*!
 * @brief Операция вычитания мантисс вещественного числа и целого
 * @param first[in,out] вещественное число типа float_value
 * @param second[out] вещественное число типа float_value
 * @return EXIT_SUCCESS - если операция удалась, EXIT_FAILURE - если не удалась
 * @warning Данная функция вычитает мантиссы только в том случае, если мантисса first >= мантиссы second
 */
int minus_op(float_value *first, const float_value *second)
{
    // Если мантисса первого числа меньше, чем мантисса второго, вычитание будет давать отрицательный
    // результат, что недопустимо в конкретной реализации
    if (first->mantis_len < second->mantis_len)
        return EXIT_FAILURE;

    unsigned short i = 0;

    // Проверка на одинаковые длины мантисс
    if (first->mantis_len == second->mantis_len)
    {
        // Пропуск одинаковых цифр с начала мантисс
        while (first->mantis[i] == second->mantis[i] && i < first->mantis_len)
        {
            i++;
        }

        // Если первая цифра мантиссы вещ. числа после одинаковой части мантисс меньше мантиссы второго числа,
        // то результат будет отрицательным
        if (first->mantis[i] < second->mantis[i])
            return EXIT_FAILURE;

        // Если обе мантиссы равны, то можно инициализировать мантиссу вещественного числа нулями.
        if (i == first->mantis_len)
        {
            float_init(first);
            return EXIT_SUCCESS;
        }
    }

    // Длина вещественной мантиссы
    unsigned short float_len = first->mantis_len;

    // Длинна целочисленной мантиссы
    unsigned short int_len = second->mantis_len;

    // Флаг, который показывает, что при вычитании взялась "десятка" из следующего разряда
    bool extra_take = false;

    for (i = 0; i < float_len; ++i)
    {
        // Текущая цифра из вещественной мантиссы (цифры берутся с конца)
        int float_digit = first->mantis[float_len - i - 1] - '0';
        // Текущая цифра из целочисленной мантиссы (цифры берутся с конца)
        // в случае, если длина целочисленной мантиссы меньше, чем вещественной, то берётся 0
        int int_digit = i < int_len ? second->mantis[int_len - i - 1] - '0' : 0;

        // Проверка на то, что к предыдущим разрядам прибавлялась "десятка"
        if (extra_take)
        {
            // Восполнение "десятки", если текущая цифра позволяет это сделать
            if (float_digit > 0)
            {
                float_digit -= 1;
                extra_take = false;
            }
            else
            {
                float_digit += 9;
            }
        }
        if (float_digit >= int_digit)
        {
            first->mantis[float_len - i - 1] = (char)(float_digit - int_digit) + '0';
        }
        else
        {
            // Если текущая цифра меньше, чем другая. Занимается "десятка" из последующих разрядов
            extra_take = true;
            first->mantis[float_len - i - 1] = (char)(10 + float_digit - int_digit) + '0';
        }
    }
    return EXIT_SUCCESS;
}


/*!
 * @brief Добавление цифры в конец мантиссы вещественного числа типа float_value
 * @param val[in,out] вещественное число типа float_value
 * @param element[in] элемент типа char, который необходимо вставить в конец
 * @return true - если операция вставки удалась, false - если не удалась
 * @warning возможна вставка только 1 символа цифры, для вставки числа необходимо многократно выполнить данную функцию
 */
bool float_push_back(float_value *val, const char element)
{
    if (val->mantis_len == MANTIS_SIZE)
        return false;
    val->mantis[val->mantis_len] = element;
    val->mantis_len += 1;
    return true;
}


/*!
 * @brief Удаление незначащих нулей из мантиссы вещественного числа типа float_value
 * @param val[in,out] вещественное число типа float_value
 * @return true - если операция удалась, false - если нет
 */
bool stabilisation(float_value *val)
{
    short i = 0;
    // Проверка на то, что мантисса состоит исключительно из нулей
    if (zero_result(val->mantis, val->mantis_len))
        return false;

    // Подсчёт количества незначащих нулей
    while(val->mantis[i] == '0')
    {
        i++;
    }

    // Расчёт новой длины мантиссы
    val->mantis_len -= i;

    // Сдвиг элементов
    if (i != 0)
    {
        for (unsigned short j = 0; j < val->mantis_len; ++j)
        {
            val->mantis[j] = val->mantis[j + i];
            val->mantis[j + i] = 0;
        }
    }
    return true;
}


/*!
 * @brief Преобразование строки в вещественное число типа float_value
 * @param[in] string строка, которая будет преобразовываться в вещественное число типа float_value
 * @param[in,out] val результат операции
 * @return EXIT_SUCCESS - если операция удалась, EXIT_FAILURE - если операция не удалась
 */
int turn_to_float(char *string, float_value *val)
{
    // Определение знака, если он присутствует в строке (по умолчанию +)
    if (string[0] == '+')
    {
        val->sign = true;
        string++;
    }
    else if (string[0] == '-')
    {
        val->sign = false;
        string++;
    }

    int exp_val;
    char *exp_check = NULL;
    // Проверка на наличие экспоненты в строке и её расчёт
    char *exp_begin = strchr(string, 'E');
    if (exp_begin != NULL)
    {
        // Проверка валидности расположения экспоненты и её диапазона значений
        if (strlen(exp_begin + 1) > MAX_EXP_LEN + 1)
            return EXIT_FAILURE;

        // Вычисление значений экспоненты
        exp_val = strtol(exp_begin + 1, &exp_check, 10);
        val->exp = exp_val;
        // "Удаление" значения экспоненты из строки
        *exp_begin = '\0';
    }

    // Проверка на ноль
    if (zero_result(string, strlen(string)))
    {
        return EXIT_SUCCESS;
    }

    // флаг, показывающий нахождение после точки
    bool dot_flag = false;

    // Конец строки
    size_t end;

    // Проверка на то, что числа находятся после точки
    exp_check = strstr(string, "0.");
    if (exp_check != NULL)
    {
        dot_flag = true;
        // Удаление знаков 0. "из строки"
        string += 2;
        exp_check += 2;
        while(*string == '0')
        {
            val->exp -= 1;
            string++;
        }
    }
    else
    {
        // Пропуск незначащих нулей
        while (*string == '0')
            string++;

        // Подсчёт нулей в конце мантиссы и изменение экспоненты
        end = strlen(string) - 1;
        while (string[end] == '0')
        {
            val->exp += 1;
            end -= 1;
        }
    }

    // Заполнение мантиссы
    for (size_t i = 0; i <= end; ++i)
    {
        if (string[i] == '.')
            dot_flag = true;
        if (isdigit(string[i]))
        {
            if (dot_flag)
                val->exp -= 1;
            if (val->mantis_len == MANTIS_SIZE)
            {
                return EXIT_FAILURE;
            }
            val->mantis[val->mantis_len++] = string[i];
        }
    }

    return EXIT_SUCCESS;
}

/*!
 * @brief Прибавление единицы к мантиссе вещественного числа типа float_value
 * @param[in,out] val вещественное число типа float_value
 */
void float_inc(float_value *val)
{
    // флаг, означающий переполнение предыдущего разряда
    bool extra_value = false;
    if (val->mantis[val->mantis_len - 1] == '9')
    {
        val->mantis[val->mantis_len - 1] = '0';
        extra_value = true;
    }
    else
    {
        val->mantis[val->mantis_len - 1] += 1;
        return;
    }
    int i = val->mantis_len - 2;
    while (i >= 0 && extra_value)
    {
        if (val->mantis[i] != '9')
        {
            val->mantis[i--] += 1;
            extra_value = false;
        }
        else
        {
            val->mantis[i--] = '0';
        }
    }
    if (i == -1 && extra_value)
    {
        val->exp += 1;
        for (int j = 1; j < val->mantis_len; ++i)
        {
            val->mantis[j] = val->mantis[j - 1];
        }
        val->mantis[0] = '1';
    }
}

/*!
 * @brief Операция деления двух вещественных чисел типа float_value
 * @param[in] first вещественное число типа float_value (числитель)
 * @param[in] second вещественное число типа float_value (знаменатель)
 * @param[in,out] res_value вещественное число типа float_value (результат деления)
 * @return возможны различные коды ошибок:
 * EXIT_SUCCESS - Операция успешно удалась
 *
 * DIVISION_BY_ZERO - Возникла ошибка деления на ноль
 *
 * EXP_OVERFLOW - порядок экспоненты превысил максимальное значение MAX_EXP_VALUE или меньше минимального значения
 * MIN_EXP_VALUE
 */
int divide_op(float_value *first, const float_value *second, float_value *res_value)
{
    // Проверка на принадлежность знаменателя к нулю
    if (second->mantis_len == 0)
    {
        return DIVISION_BY_ZERO;
    }

    // Проверка на принадлежность числителя к нулю
    if (first->mantis_len == 0)
    {
        return EXIT_SUCCESS;
    }


    float_init(res_value);
    res_value->exp = first->exp - second->exp;
    float_value current_value;
    float_init(&current_value);

    // Определение знака
    if ((first->sign && !second->sign) || (!first->sign && second->sign))
        res_value->sign = false;
    else
        res_value->sign = true;


    short current = 0;
    bool first_insertion = true;
    unsigned short current_index = 0;
    while (!bigger_or_equal(&current_value, second))
    {
        if (current_index < first->mantis_len)
            float_push_back(&current_value, first->mantis[current_index++]);
        else
        {
            float_push_back(&current_value, '0');
            res_value->exp -= 1;
        }
    }


    while (res_value->mantis_len < MANTIS_SIZE && !zero_result(current_value.mantis, current_value.mantis_len))
    {
        while (bigger_or_equal(&current_value, second))
        {
            minus_op(&current_value, second);
            stabilisation(&current_value);
            current++;
        }
        float_push_back(res_value, current + '0');
        current = 0;
        stabilisation(&current_value);

        if (current_index >= first->mantis_len && zero_result(current_value.mantis, current_value.mantis_len))
            break;

        first_insertion = true;
        while (!bigger_or_equal(&current_value, second))
        {
            if (first_insertion)
            {
                first_insertion = false;
            }
            else
            {
                float_push_back(res_value, '0');
            }

            if (current_index < first->mantis_len)
                float_push_back(&current_value, first->mantis[current_index++]);
            else
            {
                if (res_value->mantis_len != MANTIS_SIZE)
                    res_value->exp -= 1;
                float_push_back(&current_value, '0');
            }
            // Откидывание нуля, в случае, если остаток равен нулю и след. цифра числителя равна нулю
            stabilisation(&current_value);
        }
    }

    // Определение округления
    if (res_value->mantis_len == MANTIS_SIZE)
    {
        while (bigger_or_equal(&current_value, second))
        {
            minus_op(&current_value, second);
            stabilisation(&current_value);
            current++;
        }
        if (current >= 5)
            float_inc(res_value);
    }
    res_value->exp += res_value->mantis_len;
    if (res_value->exp > MAX_EXP_VALUE || res_value->exp < MIN_EXP_VALUE)
        return EXP_OVERFLOW;
    return EXIT_SUCCESS;
}

/*!
 * @brief считывание с потока ввода строки и её перевод в вещественное число
 * @param[in,out] val - вещественное число типа float_value
 * @return EXIT_SUCCESS - при успешном выполнении функции, EXIT_FAILURE - при некорректном вводе.
 */
int super_long_float_scan(float_value *val)
{
    // Создание буфера для чтения
    char string_value[STRING_SIZE];

    // Инициализация вещественного числа
    float_init(val);

    // Считывание строки, содержащей вещественное число
    printf("Input float value\n");
    printf("%31c|<-\r", ' ');
    fgets(string_value, STRING_SIZE, stdin);

    // Проверка строки с вещественным числом на допустимые символы
    if (!check_float_validity(string_value))
    {
        printf("INPUT ERROR\n");
        return INPUT_ERROR;
    }

    // Заполнение вещественного числа
    if (turn_to_float(string_value, val))
    {
        printf("INPUT_ERROR\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
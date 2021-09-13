/*!
 * @file
 * @brief файл, содержащий реализацию действий, связанных с длинной арифметикой
 */

#include "funcs.h"


int main()
{
    // Изменение буфера
    setbuf(stdout, NULL);

    // Инициализация первого вещественного числа
    float_value first_value;
    // Чтение и заполнение
    if (super_long_float_scan(&first_value))
    {
        printf("Error: Input Error");
        return EXIT_FAILURE;
    }

    // Инициализация второго вещественного числа
    float_value second_value;
    if (super_long_float_scan(&second_value))
    {
        printf("Error: Input Error");
        return EXIT_FAILURE;
    }

    // Инициализация структуры, содержащей результат
    float_value res;
    float_init(&res);

    // Выполнение деления и проверка на возможность деления
    switch (divide_op(&first_value, &second_value, &res))
    {
        case EXIT_SUCCESS:
        {
            printf("result of division: ");
            print_float(&res);
            break;
        }
        case DIVISION_BY_ZERO:
        {
            printf("Error: division by zero");
            return DIVISION_BY_ZERO;
        }
        case EXP_OVERFLOW:
        {
            printf("Error: exp overflow");
            return EXP_OVERFLOW;
        }
        default:
        {
            printf("Error: unknown error");
            return EXIT_FAILURE;
        }
    }
}
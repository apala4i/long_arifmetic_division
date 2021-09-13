/*!
 * @file
 * @brief заголовочный файл содержит сигнатуры и все необходимые библиотеки для
 * операции деления над вещественными числами
 */

#ifndef FUNCS_H
#define FUNCS_H
#include "defines.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#define DIVISION_BY_ZERO 3 ///< Ошибка, возникающая при делении на ноль
#define EXP_OVERFLOW 5 ///< Ошибка, возникающая при переполнении порядка экспоненты
#define INPUT_ERROR 6 ///< Ошибка, возникающая при некорректном вводе


/*!
 * @brief структура данных представляющая вещественное число
 */
typedef struct
{
    bool sign; ///< знак
    char mantis[MANTIS_SIZE]; ///< мантисса
    unsigned short mantis_len; ///< текущая длина мантиссы
    int exp; ///< значение порядка экспоненты
} float_value;

bool check_float_validity(char *string);

void print_float(const float_value *val);

bool zero_result(const char* arr, const size_t size);
bool bigger_or_equal(const float_value *first, const float_value *second);

void float_init(float_value *val);

bool float_push_back(float_value *val, const char element);
bool stabilisation(float_value *val);

void float_inc(float_value *val);
int divide_op(float_value *first, const float_value *second, float_value *res_value);
int minus_op(float_value *first, const float_value *second);

int super_long_float_scan(float_value *val);
int turn_to_float(char *string, float_value *val);

#endif
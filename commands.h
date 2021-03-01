#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <errno.h>
#include <sys/stat.h>
#include <math.h>
#include <string.h>
#include <cassert>

const int MAXLEN = 100;

enum Commands {
	CPU_PUSH = 1,//добавление через файл
	CPU_IN = 2,//добавление из консоли
	CPU_ADD = 3,//сложение
	CPU_SUB = 4,//вычитание
	CPU_DIV = 5,//деление
	CPU_MUL = 6,//умножение
	CPU_FSQRT = 7,//корень
	CPU_PUSH_DX = 8,//добавляет в стек значение из регистра DX
	CPU_POP_DX = 9,//берет из стека значение для регистра DX
	CPU_JL = 10,//меньше
	CPU_JG = 11,//больше
	CPU_JE = 12,//равно
	CPU_JN = 13,//нейтральный джамп(всегда выполняется)
	CPU_CMP = 14, //сравнение для регистра флага берет два значения из стека
	CPU_OUT = 15,//вывод на экран
	CPU_HLT = 16,//остановка процессора
	END = 17, //конец программы

};

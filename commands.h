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
	CPU_PUSH = 1,//���������� ����� ����
	CPU_IN = 2,//���������� �� �������
	CPU_ADD = 3,//��������
	CPU_SUB = 4,//���������
	CPU_DIV = 5,//�������
	CPU_MUL = 6,//���������
	CPU_FSQRT = 7,//������
	CPU_PUSH_DX = 8,//��������� � ���� �������� �� �������� DX
	CPU_POP_DX = 9,//����� �� ����� �������� ��� �������� DX
	CPU_JL = 10,//������
	CPU_JG = 11,//������
	CPU_JE = 12,//�����
	CPU_JN = 13,//����������� �����(������ �����������)
	CPU_CMP = 14, //��������� ��� �������� ����� ����� ��� �������� �� �����
	CPU_OUT = 15,//����� �� �����
	CPU_HLT = 16,//��������� ����������
	END = 17, //����� ���������

};

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
using namespace std;
#define bit_data 960

//g++ TZKIT3.cpp
int main3()
{
	FILE* fbar;

//	if ((fbar = fopen(argv[1], "rb")) == NULL) // Проверка на наличие файла
//	{
//		printf("File does not exist: %s\n", argv[1]);
//    	fbar = fopen("bar1.bmp","rb");
//	}

	// Считывание штрих-кода EAN13

	unsigned char bar_data[bit_data] = {0}; // Массив для хранения bmp-рисунка

	fseek(fbar, 10L, 0);                    // Находим поле в заголовке, в котором хранится индекс начала пиксельных данных
	long bar_offset = 0;
	fread(&bar_offset, sizeof(long), 1, fbar); 				  	// Считываем это поле
	fseek(fbar, bar_offset, 0);                             	// Переходим по его значению
	fread(bar_data, sizeof(unsigned char), bit_data, fbar); 	// Считываем пиксельные данные
	printf("bar_offset = %d",bar_offset);
	
	// ---------------------------
	// Перевод в двоичные данные

	// Подсчёт к-ва пикселей в каждом штрихе

	int barpix[bit_data] = {0};  	// Массив для хранения к-ва пикселей в штрихах штрих-кода
	int barnum = 0;				  	// Количество штрихов = 3 + 6*4 + 5 + 6*4 + 3 = 59
	int i = 0;
	int bit_cnt = 8;				// Счётчик битов
	int prev = 0;				  	// Значение предыдущего пикселя

	while (!((0xFF - bar_data[i]) & 128)) // Пропускаем ненужные пробелы перед кодом
	{
		printf("\n%d %d %d\n\n",i,(bar_data[i]) , (0xFF - bar_data[i] & 128));
		bar_data[i] <<= 1;
		--bit_cnt;
		if (!bit_cnt)             // Если байт прочитан полностью, переходим к следующему байту
		{
			++i;
			bit_cnt = 8;
		}
	}

	for (i; barnum != 60;)
	{
		if (((0xFF - bar_data[i]) & 128)==128)    // Получаем единицу
		{
			if (!prev) ++barnum;	  		// if (prev == 0) начинается новый штрих
		}
		else                      		// Получаем ноль
		{
			if (prev == 128) ++barnum; // if (prev == 1) начинается новый штрих
		}
		prev = ((0xFF - bar_data[i]) & 128);
		++barpix[barnum];		     		// Увеличиваем к-во пикселей в соответствующем штрих

		bar_data[i] <<= 1;
		--bit_cnt;
		if (bit_cnt==0)             	// Если байт прочитан полностью, переходим к следующему байту
		{
			++i;
			bit_cnt = 8;
		}
	}

	// Подсчёт к-ва модулей в штрихах

	float avrg = 0.0;

	for (barnum = 1; barnum != 60; barnum++)
	{
		avrg += barpix[barnum];
		printf("barpix = %d\n",barpix[barnum]);
	}
	avrg /=  95;
	printf("\n----------------\navrg = %f\n",avrg);

	for (barnum = 1; barnum != 60; barnum++)
	{
		for (int k = 1; k != 5; k++)
		{
			if ( ((barpix[barnum] - (k*avrg)) > -0.5*avrg) && ((barpix[barnum] - (k*avrg)) <= 0.5*avrg) )  // Округление
			{                                                                                              // и определение k
				barpix[barnum] = k;
				printf("barpix[%d] = %d\n",barnum,barpix[barnum]);
				break;
			}
		}
	}

	// Декодировка

	int A[10] = {0x0D, 0x19, 0x13, 0x3D, 0x23, 0x31, 0x2F, 0x3B, 0x37, 0x0B};
	int B[10] = {0x27, 0x33, 0x1B, 0x21, 0x1D, 0x39, 0x05, 0x11, 0x09, 0x17};
	int C[10] = {0x72, 0x66, 0x6C, 0x42, 0x5C, 0x4E, 0x50, 0x44, 0x48, 0x74};

	int result[12] = {0};  	// Массив декодированных чисел (Внимание! Сначала тут хранятся двоичные представления;
   							// после сравнения с таблицами А, В и С тут находятся десятичные декодированные данные)
	int k = 0;
	bit_cnt = 7;
	result[k] <<= 1;       	// Поскольку в таблицах все коды 7-разрядные, а байты у нас - 8-разрядные, то в начало
   							// каждого байта помещаем 0

	for (int barnum = 4; barnum != 28; barnum++) // Первые 3 штриха, а также последние 3 и средние 5 штрихов мы не учитываем
	{
		if (barnum & 1)	  // Если штрих парный - то он чёрный
		{
			for (int j = 0; j != barpix[barnum]; j++)
			{
				result[k] = (result[k] << 1) | 1;  // Т. е. добавляем единицу
				cout << "1";
				--bit_cnt;                         // А к-во бит до конца байта всё уменьшается...
				if (!bit_cnt)                      // ... пока не становится равным нулю
				{
					for (int p = 0; p != 10; p++)  // И тут начинается сравнение полученого байта с таблицами
					{
						if ((result[k] == A[p]) || (result[k] == B[p]))
						{                          // И если такой код нашелся - декодировка прошла успешно
							result[k] = p;         // Можем записать вместо кода само число, полученое из таблицы
							printf(" (%d) ",p);
						}
					}
					cout << " ";
					++k;                           // Переходим на следующий байт
					result[k] <<= 1;                // И повторяем процедуру
					bit_cnt = 7;
				}
			}
		}
		else               // Иначе штрих белый
		{
			for (int j = 0; j != barpix[barnum]; j++)
			{
				result[k] <<= 1;                   // Т. е. добавляем ноль
				cout << "0";
				--bit_cnt;                         // Всё то же самое, что и в первом случае
				if (!bit_cnt)
				{
					for (int p = 0; p != 10; p++)
					{
						if ((result[k] == A[p]) || (result[k] == B[p]))
						{
							result[k] = p;
							printf(" (%d) ",p);
						}
					}
					cout << " ";
					++k;
					result[k] <<= 1;
					bit_cnt = 7;
				}
			}
		}
	}
	for (int barnum = 33; barnum != 57; barnum++)
	{
		if (barnum & 1)	 // Напоминание: если штрих парный - то он чёрный
		{
			for (int j = 0; j != barpix[barnum]; j++)
			{
				result[k] = (result[k] << 1) | 1; // Опять же добавляем единицу
				cout << "1";
				--bit_cnt;
				if (!bit_cnt)
				{
					for (int p = 0; p != 10; p++)
					{
						if (result[k] == C[p])
						{
							result[k] = p;
							printf(" (%d) ",p);
						}
					}
					cout << " ";
					++k;
					result[k] <<= 1;
					bit_cnt = 7;
				}
			}
		}
		else               // Иначе штрих белый
		{
			for (int j = 0; j != barpix[barnum]; j++)
			{
				result[k] <<= 1;                 // Опять же добавляем ноль
				cout << "0";
				--bit_cnt;
				if (!bit_cnt)
				{
					for (int p = 0; p != 10; p++)
					{
						if (result[k] == C[p])
						{
							result[k] = p;
							printf(" (%d) ",p);
						}
					}
					cout << " ";
					++k;
					result[k] <<= 1;
					bit_cnt = 7;
				}
			}
		}
	}

	// Вывод

	cout << "\n";
	for (int k = 0; k != 12; k++)
	{
		printf("%x ",result[k]);
	}

	fclose(fbar);
	getchar();
	return 0;
}

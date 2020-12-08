#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
FILE* file = NULL;
char file_name[] = "lextual_soursecode.txt";
/*
	1. 关键词
	2. 标识符
	3. 数值
	//4. 赋值号
	5. 算符
	6. 界符
	7. 分隔符
	8. 注释符号
	9. 括号
*/
const char Keyword[6][10] = { "int", "void","if","else","while","return" };
const char Unary_Operator[11] = { '+','-','*','/','=','<','>'};//单目运算符
const char Binary_Operator[20][3] = { "==","<=",">=","!=" };
const char Boundary[20] = { ',','(',')','{','}',';','#' };
const char Anotation[3][4] = { "/*","*/","//" };
const int num_keyword = 6;
const int num_uo = 7;
const int num_bo = 4;
const int num_boundary = 7;
const int num_anotaion = 3;

int state;
enum states {
	s_num,
	s_letter,
	s_reject,
	//注释DFA状态
	s_ano1,
	s_ano2,
	s_ano3,
	s_ano4,
	s_ano5,
	s_ano6
};


//判断关键词
bool isKeyWord(char a[])
{
	int len = strlen(a);
	for (int i = 0; i < num_keyword; ++i)
	{
		if (strlen(Keyword[i]) == len)
		{
			if (strcmp(a, Keyword[i]) == 0)
				return true;
		}
	}
	return false;
}
//判断单目运算符
bool isUnaryOperator(char a)
{
	for (int i = 0; i < num_uo; ++i)
	{
		if (Unary_Operator[i] == a) return true;
	}
	return false;
}
//判断双目运算符
bool isBinaryOperator(char a[])
{
	for (int i = 0; i < num_bo; ++i)
	{
		if (strcmp(Binary_Operator[i], a) == 0)
			return true;
	}
	return false;
}
//判断界符
bool isBoundary(char a)
{
	for (int i = 0; i < num_boundary; ++i)
	{
		if (Boundary[i] == a) return true;
	}
	return false;
}
//判断注释
bool isAnotation(char a[])
{
	for (int i = 0; i < num_anotaion; ++i)
	{
		if (strcmp(a, Anotation[i]) == 0) return true;
	}
	return false;
}
//0-9
bool isNumber(char a)
{
	if (a >= '0' && a <= '9') return true;
	return false;
}
//a-z, A-Z
bool isLetter(char a)
{
	if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z')) return true;
	return false;
}

void dfa()
{
	char str[100];
	char ch;
	int point;

	ch = fgetc(file);
	bool finish = false;
	while (!finish)
	{
		point = 0;
		if (isNumber(ch))
		{
			state = s_num;
			str[point++] = ch;
			ch = fgetc(file);
			//这里也需要该一下，tmd
			while (isLetter(ch) || isNumber(ch) || ch == '.')
			{
				str[point++] = ch;
				ch = fgetc(file);
			}
			str[point] = '\0';

			//bool dfa_finish = false;
			for (int i = 0; i < point; ++i)
			{
				switch (state)
				{
					case s_num:
						//dfa随着功能扩展需要扩展，比如识别复数、科学技术etc
						if (isNumber(str[i])){
							state = s_num;
						}
						else {
							state = s_reject;
						}
						break;

					case s_letter:
						state = s_reject;
						break;

					case s_reject:
						state = s_reject;
						break;
				}				
			}
			if (state == s_num)
			{
				cout << str << " " << "常量" << endl;
			}
			else
			{
				cout << str << " " << "出错，不是常量" << endl;
			}
			point = 0;
			
		}
		if (isLetter(ch))
		{
			state = s_letter;
			str[point++] = ch;
			ch = fgetc(file);
			//这里也需要该一下，tmd
			while (isLetter(ch) || isNumber(ch))
			{
				str[point++] = ch;
				ch = fgetc(file);
			}
			str[point] = '\0';

			if (isKeyWord(str))
			{
				cout << str << " " << "关键字" << endl;	
			}
			else 
			{
				//DFA to recognize 标识符
				for (int i = 0; i < point; ++i)
				{
					switch (state)
					{
					case s_num:
						if (isNumber(str[i])) {
							state = s_num;
						}
						else {
							state = s_reject;
						}
						break;

					case s_letter:
						if (isLetter(str[i])) {
							state = s_letter;
						}
						else if (isNumber(str[i])) {
							state = s_num;
						}
						else {
							state = s_reject;
						}
						break;

					case s_reject:
						state = s_reject;
						break;
					}
				}

				if (state != s_reject)
				{
					cout << str << " " << "标识符" << endl;
				}
				else
				{
					cout << str << " " << "出错，不是标识符" << endl;
				}
			}

			point = 0;
		}
		if (isBoundary(ch))
		{
			cout << ch << " " << "界符" << endl;
			if (ch == '#')
			{

			}
			else
			{

			}
			if ((ch = fgetc(file)) == EOF)
			{
				finish = true;
				break;
			}
		}
		if (isUnaryOperator(ch))
		{
			if (ch == '/')
			{
				state = s_ano1;
				ch = fgetc(file);

				bool finish = false;
				while (!finish)
				{
					switch (state)
					{
					case s_ano1:
						if (ch == '/') {
							state = s_ano2;
							ch = fgetc(file);
						}
						else if (ch == '*') {
							state = s_ano5;
							ch = fgetc(file);
						}
						else {
							state = s_ano4;
							ch = fgetc(file);
						}
						break;

					case s_ano2:
						if (ch == '\n') {
							state = s_ano3;
							ch = fgetc(file);
						}
						else {
							state = s_ano2;
							ch = fgetc(file);
						}
						break;

					case s_ano3:
						state = s_ano3;
						finish = true;//接受态，退出循环
						break;

					case s_ano4:
						state = s_ano4;
						finish = true;//到达拒绝态，退出循环
						cout << "/ " << "单目运算符" << endl;
						break;

					case s_ano5:
						if (ch == '*') {
							state = s_ano6;
							ch = fgetc(file);
						}
						else {
							state = s_ano5;
							ch = fgetc(file);
						}
						break;

					case s_ano6:
						if (ch == '/') {
							state = s_ano3;
							ch = fgetc(file);
						}
						else {
							state = s_ano5;
							ch = fgetc(file);
						}
						break;
					}
				}
			}
			else {
				str[point++] = ch;
				if ((ch = fgetc(file)) == EOF)
				{
					finish = true;
				}
				str[point++] = ch;
				str[point] = '\0';
				if (finish == false && isBinaryOperator(str))
				{
					cout << str << " " << "双目运算符" << endl;
					//output<<4;
					ch = fgetc(file);
				}
				else
				{
					cout << str[0] << " " << "单目运算符" << endl;
				}
			}
			point = 0;
		}
		
		if (ch == ' ' || ch == '\n' || ch == '\t')
		{
			if ((ch = fgetc(file)) == EOF)
			{
				finish = true;
				break;
			}
			continue;
		}
		if (ch == EOF)
		{
			finish = true;
		}
	}
}

void readfile()
{
	file = fopen(file_name, "r+");
}

int main()
{
	readfile();
	dfa();
	return 0;
}
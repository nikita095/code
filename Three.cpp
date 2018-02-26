// GT_HelloWorldWin32.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <winuser.inl>
#include <math.h>
#include <xstring>
#include <string>
#include <iostream>
#include <stdexcept>
#include <limits>
#include <iostream>

// Global variables
static int xtext, ytext; //координаты текста для вывода
char AD[100], Output[10000];

int len,Num,NStrk=0,NByteStrk=0,NIDStrk=0;
bool flag = false;
int *Exsist;
#define IDC_MAIN_EDIT   102         // Edit box identifier

HWND hEdit;
// The main window class name.
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Kursovaya");

HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void SwitchByte(char *s);
void SwitchByteBack(char *s);
bool invalid_argument;
//объявление базового класса 
class Strk
{
protected:
	char *s;
	int size;
public:
	Strk()
	{}
	Strk(Strk &a)
	{
		StringCopy(a);
		size = a.GetSize();
		NStrk++;
	}
	Strk(char *S)
	{
		int l=strlen(S);
		s = new char[l+1];
		size = sizeof(s);
		for (int i = 0; i < l; i++)
			s[i] = S[i];
		s[l] = '\0';
	}
	Strk(char S)
	{
		s = new char[2];
		size = sizeof(s);
		s[0] = S;
		s[1] = '\0';
	}
	~Strk()
	{
		delete []s;
	}
	// функция получения последнего символа строки
	void GetLast(char &a)
	{
		a = s[strlen(s) - 1];
	}
	// проверка на равенство
	char *RetStr(){ return s; }
	bool CompareStrings(Strk &str)
	{
		int i;
		if (strlen(str.s) != strlen(this->s))
			return false;

		for (i = 0; i < strlen(this->s); i++)
		{
			if (str.s[i] != this->s[i])
				return false;
		}
		return true;
	}
	// фунцкия копирования строки
	void StringCopy(Strk &a)
	{
		int k;
		s = new char [strlen(a.s)+1];
		for (k = 0; k < strlen(a.s)+1; k++)
			s[k] = a.s[k];
		
	}
	int GetSize() { return size; }
	// вызов необходимых фунцкий
	void virtual ChangeFirstSym()
	{
		MessageBox(NULL, "Не тот тип строки", "Error", NULL);
	}
	char virtual ShowZnak()
	{
		return ' ';
	}
	void virtual DeleteZnak()
	{
		MessageBox(NULL, "Не тот тип строки", "Error", NULL);
	}
	bool operator==(Strk &str)
	{
		return CompareStrings(str);
	}
	Strk virtual &operator-(Strk &str)
	{
		return *this;
	}
	Strk virtual &operator=(Strk &str)
	{
		return *this;
	}
	int virtual GetZnak(){ return -1; }
};

// объявление дочернего класса строки идентификатора
class IDStrk: public Strk
{
public:
	IDStrk() :Strk()
	{}
	IDStrk(Strk &a)
	{
		StringCopy(a);
		size = a.GetSize();
		NIDStrk++;
	}
	IDStrk(char *S) :Strk(S)
	{
		std::string zz,NO[] = { "and", "return", "and_eq", "asm", "auto", "bitand", "bitor", "bool",
			"break","case","catch","char","class","const","const_cast","continue",
			"decltype","default","delete","do","double","dynamic_cast","else","enum","explicit","export",
			"extern","false","float","for","friend","goto","if","inline","int","long","mutable","namespace",
			"new","not","not_eq","nullptr","operator","or","or_eq","private","protected","public",
			"register","reinterpret_cast","return","short","signed","sizeof","static","static_assert",
			"static_cast","struct","switch","template","this","throw","true","try","typedef","typeid",
			"typename","union","unsigned","using","virtual","void","volatile","wchar_t","while","xor","xor_eq"};
		int l = strlen(S),f;
		s = new char[l+1]; // выделение памяти
		memset(s,'\0',l+1);
		size = sizeof(s);
		// проверка, является ли строка строкой индентификатором
		if (((S[0] <= 'Z') && (S[0] >= 'A')) || ((S[0] <= 'z') && (S[0] >= 'a')))
			f = 0;
		else
			f = 1;
		for (int i = 0; i < l; i++)
		{
			if (((S[i] <= 'Z') && (S[i] >= 'A')) || ((S[i] <= 'z') && (S[i] >= 'a')) || ((S[i] <= '9') && (S[i] >= '0'))||(S[i]=='_'))
			{
			}
			else
				f = 1;
		}
		zz = (const char*)S;
		for (int i = 0; i < 77; i++)
			if (NO[i] == zz)
				f = 1;
		if (f != 1)
		{
			for (int i = 0; i < l; i++)
				s[i] = S[i];
		}
		else 
			MessageBox(NULL,"Не строка-идентификатор! Строка не заполнена!","Error",NULL);


	}
	// показать знак
	char ShowZnak()
	{
		return 32;
	}
	// функция перевода первого символа в верхний регистр
	void ChangeFirstSym()
	{
		char string1[255];
		string1[0] = s[0];
		char c;
		__asm
		{
			MOV al, string1[0 * TYPE int]
				ADD al, -32
				MOV string1[0 * TYPE int], al
		}
		s[0] = string1[0];
	}
	// вызов фунцкции копирования строки из родительского класса
	IDStrk& operator=(Strk &str)
	{
		this->StringCopy(str);
		return *this;
	}

	IDStrk& operator-(Strk &str)
	{
		
		int i, j = 0; // Переменные цикла
		int N=0; //количество элементов после вычитания
		IDStrk tmp(s);
		char *a;
		for (i = 0; i < strlen(tmp.s); i++)
			for (j = 0; j < strlen(str.RetStr()); j++)
				if (tmp.s[i] == str.RetStr()[j])
					tmp.s[i] = 1;
		delete []s;
		for (i = 0; i < strlen(tmp.s); i++)
			if (tmp.s[i] != 1)
				N++;
		j = 0;
		s = new char[N+1];
		for (i = 0; i < strlen(tmp.s); i++)
		{
			if (tmp.s[i] != 1)
				s[j++] = tmp.s[i];
		}
		s[N] = '\0';
		return *this;
	}
	// вызов фунцкии копирования из родительского класса
	bool operator==(Strk &str)
	{
		return CompareStrings(str);
	}

};
// обявление дочернего класса битовая строка
class ByteStrk : public Strk
{
private:
	//int znak;//если "1", то плюс, если "0", то минус
public:
	ByteStrk() :Strk()
	{}
	ByteStrk(Strk &a)
	{
		StringCopy(a);
		size = a.GetSize();
		NByteStrk++;
		//znak = a.GetZnak();
	}
	ByteStrk(char *S, int z) :Strk(S)
	{
		int l = strlen(S), f=0;
		s = new char[12];
		memset(s, '0', 11);
		s[11] = '\0';
		size = sizeof(s);
		if (z == 0)
			s[0] = '0';
		else
			s[0] = '1';
		// проверка соответствия строки битовому типу
		if (l > 10)
			l = 10;
		for (int i = 0; i < l; i++)
		{
			if ((S[i] == '1') || (S[i] == '0'))
			{
			}
			else
				throw 1;
		}
		for (int i = 0; i < l; i++)
			s[i + 11 - l] = S[i];
		if (s[0] == '1')
		{			
			SwitchByte(s);
		}
	}
	// функция удаления знака
	void DeleteZnak()
	{
		if (GetZnak() == '1')
			SwitchByteBack(s);
		else
			MessageBox(NULL, "Число положительное!", "Error", NULL);
	}
	// вызов функции копирования строки со знаком из родительского класса
	ByteStrk& operator=(Strk &str)
	{
		this->StringCopy(str);
		s[0] = str.GetZnak();//?
		return *this;
	}
	int GetZnak(){ return s[0]; }
	ByteStrk& operator-(Strk &str)
	{
		int i=0, j = 0,a,b,ad=0,bd=0,znak1,znak2; // Переменные цикла
		if (GetZnak() == '0')
		{
			znak1 = 0;
		}
		else
		{
			SwitchByteBack(s);
			znak1 = 1;
		}
		if (str.GetZnak() == '0')
		{
			znak2 = 0;
		}
		else
		{
			SwitchByteBack(str.RetStr());
			znak2 = 1;
		}
		//ByteStrk tmp(s,znak);
		char *news,*newS;
		news = new char[strlen(s)];
		newS = new char[strlen(str.RetStr())];
		for (int ij = 0; ij < strlen(s); ij++)
			news[ij] = s[ij + 1];
		for (int ij = 0; ij < strlen(str.RetStr()); ij++)
			newS[ij] = str.RetStr()[ij + 1];
		if (znak2==1)
			SwitchByte(str.RetStr());

		sscanf_s(news, "%ui", &a);
		sscanf_s(newS, "%ui", &b);
		// способ вычитания битовых строк посредством перевода их
		// в дестятичное представление и обратно
		while (a != 0)
		{
			ad += (a%10)*pow(2.0, i);
			i++;
			a /= 10;
		}
		if (znak1 == 1)
			ad *= -1;

		while (b != 0)
		{
			bd += (b%10)*pow(2.0, j);
			j++;
			b /= 10;
		}
		if (znak2 == 1)
			bd *= -1;
		ad = ad - bd;
		if (ad < 0)
		{
			znak1 = 1;
			ad *= -1;
		}
		else
			znak1 = 0;
		a = 0;
		bd = ad;
		i = 0;
		while (bd != 0)
		{
			i++;
			bd /= 2;
		}
		j = i;
		i = 0;
		while (ad != 0)
		{
			a += (ad % 2)*pow(10.0,i);
			i++;
			ad /= 2;
		}
		delete []s;
		s = new char[j + 1];
		sprintf_s(AD, "%i", a);
		
		ByteStrk k(AD,znak1);
		StringCopy(k);
		return *this;
	}
	ByteStrk& operator+(Strk &str)
	{
		int i = 0, j = 0, a, b, ad = 0, bd = 0, znak1, znak2; // Переменные цикла
		if (GetZnak() == '0')
		{
			znak1 = 0;
		}
		else
		{
			SwitchByteBack(s);
			znak1 = 1;
		}
		if (str.GetZnak() == '0')
		{
			znak2 = 0;
		}
		else
		{
			SwitchByteBack(str.RetStr());
			znak2 = 1;
		}
		//ByteStrk tmp(s,znak);
		char *news, *newS;
		news = new char[strlen(s)];
		newS = new char[strlen(str.RetStr())];
		for (int ij = 0; ij < strlen(s); ij++)
			news[ij] = s[ij + 1];
		for (int ij = 0; ij < strlen(str.RetStr()); ij++)
			newS[ij] = str.RetStr()[ij + 1];

		sscanf_s(news, "%ui", &a);
		sscanf_s(newS, "%ui", &b);
		// способ вычитания битовых строк посредством перевода их
		// в дестятичное представление и обратно
		while (a != 0)
		{
			ad += (a % 10)*pow(2.0, i);
			i++;
			a /= 10;
		}
		if (znak1 == 1)
			ad *= -1;

		while (b != 0)
		{
			bd += (b % 10)*pow(2.0, j);
			j++;
			b /= 10;
		}
		if (znak2 == 1)
			bd *= -1;
		ad = ad + bd;
		if (ad < 0)
		{
			znak1 = 1;
			ad *= -1;
		}
		else
			znak1 = 0;
		a = 0;
		bd = ad;
		i = 0;
		while (bd != 0)
		{
			i++;
			bd /= 2;
		}
		j = i;
		i = 0;
		while (ad != 0)
		{
			a += (ad % 2)*pow(10.0, i);
			i++;
			ad /= 2;
		}
		delete[]s;
		s = new char[j + 1];
		sprintf_s(AD, "%i", a);

		ByteStrk k(AD, znak1);
		StringCopy(k);
		return *this;
	}
	// вызов функции сравнения битовых строк
	bool operator==(Strk &str)
	{
		return CompareStrings(str);
	}
	// функция показать знак
	char ShowZnak()
	{
		if (s[0] == '0')
			return ' ';
		else
			return 45;
	}
};

void SwitchByte(char *s)
{
	char s2[11];
	for (int i = 0; i < 11; i++)
		s2[i] = s[i + 1];
	ByteStrk S(s2, 0), s1("1", 0);
	S - s1;
	for (int i = 0; i < strlen(S.RetStr())+1; i++)
	{
		if (S.RetStr()[i] == '0')
			S.RetStr()[i] = '1';
		else if (S.RetStr()[i] == '1')
			S.RetStr()[i] = '0';
	}
	for (int i = 0; i < strlen(S.RetStr())+1; i++)
		s[i] = S.RetStr()[i];
}
void SwitchByteBack(char *s)
{
	char s2[11];
	for (int i = 0; i < 11; i++)
		s2[i] = s[i + 1];
	ByteStrk S(s2, 0), s1("1", 0);
	for (int i = 1; i < strlen(S.RetStr()) + 1; i++)
	{
		if (S.RetStr()[i] == '0')
			S.RetStr()[i] = '1';
		else if (S.RetStr()[i] == '1')
			S.RetStr()[i] = '0';
	}
	S + s1;
	for (int i = 0; i < strlen(S.RetStr()) + 1; i++)
		s[i] = S.RetStr()[i];
}

Strk **Massive;
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	hInst = hInstance;

	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1000, 500,
		NULL,
		NULL,
		hInstance,
		NULL
		),hText;

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}
	HWND hWndButton1 = CreateWindowEx(NULL,
		_T("BUTTON"),
		_T("Инициализация"),
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD,
		20,
		20,
		120, 30,
		hWnd,
		(HMENU)10006,
		GetModuleHandle(NULL),
		NULL);
	HWND hWndButton2 = CreateWindowEx(NULL,
		_T("BUTTON"),
		_T("Тестирование"),
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD,
		20,
		60,
		120, 30,
		hWnd,
		(HMENU)10007,
		GetModuleHandle(NULL),
		NULL);
	HWND hWndButton3 = CreateWindowEx(NULL,
		_T("BUTTON"),
		_T("Выход"),
		WS_TABSTOP | WS_VISIBLE |
		WS_CHILD,
		20,
		100,
		120, 30,
		hWnd,
		(HMENU)10008,
		GetModuleHandle(NULL),
		NULL);
	

	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	int Len = 3;
	char a[2],a0[100],a1[] = "Конструктор Строки:   ", a2[] = "\nКонструктор Строки-Идентификотора:   ", a3[] = "\nКонструктор Битовой Строки:   ";
	static HWND hwndEditBox;
	HFONT hf; //шрифт для рисования
	static LOGFONT font; //структура для логического шрифта
	static int len = 14;
	static HWND hEditNum, hNum, hStr, hOk, hNew, hType, h1, h2, h3, h4, hOutPut, Konstuctor;
	switch (message)
	{
	case WM_CREATE:
	
		font.lfHeight = 30;
		font.lfWidth = 0; //use default value
		font.lfEscapement = 0; //without slope
		font.lfOrientation = 0;
		font.lfWeight = 0; //default
		font.lfItalic = 0;
		font.lfUnderline = 0;
		font.lfStrikeOut = 0;
		font.lfCharSet = DEFAULT_CHARSET;
		font.lfOutPrecision = OUT_DEFAULT_PRECIS;
		font.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		font.lfQuality = DEFAULT_QUALITY;
		font.lfPitchAndFamily = VARIABLE_PITCH | FF_DONTCARE;
		xtext = 100, ytext = 100;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);		
		hf = CreateFontIndirect(&font);
		SelectObject(hdc, hf);			
		EndPaint(hWnd, &ps);
		break;
	case WM_INITDIALOG:
		hwndEditBox = GetDlgItem(hWnd, 1);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_COMMAND:

		if (LOWORD(wParam) == 10006)
		{
			DestroyWindow(h1);
			DestroyWindow(h2);
			DestroyWindow(h3);
			DestroyWindow(h4);
			DestroyWindow(hEdit);
			DestroyWindow(hEditNum);
			DestroyWindow(hNew);
			DestroyWindow(hOk);
			DestroyWindow(hStr);
			DestroyWindow(hNum);
			DestroyWindow(hType);
			hNum = CreateWindowEx(NULL,
				_T("BUTTON"),
				_T("Число Элементов"),
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD,
				150,
				20,
				150, 30,
				hWnd,
				(HMENU)10009,
				GetModuleHandle(NULL),
				NULL);
			hNew = CreateWindowEx(NULL,
				_T("BUTTON"),
				_T("Начальное значение"),
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD,
				150,
				60,
				150, 30,
				hWnd,
				(HMENU)10019,
				GetModuleHandle(NULL),
				NULL);
		}
		if (LOWORD(wParam) == 10008)
		{
			DestroyWindow(hWnd);
		}
		if (LOWORD(wParam) == 10019)
		{
			if (Num == 0)
				MessageBox(NULL, "Число элементов не задано", "Error!", NULL);
			else
			{
				DestroyWindow(hNum);
				DestroyWindow(hStr);
				DestroyWindow(hOk);
				hEditNum = CreateWindowEx(WS_EX_CLIENTEDGE,_T("EDIT"),_T("Номер элемента"),
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE,
					360, 25, 150, 20, hWnd, (HMENU)10010,
					GetModuleHandle(NULL), NULL);
				
				hType = CreateWindow(_T("COMBOBOX"), _T("combobox"),
					WS_CHILD | WS_VISIBLE | CBS_SORT | CBS_DROPDOWNLIST,
					360, 55, 200, 400,
					hWnd, (HMENU)10012, GetModuleHandle(NULL), 0); 
				SendMessage(hType, CB_ADDSTRING, 0, (LPARAM)"Строка_Идентификатор");
				SendMessage(hType, CB_ADDSTRING, 0, (LPARAM)"Битовая_Строка");
				SendMessage(hType, CB_ADDSTRING, 0, (LPARAM)"Строка");
				SendMessage(hType, CB_SELECTSTRING, 1, (LPARAM)"Строка");
				SendMessage(hType, CB_SELECTSTRING, 2, (LPARAM)"Битовая_Строка");
				SendMessage(hType, CB_SELECTSTRING, 0, (LPARAM)"Строка_Идентификатор");
				hStr = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T("Строка"),
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE,
					360, 95, 150, 20, hWnd, (HMENU)10013,
					GetModuleHandle(NULL), NULL);
				
				hOk = CreateWindowEx(NULL,
					_T("BUTTON"),
					_T("Задать"),
					WS_TABSTOP | WS_VISIBLE |
					WS_CHILD,
					360,
					120,
					80, 30,
					hWnd,
					(HMENU)10014,
					GetModuleHandle(NULL),
					NULL);
			}
		}
		if (LOWORD(wParam) == 10009)
		{   
			if (Num != 0)
				MessageBox(NULL, "Число элементов уже задано", "Error!",NULL);
			else
			{
				DestroyWindow(hNew);
				hEditNum = CreateWindowEx(WS_EX_CLIENTEDGE,
					_T("EDIT"),
					_T("0"),
					WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE,
					320,
					25,
					100,
					20,
					hWnd,
					(HMENU)10010,
					GetModuleHandle(NULL),
					NULL);
				hOk = CreateWindowEx(NULL,
					_T("BUTTON"),
					_T("OK"),
					WS_TABSTOP | WS_VISIBLE |
					WS_CHILD,
					450,
					20,
					50, 30,
					hWnd,
					(HMENU)10011,
					GetModuleHandle(NULL),
					NULL);
			}
		}
		if (LOWORD(wParam) == 10007)
		{
			if (Num == 0)
				MessageBox(NULL, "Число элементов не задано", "Error!", NULL);
			else
			{
				DestroyWindow(h1);
				DestroyWindow(h2);
				DestroyWindow(h3);
				DestroyWindow(h4);
				DestroyWindow(hEdit);
				DestroyWindow(hEditNum);
				DestroyWindow(hNew);
				DestroyWindow(hOk);
				DestroyWindow(hStr);
				DestroyWindow(hNum);
				DestroyWindow(hType);
				hStr = CreateWindowEx(NULL,
					_T("BUTTON"),
					_T("Строка"),
					WS_TABSTOP | WS_VISIBLE |
					WS_CHILD,
					150,
					20,
					200, 30,
					hWnd,
					(HMENU)10015,
					GetModuleHandle(NULL),
					NULL);
				hOk = CreateWindowEx(NULL,
					_T("BUTTON"),
					_T("СТРОКА_ИДЕНТИФИКАТОР"),
					WS_TABSTOP | WS_VISIBLE |
					WS_CHILD,
					150,
					60,
					200, 30,
					hWnd,
					(HMENU)10016,
					GetModuleHandle(NULL),
					NULL);
				hNum = CreateWindowEx(NULL,
					_T("BUTTON"),
					_T("БИТОВАЯ_СТРОКА"),
					WS_TABSTOP | WS_VISIBLE |
					WS_CHILD,
					150,
					100,
					200, 30,
					hWnd,
					(HMENU)10017,
					GetModuleHandle(NULL),
					NULL);
				hNew = CreateWindowEx(NULL,
					_T("BUTTON"),
					_T("Задать операнд"),
					WS_TABSTOP | WS_VISIBLE |
					WS_CHILD,
					150,
					140,
					200, 30,
					hWnd,
					(HMENU)10019,
					GetModuleHandle(NULL),
					NULL);
			}
		}
		if (LOWORD(wParam) == 10011)
		{
			SendMessage((HWND)hEditNum, WM_GETTEXT, 64, (LPARAM)AD);
			sscanf_s(AD, "%i", &Num);
			Massive = new Strk*[Num];
			Exsist = new int[Num];
			memset(Exsist, int(0), sizeof(int)*Num);
			DestroyWindow(hNum);
			DestroyWindow(hEditNum);
			DestroyWindow(hOk);
		}
		if (LOWORD(wParam) == 10014)
		{
			int i, j, k = 0, z=0;
			char AD1[100];
			memset(AD, 0, 100);
			memset(AD1, 0, 100);
			SendMessage((HWND)hEditNum, WM_GETTEXT, 64, (LPARAM)AD);
			sscanf_s(AD, "%i", &i);

			sprintf_s(AD, "%i", Num);
			
			SendMessage((HWND)hStr, WM_GETTEXT, 64, (LPARAM)AD);
			if (AD[0] == '-')
			{
				z = 1;
				for (int d = 0; d < strlen(AD); d++)
					AD1[d] = AD[d + 1];
			}
			else
			{
				z = 0;
				for (int d = 0; d < strlen(AD); d++)
					AD1[d] = AD[d];
			}
			if (i > Num)
			{
				MessageBox(NULL, _T("Данного элемента не существует!"), _T("Ошибка"), NULL);
				break;
			}
			j=SendMessage((HWND)hType, CB_GETCURSEL, 0, 0);
			if (j == 2)
			{
				Massive[i + 1] = new IDStrk(AD);
				Exsist[i-1] = 1;
			}
			else if (j == 1)
			{
				Massive[i + 1] = new Strk(AD);
				Exsist[i-1] = 2;
			}
			else if (j == 0)
			{
				try
				{
					Massive[i + 1] = new ByteStrk(AD1, z);
				}
				catch (int a)
				{
					if (a == 1)
					{
						MessageBox(NULL, "Не битовая строка! Строка не заполнена!", "Error", NULL);
						char NStr[] = "0";
						Massive[i + 1] = new ByteStrk(NStr, 0);
					}
				}
				Exsist[i - 1] = 3;
			}
			memset(AD,0,100);
			while (Massive[i + 1]->RetStr()[k] != '\0')
			{
				//if (j == 2) AD[0] = ' ';
				//else if (j == 1) AD[0] = '+';
				if (j == 0 && z == 0) AD[0] = '-';
				else AD[0] = ' ';
				AD[k+1] = Massive[i + 1]->RetStr()[k];
				k++;
			}
			
		
			DestroyWindow(hNum);
			DestroyWindow(hStr);
			DestroyWindow(hNew);
			DestroyWindow(hEditNum);
			DestroyWindow(hType);
			DestroyWindow(hOk);
		}
		if (LOWORD(wParam) == 10015)
		{
			DestroyWindow(hNum);
			DestroyWindow(hOk);
			DestroyWindow(hNew);
			hEditNum = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T("Номер элемента"),
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE,
				400, 25, 150, 20, hWnd, (HMENU)10010,
				GetModuleHandle(NULL), NULL);
			hNew = CreateWindowEx(NULL,
				_T("BUTTON"),
				_T("Последний символ строки"),
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD,
				300,
				140,
				200, 30,
				hWnd,
				(HMENU)10020,
				GetModuleHandle(NULL),
				NULL);
		}
		if (LOWORD(wParam) == 10020)
		{
			int i;
			memset(AD, 0, 100);
			SendMessage((HWND)hEditNum, WM_GETTEXT, 64, (LPARAM)AD);
			sscanf_s(AD, "%i", &i);
			memset(AD, 0, 100);
			if (i > Num)
			{
				MessageBox(NULL, _T("Данного элемента не существует!"), _T("Ошибка"), NULL);
				break;
			}
			if (Exsist[i - 1] != 0)
				Massive[i+1]->GetLast(AD[0]);
			MessageBox(NULL, AD, "Содержимое", NULL);
			DestroyWindow(hNew);
			DestroyWindow(hEditNum);
			DestroyWindow(hStr);
		}
		if (LOWORD(wParam) == 10016)
		{
			DestroyWindow(hNum);
			DestroyWindow(hStr);
			DestroyWindow(hNew);
			h1 = CreateWindowEx(NULL,
				_T("BUTTON"),
				_T("Перевод 1-го символа в верхний регистр"),
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD,
				360,
				20,
				300, 30,
				hWnd,
				(HMENU)10025,
				GetModuleHandle(NULL),
				NULL);
			h2 = CreateWindowEx(NULL,
				_T("BUTTON"),
				_T("Присваивание"),
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD,
				360,
				60,
				200, 30,
				hWnd,
				(HMENU)10026,
				GetModuleHandle(NULL),
				NULL);
			h3 = CreateWindowEx(NULL,
				_T("BUTTON"),
				_T("Вычитание"),
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD,
				360,
				100,
				200, 30,
				hWnd,
				(HMENU)10027,
				GetModuleHandle(NULL),
				NULL);
			h4 = CreateWindowEx(NULL,
				_T("BUTTON"),
				_T("Проверка на равенство"),
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD,
				360,
				140,
				200, 30,
				hWnd,
				(HMENU)10028,
				GetModuleHandle(NULL),
				NULL);
		}
		if (LOWORD(wParam) == 10025)
		{
			DestroyWindow(h2);
			DestroyWindow(h3);
			DestroyWindow(h4);
			hEditNum = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T("Номер элемента"),
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE,
				400, 65, 150, 20, hWnd, (HMENU)10010,
				GetModuleHandle(NULL), NULL);
			hNew = CreateWindowEx(NULL,
				_T("BUTTON"),
				_T("Сменить регистр"),
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD,
				400,
				140,
				200, 30,
				hWnd,
				(HMENU)10029,
				GetModuleHandle(NULL),
				NULL);
		}
		if (LOWORD(wParam) == 10029)
		{
			int i;
			memset(AD, 0, 100);
			SendMessage((HWND)hEditNum, WM_GETTEXT, 64, (LPARAM)AD);
			sscanf_s(AD, "%i", &i);
			memset(AD, 0, 100);
			if (i > Num)
			{
				MessageBox(NULL, _T("Данного элемента не существует!"), _T("Ошибка"), NULL);
				break;
			}
			if (Exsist[i-1]==1)
				Massive[i + 1]->ChangeFirstSym();
			else
				MessageBox(NULL, "Не тот тип элемента!", "Ошибка", NULL);
			DestroyWindow(h1);
			DestroyWindow(hEditNum);
			DestroyWindow(hOk);
			DestroyWindow(hNew);
		}
		if (LOWORD(wParam) == 10026)
		{
			hEditNum = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T("Номер 1 элемента"),
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE,
				580, 65, 150, 20, hWnd, (HMENU)10030,
				GetModuleHandle(NULL), NULL);
			hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T("Номер 2 элемента"),
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE,
				740, 65, 150, 20, hWnd, (HMENU)10031,
				GetModuleHandle(NULL), NULL);
			hNew = CreateWindowEx(NULL,
				_T("BUTTON"),
				_T("Приравнять"),
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD,
				600,
				140,
				200, 30,
				hWnd,
				(HMENU)10032,
				GetModuleHandle(NULL),
				NULL);
			DestroyWindow(h1);
			DestroyWindow(h3);
			DestroyWindow(h4);
		}
		if (LOWORD(wParam) == 10032)
		{
			int i,j;
			memset(AD, 0, 100);
			SendMessage((HWND)hEditNum, WM_GETTEXT, 64, (LPARAM)AD);
			sscanf_s(AD, "%i", &i);
			memset(AD, 0, 100);
			SendMessage((HWND)hEdit, WM_GETTEXT, 64, (LPARAM)AD);
			sscanf_s(AD, "%i", &j);
			memset(AD, 0, 100);
			if (i > Num)
			{
				MessageBox(NULL, _T("Данного элемента не существует!"), _T("Ошибка"), NULL);
				break;
			}
			if (j > Num)
			{
				MessageBox(NULL, _T("Данного элемента не существует!"), _T("Ошибка"), NULL);
				break;
			}
			if (Exsist[i-1] == Exsist[j-1] && Exsist[j-1]!=0)
				*Massive[i + 1]=*Massive[j+1];
			else if(Exsist[i-1] != Exsist[j-1] && Exsist[j-1] != 0 && Exsist[i-1] !=0)
				MessageBox(NULL, "Несоответствие типов элементов!", "Ошибка", NULL);
			else if (Exsist[j-1] == 0)
				MessageBox(NULL, "Пустой элемент!", "Ошибка", NULL);
			else if (Exsist[j - 1] != 0 && Exsist[i - 1] == 0)
			{
				if (Exsist[j - 1] == 1)
					Massive[i + 1] = new IDStrk(*Massive[j + 1]);
				if (Exsist[j - 1] == 2)
					Massive[i + 1] = new Strk(*Massive[j + 1]);
				if (Exsist[j - 1] == 3)
					Massive[i + 1] = new ByteStrk(*Massive[j + 1]);
				Exsist[i - 1] = Exsist[j - 1];
			}
			DestroyWindow(h2);
			DestroyWindow(hEditNum);
			DestroyWindow(hEdit);
			DestroyWindow(hOk);
			DestroyWindow(hNew);
			DestroyWindow(h3);
			DestroyWindow(hNum);
		}
		if (LOWORD(wParam) == 10027)
		{
			hEditNum = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T("Номер 1 элемента"),
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE,
				580, 65, 150, 20, hWnd, (HMENU)10030,
				GetModuleHandle(NULL), NULL);
			hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T("Номер 2 элемента"),
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE,
				740, 65, 150, 20, hWnd, (HMENU)10031,
				GetModuleHandle(NULL), NULL);
			hNew = CreateWindowEx(NULL,
				_T("BUTTON"),
				_T("Вычесть"),
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD,
				600,
				140,
				200, 30,
				hWnd,
				(HMENU)10033,
				GetModuleHandle(NULL),
				NULL);
			DestroyWindow(h1);
			DestroyWindow(h2);
			DestroyWindow(h4);
		}
		if (LOWORD(wParam) == 10033)
		{
			int i, j;
			memset(AD, 0, 100);
			SendMessage((HWND)hEditNum, WM_GETTEXT, 64, (LPARAM)AD);
			sscanf_s(AD, "%i", &i);
			memset(AD, 0, 100);
			SendMessage((HWND)hEdit, WM_GETTEXT, 64, (LPARAM)AD);
			sscanf_s(AD, "%i", &j);
			memset(AD, 0, 100);
			if (i > Num)
			{
				MessageBox(NULL, _T("Данного элемента не существует!"), _T("Ошибка"), NULL);
				break;
			}
			if (j > Num)
			{
				MessageBox(NULL, _T("Данного элемента не существует!"), _T("Ошибка"), NULL);
				break;
			}
			if (Exsist[i - 1] == Exsist[j - 1] && Exsist[j - 1] != 0)
				*Massive[i + 1] - *Massive[j + 1];
			else if (Exsist[i - 1] != Exsist[j - 1] && Exsist[j - 1] != 0 && Exsist[i - 1] != 0)
				MessageBox(NULL, "Несоответствие типов элементов!", "Ошибка", NULL);
			else if (Exsist[j - 1] == 0 || Exsist[i - 1] == 0)
				MessageBox(NULL, "Пустой элемент!", "Ошибка", NULL);

			DestroyWindow(h2);
			DestroyWindow(hEditNum);
			DestroyWindow(hEdit);
			DestroyWindow(hOk);
			DestroyWindow(hNew);
			DestroyWindow(h3);
			DestroyWindow(hNum);
		}
		if (LOWORD(wParam) == 10028)
		{
			hEditNum = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T("Номер 1 элемента"),
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE,
				580, 65, 150, 20, hWnd, (HMENU)10030,
				GetModuleHandle(NULL), NULL);
			hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T("Номер 2 элемента"),
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE,
				740, 65, 150, 20, hWnd, (HMENU)10031,
				GetModuleHandle(NULL), NULL);
			hNew = CreateWindowEx(NULL,
				_T("BUTTON"),
				_T("Сравнить"),
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD,
				600,
				140,
				200, 30,
				hWnd,
				(HMENU)10034,
				GetModuleHandle(NULL),
				NULL);
			DestroyWindow(h1);
			DestroyWindow(h2);
			DestroyWindow(h3);
		}
		if (LOWORD(wParam) == 10034)
		{
			int i, j;
			memset(AD, 0, 100);
			SendMessage((HWND)hEditNum, WM_GETTEXT, 64, (LPARAM)AD);
			sscanf_s(AD, "%i", &i);
			memset(AD, 0, 100);
			SendMessage((HWND)hEdit, WM_GETTEXT, 64, (LPARAM)AD);
			sscanf_s(AD, "%i", &j);
			memset(AD, 0, 100);
			if (i > Num)
			{
				MessageBox(NULL, _T("Данного элемента не существует!"), _T("Ошибка"), NULL);
				break;
			}
			if (j > Num)
			{
				MessageBox(NULL, _T("Данного элемента не существует!"), _T("Ошибка"), NULL);
				break;
			}
			if (Exsist[i - 1] != 0 && Exsist[j - 1] != 0)
			{
				if (*Massive[i + 1] == *Massive[j + 1])
					MessageBox(NULL, _T("Строки равны"), _T("Результат"), NULL);
				else
					MessageBox(NULL, _T("Строки не равны"), _T("Результат"), NULL);
			}
			else
				MessageBox(NULL, _T("Пустой элемент!"), _T("Ошибка"), NULL);
			DestroyWindow(h4);
			DestroyWindow(hEditNum);
			DestroyWindow(hEdit);
			DestroyWindow(hOk);
			DestroyWindow(hNew);
			DestroyWindow(h3);
			DestroyWindow(hNum);
		}

		if (LOWORD(wParam) == 10017)
		{
			DestroyWindow(hOk);
			DestroyWindow(hStr);
			DestroyWindow(hNew);
			h1 = CreateWindowEx(NULL,
				_T("BUTTON"),
				_T("Удалить знак"),
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD,
				360,
				20,
				300, 30,
				hWnd,
				(HMENU)10035,
				GetModuleHandle(NULL),
				NULL);
			h2 = CreateWindowEx(NULL,
				_T("BUTTON"),
				_T("Присваивание"),
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD,
				360,
				60,
				200, 30,
				hWnd,
				(HMENU)10026,
				GetModuleHandle(NULL),
				NULL);
			h3 = CreateWindowEx(NULL,
				_T("BUTTON"),
				_T("Вычитание"),
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD,
				360,
				100,
				200, 30,
				hWnd,
				(HMENU)10027,
				GetModuleHandle(NULL),
				NULL);
			h4 = CreateWindowEx(NULL,
				_T("BUTTON"),
				_T("Проверка на равенство"),
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD,
				360,
				140,
				200, 30,
				hWnd,
				(HMENU)10028,
				GetModuleHandle(NULL),
				NULL);
		}
		if (LOWORD(wParam) == 10035)
		{
			DestroyWindow(h2);
			DestroyWindow(h3);
			DestroyWindow(h4);
			hEditNum = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T("Номер элемента"),
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_MULTILINE,
				400, 65, 150, 20, hWnd, (HMENU)10010,
				GetModuleHandle(NULL), NULL);
			hNew = CreateWindowEx(NULL,
				_T("BUTTON"),
				_T("Удалить знак"),
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD,
				400,
				140,
				200, 30,
				hWnd,
				(HMENU)10039,
				GetModuleHandle(NULL),
				NULL);
		}
		if (LOWORD(wParam) == 10039)
		{
			int i;
			memset(AD, 0, 100);
			SendMessage((HWND)hEditNum, WM_GETTEXT, 64, (LPARAM)AD);
			sscanf_s(AD, "%i", &i);
			memset(AD, 0, 100);
			if (i > Num)
			{
				MessageBox(NULL, _T("Данного элемента не существует!"), _T("Ошибка"), NULL);
				break;
			}
			if (Exsist[i - 1] != 0)
				Massive[i + 1]->DeleteZnak();
			DestroyWindow(h1);
			DestroyWindow(hEditNum);
			DestroyWindow(hOk);
			DestroyWindow(hNew);
			DestroyWindow(hNum);
		}
		memset(a, ' ',2);
		memset(a0, ' ', 100);
		sprintf_s(a,"%i",NStrk);
		for (int i = 0; i < 22; i++)
			a0[i] = a1[i];
		for (int i = 22; i < 23; i++)
			a0[i] = a[i-22];
		for (int i = 23; i < 61; i++)
			a0[i] = a2[i-23];
		memset(a, ' ', 2);
		sprintf_s(a, "%i", NIDStrk);
		for (int i = 61; i < 62; i++)
			a0[i] = a[i-61];
		for (int i = 66; i < 97; i++)
			a0[i] = a3[i-66];
		sprintf_s(a, "%i", NByteStrk);
		for (int i = 97; i < 100; i++)
			a0[i] = a[i-97];
		Konstuctor = CreateWindowEx(NULL,
			_T("STATIC"),
			a0,
			WS_TABSTOP | WS_VISIBLE |
			WS_CHILD,
			20,
			360,
			300, 300,
			hWnd,
			(HMENU)10111,
			GetModuleHandle(NULL),
			NULL);

		memset(Output, '\0', 10000);

		if (Num != 0)
		for (int i = 1; i < Num + 1; i++)
		{

			if (Exsist[i-1] != 0)
			{
				Output[Len - 3] = '0' + i;
				Output[Len - 2] = ' ';
				Output[Len - 1] = ' ';
				//Output[Len - 1] = Massive[i+1]->ShowZnak();
				//if (Exsist[i - 1] != 3)
				//{
					for (int j = 0; j < strlen(Massive[i + 1]->RetStr()); j++)
					{
						Output[j + Len] = Massive[i + 1]->RetStr()[j];
					}
					Len += strlen(Massive[i + 1]->RetStr()) + 4;
				/*}
				else
				{
					for (int j = 2; j < strlen(Massive[i + 1]->RetStr()); j++)
					{
						Output[j + Len - 2] = Massive[i + 1]->RetStr()[j];
					}
					Len += strlen(Massive[i + 1]->RetStr()) + 2;
				}*/
				Output[Len - 4] = '\n';
			}
			else
			{
				Output[Len - 3] = '0' + i;
				Output[Len - 2] = ' ';
				Output[Len - 1] = ' ';
				Len += 4;
				Output[Len - 4] = '\n';
			}
		}
		hOutPut = CreateWindowEx(NULL,
			_T("STATIC"),
			Output,
			WS_TABSTOP | WS_VISIBLE |
			WS_CHILD,
			20,
			180,
			300, 145,
			hWnd,
			(HMENU)10111,
			GetModuleHandle(NULL),
			NULL);
		
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

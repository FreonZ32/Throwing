#include<iostream>
#include<conio.h>
#include<thread>
#include<stdio.h>
#include<windows.h>
using namespace std;
using std::cout;
using std::cin;
using std::endl;

bool esc = false;

enum class Color
{
	pink = 0x00AA00FF,
	grass = 0x0000F0C0,
	light_blue = 0x00FFFF00,
	sea_wave = 0x00AFAA0C,
	white = 0x00FFFFFF,
	red = 0x000000FF,
	green = 0x0000FF00,
	blue = 0x00FF0000,
	yellow = 0x0000FFFF,
	grey = 0x00AAAAAA,
	orange = 0x000AAFFA,
	sky = 0x00FFADAA
};

int get_Color(int number)
{
	switch (number)
	{
	case 0:return (int)Color::pink; break;
	case 1:return (int)Color::grass; break;
	case 2:return (int)Color::light_blue; break;
	case 3:return (int)Color::sea_wave; break;
	case 4:return (int)Color::white; break;
	case 5:return (int)Color::red; break;
	case 6:return (int)Color::green; break;
	case 7:return (int)Color::blue; break;
	case 8:return (int)Color::yellow; break;
	case 9:return (int)Color::grey; break;
	case 10:return (int)Color::orange; break;
	case 11:return (int)Color::sky; break;
	default:return 0; break;
	}
}

void setCousorPosition(int x, int y)
{
	COORD position = { (short int)x,(short int)y };
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, position);
}

class Objects
{
protected:
	Color color;	//Цвет
	double elasticity_of_material;	//Материал объекта
public:
	//GET/SET
	const Color get_color()const
	{return this->color;}

	double get_elasticity_of_material()const
	{return elasticity_of_material;}

	void set_color(Color color)
	{this->color = color;}

	void set_elasticityl_of_material(double elasticity_of_material)
	{if (elasticity_of_material <= 0)elasticity_of_material = 1; this->elasticity_of_material = elasticity_of_material;}

	//CONSTRUCTOR
	Objects(Color color,double elasticity_of_material) :color(color)
	{set_color(color); set_elasticityl_of_material(elasticity_of_material);}
	~Objects(){}

	//METHODS
	virtual void draw()const = 0;
};
class Box :public Objects
{
	double field_height;	//Высота
	double field_width;		//Ширина
public:
	//GET/SET
	double get_field_height()const
	{
		return field_height;
	}

	double get_field_width()const
	{
		return field_width;
	}

	void set_field_height(double field_height)
	{
		if (field_height <= 0)field_height = 1; this->field_height = field_height;
	}

	void set_field_width(double field_width)
	{
		if (field_width <= 0)field_width = 1; this->field_width = field_width;
	}

	//CONSTRUCTOR
	Box(Color color, double elasticity_of_material,double field_height = 1, double field_width = 1) :Objects(color, elasticity_of_material)
	{
		set_field_height(field_height);
		set_field_width(field_width);
	}
	~Box(){}

	//METHODS
	void draw()const
	{
		HWND hwnd = GetConsoleWindow();
		HDC hdc = GetDC(hwnd);
		HBRUSH hBrush = CreateSolidBrush(RGB(0,0,0));
		//HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF)Color::grey);
		//HPEN holdPen = (HPEN)SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);
		SelectObject(hdc, hPen);
		Rectangle(hdc,200, 0,200+field_width,field_height);
		//DeleteObject(hPen);
		DeleteObject(hBrush);
	}
};

//void Input()
//{
//	char key;
//	key = _getch();
//	do
//	{
//		switch (key)
//		{
//		case 27:esc = true;
//		}
//	} while (_kbhit());
//}
void ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR)' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}
void thr1(Box& obj)
{
	while (!esc)
	{obj.draw();}

}
void thr2()
{
	while (!esc)
	{
	setCousorPosition(0, 20);
	cout << "Размер равен: " << 300;Sleep(2);
	}
	
}
void main()
{
	Box A(Color::red, 1, 300, 500);
	setlocale(LC_ALL, "rus");
	std::thread Box_thread(thr1,ref(A));
	std::thread Text_thread(thr2);
	cin.get();
	esc = true;

	Box_thread.join();
	Text_thread.join();
	
}
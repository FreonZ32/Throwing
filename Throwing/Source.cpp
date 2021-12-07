#include<iostream>
//#include<conio.h>
#include<thread>
#include<stdio.h>
#include<windows.h>
#include<vector>  
#include<ctime>
#include<math.h>
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
	sky = 0x00FFADAA,
	black = 0x00000000
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

int getStringHW(char obj)
{
	int width = 0;
	int height = 0;
	HWND hWnd = GetConsoleWindow();RECT rc;
	GetClientRect(hWnd, &rc);
	width = rc.right;
	height = rc.bottom;
	if (obj == 'y')return height;
	else if (obj == 'x')return width;
	else return 0;
}
int getCharHW(char obj)
{
	int width = 0;int height = 0;
	HANDLE hWndConsole;
	if (hWndConsole = GetStdHandle(-12))
	{
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		if (GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo))
		{
			width = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
			height = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1;
		}
	}
	if (obj == 'y')return height;
	else if (obj == 'x')return width;
	else return 0;
}
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
	if (!FillConsoleOutputCharacter(hStdOut, (TCHAR)' ', cellCount, homeCoords, &count)) return;
	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count)) return;
	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

class Objects
{
protected:
	Color color;	//Цвет
	double elasticity_of_material;	//Материал объекта
	short int screen_size_h;	//Размер окна в котором рисуется объект по высоте в пикселях
	short int screen_size_w;	//Размер окна в котором рисуется объект по широте в пикселя
	short int char_size_h;	//Размер знака в высоту в пикселях
	short int char_size_w;	//Размер знака в ширину в пикселях

public:
	//GET/SET
	const Color get_color()const
	{return this->color;}

	double get_elasticity_of_material()const
	{return elasticity_of_material;}

	short int get_char_size_h()const
	{return char_size_h;}

	void set_color(Color color)
	{this->color = color;}

	void set_elasticityl_of_material(double elasticity_of_material)
	{if (elasticity_of_material <= 0)elasticity_of_material = 1; this->elasticity_of_material = elasticity_of_material;}

	void set_screen_size_h()
	{this->screen_size_h = getStringHW('y');}

	void set_screen_size_w()
	{this->screen_size_w = getStringHW('x');}

	void set_char_size_h()
	{this->char_size_h = getStringHW('y')/getCharHW('y');}

	void set_char_size_w()
	{this->char_size_w = getStringHW('x')/getCharHW('x');}

	//CONSTRUCTOR
	Objects(Color color,double elasticity_of_material) :color(color)
	{
		set_color(color); set_elasticityl_of_material(elasticity_of_material);
		set_screen_size_h(); set_screen_size_w(); set_char_size_h(); set_char_size_w();
	}
	~Objects(){}

	//METHODS
};
class Box :public Objects
{
protected:
	double field_height;	//Высота
	double field_width;		//Ширина
public:
	//GET/SET
	double get_field_height()const
	{return field_height;}

	double get_field_width()const
	{return field_width;}

	void set_field_height(double field_height)
	{if (field_height <= 0)field_height = 1; this->field_height = field_height;}

	void set_field_width(double field_width)
	{if (field_width <= 0)field_width = 1; this->field_width = field_width;}

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
		HBRUSH hBrush = CreateSolidBrush((COLORREF)color);
		//SelectObject(hdc, hBrush);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF)Color::blue);
		//SelectObject(hdc, hPen);
		HPEN holdPen = (HPEN)SelectObject(hdc, hPen);
		Rectangle(hdc,int(screen_size_w/2-field_width/2), 0, int(screen_size_w/2 + field_width /2), int(field_height));
		DeleteObject(hPen);
		DeleteObject(hBrush);
		ReleaseDC(hwnd, hdc);
		Sleep(20);
	}
};

class Ball :public Objects
{
	int radius;
	int coord_x;	//Координаты по ширине поля
	int coord_y;	//Координаты по высоте поля
	double push_x;
	double push_y;
	//vector < vector <int> > path(n, vector <int> (m) );
public:
	//GET/SET
	int get_radius()const
	{return radius;}

	int get_coord_x()const
	{return coord_x;}

	int get_coord_y()const
	{return coord_y;}

	double get_push_x()const
	{return push_x;}

	double get_push_y()const
	{return push_y;}

	void set_radius(int radius)
	{if (radius <= 0)radius = 1; this->radius = radius;}

	void set_coord_x(int coord_x)
	{this->coord_x = coord_x;}

	void set_coord_y(int coord_y)
	{this->coord_y = coord_y;}

	void set_push_x(double push_x)
	{this->push_x = push_x;}

	void set_push_y(double push_y)
	{this->push_y = push_y;}

	//CONSTRUCTOR
	Ball(Color color, double elasticity_of_material, int radius = 1, int coord_x = 1, int coord_y = 1) 
		:Objects(color, elasticity_of_material)
	{
		set_radius(radius);
		set_coord_x(coord_x);
		set_coord_y(coord_y);
	}
	~Ball(){}

	//METHODS
	void draw(Box& obj)
	{
		HWND hwnd = GetConsoleWindow();
		HDC hdc = GetDC(hwnd);
		HBRUSH hBrush = CreateSolidBrush((COLORREF)color);
		//SelectObject(hdc, hBrush);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		HPEN hPen = CreatePen(PS_SOLID, 1, (COLORREF)Color::blue);
		//SelectObject(hdc, hPen);
		HPEN holdPen = (HPEN)SelectObject(hdc, hPen);
		Ellipse(hdc,int((screen_size_w / 2 - obj.get_field_width() / 2)+coord_x-radius), coord_y-radius,int((screen_size_w / 2 - obj.get_field_width() / 2)+coord_x+radius),coord_y+radius );
		DeleteObject(hPen);
		DeleteObject(hBrush);
		ReleaseDC(hwnd, hdc);
		Sleep(10);
	}
	void move(Box& obj)
	{
		if (coord_x - radius < 0 || ((coord_x + radius) > obj.get_field_width())) { set_push_x(push_x *= -1); coord_x += push_x; }
		else coord_x += push_x;
		if (coord_y - radius < 0 || ((coord_y + radius) > obj.get_field_height())) { set_push_y(push_y *= -1);coord_y += push_y; }
		else { coord_y += push_y;}
	}

};

void thr1(Box& obj)
{
	while (!esc)
	{obj.draw();}

}
void thr3(Box& obj,Ball& obj2)
{
	while (!esc)
	{
		obj2.draw(obj); 
		obj2.move(obj);
	}
}
void thr2(Box& obj,Ball& obj2)
{
	while (!esc)
	{
		setCousorPosition(0,int(obj.get_field_height()/obj.get_char_size_h()+1));
		cout << "Размер поля выс/длин: " << obj.get_field_height() << "/" << obj.get_field_width() << endl;
		cout << "Координаты шарика x/y: " << obj2.get_coord_x() << "/" << obj2.get_coord_y() << endl;
		Sleep(200);
	}
	
}
void main()
{
	Box A(Color::grey, 1, 400, 800);
	Ball B(Color::grass, 1, 15, 15, 15);
	setlocale(LC_ALL, "rus");
		B.set_push_x(15);
		B.set_push_y(-30);
	
	thread Box_thread(thr1,ref(A));
	thread Ball_thread(thr3,ref(A),ref(B));
	thread Text_thread(thr2,ref(A),ref(B));

	cin.get();
	esc = true;

	Box_thread.join();
	Ball_thread.join();
	Text_thread.join();
}
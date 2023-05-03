#include<iostream>
#include<stdio.h>
#include<conio.h>
#include<time.h>
#include<Windows.h>
#include"LevelGenerator.h"
#include"comander.h"
using namespace std;
void SetColor(int text, int background)
{
	SetConsoleTextAttribute(hCon, (WORD)((background << 4) | text));
}


void testShow(int** ptr, int** pUp, int** pCars, int** pMen, managerTL* cr, managerC* crCar, int x, int y)
{
	scrn.X = 1; scrn.Y = 1;
	SetConsoleCursorPosition(hCon, scrn);
	int count = 0;
	//char testSim=' ';
	char smile = char(2);
	for (int i = y; i < y + 47; i++, count++)
	{
		scrn.X = 1; scrn.Y = 1 + count;
		SetConsoleCursorPosition(hCon, scrn);
		for (int j = x; j < x + 97; j++)
		{
			if (count < 22 && j < x + 87 || count >= 22)
			{
				if (i >= 0 && i < row && j >= 0 && j < row)
					if (pUp[i][j] == trafficLightHorizont || pUp[i][j] == trafficLightVertic)
					{
						switch (pUp[i][j])
						{
							//case trafficLight:
							//SetColor(15, 10); _putch(' ');break;
						case trafficLightHorizont:
							switch (cr->horizont)
							{
							case greenTraf:
								SetColor(15, 10); break;
							case yellowTraf:
								SetColor(15, 14); break;
							case redTraf:
								SetColor(15, 12); break;
							}
							_putch(' '); break;
						case trafficLightVertic:
							switch (cr->vertical)
							{
							case greenTraf:
								SetColor(15, 10); break;
							case yellowTraf:
								SetColor(15, 14); break;
							case redTraf:
								SetColor(15, 12); break;
							}
							_putch(' '); break;
						}
						SetColor(15, 0);
					}
					else if (pCars[i][j])
					{
						SetColor(15, FindColorCar(crCar, pCars[i][j])); _putch(' ');
						SetColor(15, 0);
					}
					else if (pMen[i][j])//без учета переходов пешеходных
					{
						SetColor(14, 7); _putch(smile);
						SetColor(15, 0);
					}
					else
					{
						if (ptr[i][j] == road || ptr[i][j] == crosswalkGrey)
						{
							SetColor(15, 8); _putch(' ');
						}
						else if (ptr[i][j] == crosswalkWhite)
						{
							SetColor(15, 15); _putch(' ');
						}
						else if (ptr[i][j] == roadSide)
						{
							SetColor(15, 7);
							_putch(' ');
						}
						else if (ptr[i][j] == tree)
						{
							SetColor(2, 7); _putch('@');
						}
						else if (ptr[i][j] == lifeHouse)
						{
							SetColor(15, 3); _putch('#');
						}
						else if (ptr[i][j] == fabric)
						{
							SetColor(15, 0); _putch('#');
						}
						else
						{
							SetColor(15, 0);
							_putch(' ');
						}
					}
				else
				{
					SetColor(15, 0);
					_putch(' ');
				}
			}
		}
		_putch('\r'); _putch('\n');
		//cout<<endl;
	}
	SetColor(15, 0);
}
void __testShow(int** ptr, int** pUp, int** pCars, int** pMen, managerTL* cr, managerC* crCar, int x, int y)
{
	scrn.X = 0; scrn.Y = 5;
	SetConsoleCursorPosition(hCon, scrn);
	for (int i = y; i < y + 44; i++)
	{
		for (int j = x; j < x + 99; j++)
		{
			if (i >= 0 && i < row && j >= 0 && j < row)
				if (pUp[i][j])
				{
					switch (pUp[i][j])
					{
					case trafficLight:
						SetColor(15, 0); cout << " "; break;
					case trafficLightHorizont:
						switch (cr->horizont)
						{
						case greenTraf:
							SetColor(15, 10); break;
						case yellowTraf:
							SetColor(15, 14); break;
						case redTraf:
							SetColor(15, 12); break;
						}
						cout << " "; break;
					case trafficLightVertic:
						switch (cr->vertical)
						{
						case greenTraf:
							SetColor(15, 10); break;
						case yellowTraf:
							SetColor(15, 14); break;
						case redTraf:
							SetColor(15, 12); break;
						}
						cout << " "; break;
					}
					SetColor(15, 0);
				}
				else if (pCars[i][j])
				{
					SetColor(15, FindColorCar(crCar, pCars[i][j])); cout << " ";
					SetColor(15, 0);
				}
				else if (pMen[i][j])//без учета переходов пешеходных
				{
					SetColor(14, 7); cout << char(2);
					SetColor(15, 0);
				}
				else
				{
					if (ptr[i][j] == road)
					{
						SetColor(15, 8); cout << " ";
					}
					else if (ptr[i][j] == roadSide)
					{
						SetColor(15, 7);
						cout << " ";
					}
					else if (ptr[i][j] == tree)
					{
						SetColor(2, 7); cout << "@";
					}
					else if (ptr[i][j] == lifeHouse)
					{
						SetColor(15, 3); cout << "#";
					}
					else if (ptr[i][j] == fabric)
					{
						SetColor(15, 0); cout << "#";
					}
					else
					{
						SetColor(15, 0);
						cout << " ";
					}
				}
			else
			{
				SetColor(15, 0);
				cout << " ";
			}
		}
		cout << endl;
	}
	SetColor(15, 0);
}
bool PausaInfoWindow(managerH* ch, managerTL* ct, managerC* cr, managerM* cm, int xWindow, int yWindow)
{
	xWindow += 49;
	yWindow += 24;
	for (int i = 0; i < 12; i++)
	{
		scrn.Y = 15 + i; scrn.X = 30;
		SetConsoleCursorPosition(hCon, scrn);
		for (int j = 0; j < 40; j++)
		{
			if (i == 0 || j == 0 || i == 11 || j == 39)
			{
				SetColor(15, 14); _putch(' ');
			}
			else
			{
				SetColor(15, 6); _putch(' ');
			}
		}

	}

	SetColor(14, 6);
	scrn.X = 32; scrn.Y = 16;
	SetConsoleCursorPosition(hCon, scrn);
	cout << "PAUSA";

	SetColor(14, 6);
	scrn.X = 32; scrn.Y = 17;
	SetConsoleCursorPosition(hCon, scrn);
	cout << "Your position: (i)-> " << yWindow << " (j)-> " << xWindow;

	scrn.X = 32; scrn.Y = 19;
	SetConsoleCursorPosition(hCon, scrn);
	cout << "People->\t" << cm->countOfPeople;

	scrn.X = 32; scrn.Y = 20;
	SetConsoleCursorPosition(hCon, scrn);
	cout << "Cars->\t\t" << cr->countOfCar;

	scrn.X = 32; scrn.Y = 21;
	SetConsoleCursorPosition(hCon, scrn);
	cout << "Trafficlights->\t" << ct->count;

	scrn.X = 32; scrn.Y = 22;
	SetConsoleCursorPosition(hCon, scrn);
	cout << "LifeHouse->\t" << ch->countOfLifeHouse;

	scrn.X = 32; scrn.Y = 23;
	SetConsoleCursorPosition(hCon, scrn);
	cout << "Fabric->\t" << ch->countOfFabric;

	SetColor(14, 6);
	scrn.X = 32; scrn.Y = 25;
	SetConsoleCursorPosition(hCon, scrn);
	cout << "press: (esc)exit  (space)continue";

	SetColor(15, 0);
	scrn.X = 0; scrn.Y = 48;
	SetConsoleCursorPosition(hCon, scrn);
	int key = 1;
	do
	{
		if (_kbhit())
		{
			key = _getch();
			if (key == 32) return false;
		}
	} while (key != 27);
	return true;
}
void testMoveWindow(int& x, int& y, int step = 20)
{
	switch (_getch())
	{
	case 72: y -= step; break;
	case 80: y += step; break;
	case 75: x -= step; break;
	case 77: x += step; break;
	}
}

void InfoPanel()
{
	scrn.X = 0; scrn.Y = 0;
	SetConsoleCursorPosition(hCon, scrn);
	for (int i = 0; i < 49; i++)
	{
		for (int j = 0; j < 99; j++)
		{
			SetColor(14, 8);
			if (i == 0)
			{
				if (j == 0) cout << char(201);
				else if (j == 88) cout << char(203);
				else if (j == 98) cout << char(187);
				else cout << char(205);
			}
			else if (i == 22)
			{
				if (j == 0) cout << char(186);
				else if (j == 88) cout << char(200);
				else if (j == 98) cout << char(185);
				else if (j > 88 && j < 98) cout << char(205);
				else _putch(' ');
			}
			else if (i < 23)
			{
				if (j == 0 || j == 88 || j == 98)cout << char(186);
				else if (j < 88) _putch(' ');
				else
				{

					SetColor(15, 14);
					cout << char(176);
				}
			}
			else if (i == 48)
			{
				if (j == 0) cout << char(200);
				else if (j == 98) cout << char(188);
				else cout << char(205);
			}
			else if (i > 22)
				if (j == 0 || j == 98) cout << char(186);
				else _putch(' ');

		}
		cout << endl;
	}
}
int SizeIn()
{
	char size[5] = {};
	char sim = '\0';
	int k = 0;
	while (sim != 13)
	{
		sim = _getch();
		if (sim > 47 && sim < 58)
		{
			size[k] = sim;
			int temp = strlen(size);
			for (int i = 0; i < temp; i++) cout << char(8);
			cout << size;
			if (k < 3) k++;
		}
		else if (sim == 8)
		{
			int temp = strlen(size);
			for (int i = 0; i < temp; i++) cout << char(8);
			for (int i = 0; i < temp; i++) cout << " ";
			if (size[k])
			{
				size[k] = '\0';
				//k--;
			}
			else if (k > 0)
			{
				size[k - 1] = '\0';
				k--;
			}
			temp = strlen(size);
			for (int i = 0; i < temp; i++) cout << char(8);
			cout << size;
		}
	}
	int rez = atoi(size);
	if (rez == 0 || rez < 500)
	{
		int temp = strlen(size);
		for (int i = 0; i < temp; i++) cout << char(8);
		for (int i = 0; i < temp; i++) cout << " ";
		for (int i = 0; i < 3; i++) cout << char(8);
		cout << "500";
		return 500;
	}
	if (rez > 1500)
	{
		int temp = strlen(size);
		for (int i = 0; i < temp; i++) cout << char(8);
		for (int i = 0; i < temp; i++) cout << " ";
		for (int i = 0; i < 4; i++) cout << char(8);
		cout << "1500";
		return 1500;
	}
	return rez;
}
void IntroMenu()
{
	for (int i = 0; i < 16; i++)
	{
		scrn.Y = 15 + i; scrn.X = 30;
		SetConsoleCursorPosition(hCon, scrn);
		for (int j = 0; j < 40; j++)
		{
			if (i == 0 || i == 1 || i == 2 || j == 0 || i == 11 || i == 15 || j == 39)
			{
				SetColor(15, 14); _putch(' ');
			}
			else
			{
				SetColor(15, 6); _putch(' ');
			}
		}
	}
	scrn.Y = 16; scrn.X = 44;
	SetConsoleCursorPosition(hCon, scrn);
	SetColor(6, 14);
	cout << "CITY WINDOW";

	scrn.Y = 19; scrn.X = 32;
	SetConsoleCursorPosition(hCon, scrn);
	SetColor(14, 6);
	cout << "Choise size of the city (500-1500)";

	scrn.Y = 27; scrn.X = 60;
	SetConsoleCursorPosition(hCon, scrn);
	SetColor(14, 6);
	cout << "CONTROL:";
	scrn.Y = 28; scrn.X = 60;
	SetConsoleCursorPosition(hCon, scrn);
	cout << char(24) << " " << char(25) << " " << char(26) << " " << char(27);
	scrn.Y = 29; scrn.X = 57;
	SetConsoleCursorPosition(hCon, scrn);
	cout << "esc to menu";

	scrn.Y = 22; scrn.X = 32;
	SetConsoleCursorPosition(hCon, scrn);
	cout << "ROW            ";
	SetColor(15, 0);
	for (int i = 0; i < 20; cout << " ", i++);

	SetColor(14, 6);
	scrn.Y = 24; scrn.X = 32;
	SetConsoleCursorPosition(hCon, scrn);
	cout << "COLUM          ";
	SetColor(15, 0);
	for (int i = 0; i < 20; cout << " ", i++);

	scrn.Y = 22; scrn.X = 66;
	SetConsoleCursorPosition(hCon, scrn);
	row = SizeIn();

	scrn.Y = 24; scrn.X = 66;
	SetConsoleCursorPosition(hCon, scrn);
	colum = SizeIn();


	scrn.Y = 0; scrn.X = 0;
	SetConsoleCursorPosition(hCon, scrn);

}

void main()
{
	srand(time(NULL));
	system("mode con cols=100 lines=50");//размеры привязаны к show()
	//InfoPanel();
	IntroMenu();

	int** downLayer = CreateNewBigLevel();
	int** houseLayer = CreateNewBigLevel();
	int** upLayer = CreateNewBigLevel();
	int** carLayer = CreateNewBigLevel();
	int** menLayer = CreateNewBigLevel();
	managerTL* mainTL = CreatTrafManager();
	managerH* mainH = CreatHouseManager();
	managerC* mainC = CreatCarManager();
	managerM* mainM = CreatMenManager();
	myTime timer = { 9,0,0 };
	int x = 1, y = 1;
	int key = 1;

	//лог файл
	char* path = "C:\\Users\\Grizzly\\Desktop\\c_work\\Projects\\BigCity\\CityWindows\\logOfCity.txt";

	MaineBuilder(downLayer, houseLayer, carLayer, upLayer, menLayer, mainH, mainTL, mainC, mainM);

	InfoPanel();
	testShow(downLayer, upLayer, carLayer, menLayer, mainTL, mainC, x, y);

	bool flagWork = true;
	do
	{
		if (_kbhit())
		{
			key = _getch();
			if (key == 224)
				testMoveWindow(x, y, 40);
			else if (key == 27)
			{
				if (PausaInfoWindow(mainH, mainTL, mainC, mainM, x, y)) break;
			}
		}

		testShow(downLayer, upLayer, carLayer, menLayer, mainTL, mainC, x, y);
		EveryBodyMoveNow(carLayer, upLayer, downLayer, menLayer, mainTL, mainC, mainM, &timer, path);
	} while (flagWork);

	FreeBigLevel(downLayer);
	FreeBigLevel(upLayer);
	FreeBigLevel(houseLayer);
	FreeBigLevel(carLayer);
	FreeBigLevel(menLayer);
	FreeCars(mainC);
	FreeTrafficLight(mainTL);
	FreeBuilding(mainH);
	FreeMens(mainM);
}
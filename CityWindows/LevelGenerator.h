COORD scrn;
HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);

#define STOPTIME 14
#define DELAY 120000

enum { road = 1, roadSide, carBody, carHead, oneMen, trafficLight, trafficLightHorizont, trafficLightVertic, tree, crosswalkGrey, crosswalkWhite };
enum colorOfTraffic { greenTraf = 30, redTraf, yellowTraf };
enum houseType { fabric = 40, lifeHouse };
enum movingVec { rightMove, downMove, leftMove, upMove };
enum speed { stop, lightMove, middleMove, fastMove };
enum colorOf { blue = 1, green, cyan, red, magenta, brown, lightBlue = 9, lightGreen, lightCyan, lightRed, lightMagenta, yellow };
enum modeOfMen { freeWalking, stopMode, searchLifeHouse, searchFabric };
enum stateOfMen { inTheHouse, onTheStreet };
enum power { noPower, lightPower, middlePower, strongPower };

int row = 1000, colum = 1000;

//вектор движения для людей и машин
int movVector[4][2] = { 0,1,1,0,0,-1,-1,0 };

//светофор
struct trafLight
{
	trafLight* pPrev;
	trafLight* pNext;
	int centrI;
	int centrJ;
};
//управляющий светофорами
struct managerTL
{
	trafLight* pFirst;
	int count;
	colorOfTraffic horizont;
	colorOfTraffic vertical;
};
//дом
struct house
{
	house* pPrev;
	house* pNext;
	int countOfPeople;
	int numberOfHouse;
	houseType typeOfHouse;
};
//управляющий домами
struct managerH
{
	house* pFirst;
	house* pLast;
	int count;
	int countOfFabric;
	int countOfLifeHouse;
};
//автомобиль
struct car
{
	car* pPrev;
	car* pNext;
	movingVec defaultVec;
	movingVec priorityVec;
	int numberOfCar;
	colorOf colorOfCar;
	colorOf colorOfCarHead;
	speed carSpeed;
	int startI;
	int startJ;
	int countStepAfterPriority;
	int countOfStop;
	bool flagPriority;
};
//управляющий автомобилями
struct managerC
{
	car* pFirst;
	car* pLast;
	int countOfCar;
};
//человек
struct men
{
	men* pPrev;
	men* pNext;
	int numberOfMen;
	int startI;
	int startJ;
	movingVec vectorOfMove;
	modeOfMen defaultMode;
	modeOfMen modeOfPriority;
	stateOfMen state;
	bool flagPriority;
};
//управляющий людьми
struct managerM
{
	men* pFirst;
	men* pLast;
	int countOfPeople;
	int countInHouse;
	modeOfMen modeOfPriority;
	power powerOfPriority;
};
//таймер
struct myTime
{
	int hours;
	int minutes;
	int seconds;
};

//--------------------------------------PEOPLE---------------------------------
managerM* CreatMenManager()
{
	managerM* cr = new managerM;
	cr->countOfPeople = 0;
	cr->pFirst = NULL;
	cr->pLast = NULL;
	cr->powerOfPriority = noPower;
	cr->countInHouse = 0;
	cr->modeOfPriority = freeWalking;
	return cr;
}
movingVec RandomVector()
{
	switch (rand() % 4)
	{
	case 0: return rightMove;
	case 1: return downMove;
	case 2: return leftMove;
	}
	return upMove;
}
men* CreatMen()
{
	men* nElem = new men;
	nElem->pNext = NULL;
	nElem->pPrev = NULL;
	nElem->flagPriority = false;
	nElem->defaultMode = freeWalking;
	nElem->modeOfPriority = freeWalking;
	nElem->state = onTheStreet;
	nElem->vectorOfMove = RandomVector();
	return nElem;
}
int AddNewMen(managerM* cm, int tempI, int tempJ)
{
	men* nElem = CreatMen();
	cm->countOfPeople++;
	nElem->numberOfMen = cm->countOfPeople;
	nElem->startI = tempI;
	nElem->startJ = tempJ;
	if (cm->pFirst == NULL)
	{
		cm->pFirst = nElem;
		cm->pLast = nElem;
		return nElem->numberOfMen;
	}
	men* temp = cm->pLast;
	temp->pNext = nElem;
	nElem->pPrev = temp;
	cm->pLast = nElem;
	return nElem->numberOfMen;
}
bool TestMenPosition(int** pDown, int** pMen, int& startI, int& startJ)
{
	startI = 10 + rand() % (row - 11);
	startJ = 14 + rand() % (colum - 15);
	if (pDown[startI][startJ] == roadSide && pMen[startI][startJ] == 0) return true;
	return false;
}
void MenOnPosition(managerM* cm, int** pDown, int** pMen)
{
	int tempI = 0, tempJ = 0;
	while (!TestMenPosition(pDown, pMen, tempI, tempJ));
	pMen[tempI][tempJ] = AddNewMen(cm, tempI, tempJ);
}
void BuildMen(managerM* cm, int** pDown, int** pMen, int countOfHouse)
{
	int countOfPeople = countOfHouse * 4;
	for (int i = 0; i < countOfPeople; i++)
		MenOnPosition(cm, pDown, pMen);
}
void FreeMens(managerM* cr)
{
	if (cr->pFirst == NULL)
	{
		delete cr;
		return;
	}
	men* temp = cr->pFirst;
	men* tempNext = NULL;
	while (temp)
	{
		tempNext = temp->pNext;
		delete temp;
		temp = tempNext;
	}
	delete cr;
}

//--------------------------------------CARS-----------------------------------
managerC* CreatCarManager()
{
	managerC* cr = new managerC;
	cr->countOfCar = 0;
	cr->pFirst = NULL;
	cr->pLast = NULL;
	return cr;
}
colorOf ChoiseColorOfCar()
{
	colorOf tempColor;
	int temp = rand() % 12;
	switch (temp)
	{
	case 0:
		tempColor = blue; break;
	case 1:
		tempColor = green; break;
	case 2:
		tempColor = cyan; break;
	case 3:
		tempColor = red; break;
	case 4:
		tempColor = magenta; break;
	case 5:
		tempColor = brown; break;
	case 6:
		tempColor = lightBlue; break;
	case 7:
		tempColor = lightGreen; break;
	case 8:
		tempColor = lightCyan; break;
	case 9:
		tempColor = lightRed; break;
	case 10:
		tempColor = lightMagenta; break;
	case 11:
		tempColor = yellow; break;
	}
	return tempColor;
}

car* CreatCar()
{
	car* nElem = new car;
	nElem->carSpeed = stop;
	nElem->colorOfCar = ChoiseColorOfCar();
	nElem->colorOfCarHead = ChoiseColorOfCar();
	nElem->flagPriority = false;
	nElem->pNext = NULL;
	nElem->pPrev = NULL;
	nElem->numberOfCar = -1;
	nElem->countStepAfterPriority = 3;
	nElem->countOfStop = 0;
	return nElem;
}
int AddNewCar(managerC* cr, movingVec vec, int tempI, int tempJ)
{
	car* nElem = CreatCar();
	cr->countOfCar++;
	nElem->numberOfCar = cr->countOfCar;
	nElem->defaultVec = vec;
	switch (vec)
	{
	case rightMove:
		nElem->startI = tempI - 1;
		nElem->startJ = tempJ;
		break;
	case leftMove:
		nElem->startI = tempI;
		nElem->startJ = tempJ;
		break;
	case upMove:
		nElem->startI = tempI;
		nElem->startJ = tempJ - 2;
		break;
	case downMove:
		nElem->startI = tempI;
		nElem->startJ = tempJ;
		break;
	}
	if (cr->pFirst == NULL)
	{
		cr->pFirst = nElem;
		cr->pLast = nElem;
		return nElem->numberOfCar;
	}
	car* temp = cr->pLast;
	temp->pNext = nElem;
	nElem->pPrev = temp;
	cr->pLast = nElem;
	return nElem->numberOfCar;
}

//проверка места под авто
bool TestCarPlace(int** pDown, int** pCars, int tempI, int tempJ, movingVec tempVec)
{
	switch (tempVec)
	{
	case rightMove:
	case leftMove:
		for (int j = tempJ + 1; j < tempJ + 4; j++)
			if (pDown[tempI][j] == roadSide || pCars[tempI][j]) return false;
		break;
	case upMove:
	case downMove:
		for (int i = tempI + 1; i < tempI + 3; i++)
			if (pDown[i][tempJ] == roadSide || pCars[i][tempJ]) return false;
		break;
	}
	return true;
}
//находим место для авто
movingVec FindCarPlace(int** pDown, int** pCars, int& startI, int& startJ)
{
	movingVec tempVec;
	int tempI, tempJ;
	bool flag = false;
	do
	{
		tempI = rand() % row;
		tempJ = rand() % colum;
		if (pDown[tempI][tempJ] == road && pCars[tempI][tempJ] == NULL)
			for (int i = tempI - 3; i < tempI + 2; i++)
			{
				for (int j = tempJ - 4; j < tempJ + 3; j++)
				{
					if (i > 0 && j > 0 && i < row - 1 && j < colum - 1)
					{
						if (pDown[i][j] == roadSide)
						{
							if (pDown[i - 1][j] == road)
							{
								tempVec = rightMove;
								flag = true;
								startI = i - 1; startJ = j;
								break;
							}
							if (pDown[i + 1][j] == road)
							{
								tempVec = leftMove;
								flag = true;
								startI = i + 1;	startJ = j;
								break;
							}
							if (pDown[i][j - 1] == road)
							{
								tempVec = upMove;
								flag = true;
								startI = i;	startJ = j - 1;
								break;
							}
							if (pDown[i][j + 1] == road)
							{
								tempVec = downMove;
								flag = true;
								startI = i;	startJ = j + 1;
								break;
							}
						}
					}
					if (flag) break;
				}
				if (flag) break;
			}
		if (flag)
			if (!TestCarPlace(pDown, pCars, startI, startJ, tempVec)) flag = false;
	} while (!flag);
	return tempVec;
}
//разместитель автомобилей
void MadeNewCar(managerC* cr, int** pDown, int** pCars)
{
	int startI, startJ;
	movingVec tempVec = FindCarPlace(pDown, pCars, startI, startJ);

	int numberCars = AddNewCar(cr, tempVec, startI, startJ);
	switch (tempVec)
	{
	case rightMove:
		for (int i = startI - 1; i <= startI; i++)
		{
			for (int j = startJ; j < startJ + 4; j++)
			{
				pCars[i][j] = numberCars;
			}
		}
		break;
	case leftMove:
		for (int i = startI; i <= startI + 1; i++)
		{
			for (int j = startJ; j < startJ + 4; j++)
			{
				pCars[i][j] = numberCars;
			}
		}
		break;
	case upMove:
		for (int i = startI; i < startI + 3; i++)
		{
			for (int j = startJ - 2; j <= startJ; j++)
			{
				pCars[i][j] = numberCars;
			}
		}
		break;
	case downMove:
		for (int i = startI; i < startI + 3; i++)
		{
			for (int j = startJ; j < startJ + 3; j++)
			{
				pCars[i][j] = numberCars;
			}
		}
		break;
	}
}
//строитель машин
void BuildCars(managerC* cr, int** pDown, int** pCars, int countOfHouse)
{
	int numbersOfCars = countOfHouse;
	for (int i = 0; i < numbersOfCars; i++)
	{
		MadeNewCar(cr, pDown, pCars);
	}
}
colorOf FindColorCar(managerC* cr, int number)
{
	car* temp = cr->pFirst;
	while (temp->numberOfCar != number)
	{
		temp = temp->pNext;
	}
	return temp->colorOfCar;
}
void FreeCars(managerC* cr)
{
	if (cr->pFirst == NULL)
	{
		delete cr;
		return;
	}
	car* temp = cr->pFirst;
	car* tempNext = NULL;
	while (temp)
	{
		tempNext = temp->pNext;
		delete temp;
		temp = tempNext;
	}
	delete cr;
}

//-----------------------------------TRAFFICLIGHTS-----------------------------
managerTL* CreatTrafManager()
{
	managerTL* cr = new managerTL;
	cr->pFirst = NULL;
	cr->count = 0;
	cr->horizont = yellowTraf;
	cr->vertical = yellowTraf;
	return cr;
}
trafLight* CreatTraf()
{
	trafLight* nElem = new trafLight;
	nElem->pNext = NULL;
	nElem->pPrev = NULL;
	nElem->centrI = 0;
	nElem->centrJ = 0;
	return nElem;
}
void AddNewTraf(managerTL* cr, int i, int j)
{
	trafLight* nElem = CreatTraf();
	nElem->centrI = i;
	nElem->centrJ = j;
	cr->count++;
	if (!cr->pFirst)
	{
		cr->pFirst = nElem;
		return;
	}
	trafLight* temp = cr->pFirst;
	while (temp->pNext)
	{
		temp = temp->pNext;
	}
	temp->pNext = nElem;
	nElem->pPrev = temp;
}
//новый светофор если дрога уперлась в другую или развилка
void PositionTraf(managerTL* cr, int i, int j, int vec, int rez = 0)
{
	int tempI, tempJ;
	switch (vec)
	{
	case 0:
		tempI = i + 2; tempJ = j + 3 + rez / 5; break;
	case 1:
		tempI = i + 2 + rez / 7; tempJ = j + 3; break;
	case 2:
		tempI = i + 2; tempJ = j + 3 - rez / 5; break;
	case 3:
		tempI = i + 2 - rez / 7; tempJ = j + 3; break;
	}
	if (tempI < 0 || tempJ < 0 || tempI >= row || tempJ >= colum) return;
	AddNewTraf(cr, tempI, tempJ);
}
//новый светофор при стартовой позиции
void PositionTrafStart(managerTL* cr, int i, int j, int vec)
{
	int tempI, tempJ;
	switch (vec)
	{
	case 0:
		tempI = i + 2; tempJ = j - 4; break;
	case 1:
		tempI = i - 3; tempJ = j + 3; break;
	case 2:
		tempI = i + 2; tempJ = j + 10; break;
	case 3:
		tempI = i + 7; tempJ = j + 3; break;
	}
	if (tempI < 0 || tempJ < 0 || tempI >= row || tempJ >= colum) return;
	AddNewTraf(cr, tempI, tempJ);
}
//размещение светофоров на верхнем слое
bool BuildTraf(managerTL* cr, int** pUp)
{
	trafLight* temp = cr->pFirst;
	if (temp == NULL) return false;
	while (temp)
	{
		int i = temp->centrI - 1;
		int j;
		for (; i <= temp->centrI + 1; i++)
		{
			j = temp->centrJ - 1;
			for (; j <= temp->centrJ + 1; j++)
			{
				if (i >= 0 && j >= 0 && i < row && j < colum)
				{
					if (i == temp->centrI && j != temp->centrJ)
						pUp[i][j] = trafficLightVertic;
					else if (i != temp->centrI && j == temp->centrJ)
						pUp[i][j] = trafficLightHorizont;
					else
						pUp[i][j] = trafficLight;
				}
			}
		}
		temp = temp->pNext;
	}
	return true;
}
void FreeTrafficLight(managerTL* cr)
{
	if (cr->pFirst == NULL)
	{
		delete cr; return;
	}
	trafLight* temp = cr->pFirst;
	trafLight* temp2 = NULL;
	if (temp->pNext == NULL)
	{
		delete cr->pFirst;
		delete cr;
		return;
	}
	temp2 = temp;
	temp = temp->pNext;
	while (temp)
	{
		temp2 = temp;
		temp = temp->pNext;
		delete temp2;
	}
	delete cr->pFirst;
	delete cr;
}

//--------------------------------BUILDINGS------------------------------------
managerH* CreatHouseManager()
{
	managerH* cr = new managerH;
	cr->pFirst = NULL;
	cr->pLast = NULL;
	cr->count = 0;
	cr->countOfFabric = 0;
	cr->countOfLifeHouse = 0;
	return cr;
}
house* CreatHouse()
{
	house* nElem = new house;
	nElem->pPrev = NULL;
	nElem->pNext = NULL;
	nElem->countOfPeople = 0;
	nElem->numberOfHouse = -1;
	return nElem;
}
//добавляем новый дом структуру 1-жилой дом, 2-фабрика возвращает номер дома
int AddNewHouse(managerH* cr, houseType tHouse = lifeHouse)
{
	house* nElem = CreatHouse();
	nElem->typeOfHouse = tHouse;
	if (tHouse == lifeHouse) cr->countOfLifeHouse++;
	else if (tHouse == fabric) cr->countOfFabric++;
	cr->count++;
	nElem->numberOfHouse = cr->count;
	if (cr->pFirst == NULL)
	{
		cr->pFirst = nElem;
		cr->pLast = nElem;
		return nElem->numberOfHouse;
	}
	house* temp = cr->pFirst;
	while (temp->pNext)
	{
		temp = temp->pNext;
	}
	temp->pNext = nElem;
	nElem->pPrev = temp;
	cr->pLast = nElem;
	return nElem->numberOfHouse;
}
//создаем дом
void BuildHouse(int** pDown, int** pHouse, managerH* cr, int startI, int startJ, int countLine, int countCols, int min, int max)
{
	//ошибка в неправильном расчете длины участка. sizeI/2 нельзя надо (sizeI-startI)/2
	int sizeI = startI + countCols;
	int sizeJ = startJ + countLine;
	int forma = 0;
	houseType tHouse = lifeHouse;
	int numHouse = 0;
	//условия выбора типа и формы дома
	if (countLine >= max * 0.6 && countCols >= max * 0.6)
	{
		forma = rand() % 6 + 1; tHouse = fabric;
	}
	/*else if(countLine>countCols*2)
	forma=3;
	else if(countCols>countLine*2)
	forma=2;*/
	/*else if(countLine<min*1,2&&countCols<min*1,2)
	forma=rand()%3+1;*/
	else forma = rand() % 6 + 1;
	numHouse = AddNewHouse(cr, tHouse);
	switch (forma)
	{
	case 1:
		for (int i = startI; i < sizeI; i++)
		{
			for (int j = startJ; j < sizeJ; j++)
			{
				if (i == startI || i == sizeI - 1 || j == startJ || j == sizeJ - 1) pDown[i][j] = roadSide;
				else
				{
					pDown[i][j] = tHouse;
					pHouse[i][j] = numHouse;
				}
			}
		}
		break;
	case 2:
		for (int i = startI; i < sizeI; i++)
		{
			for (int j = startJ; j < sizeJ; j++)
			{
				if (i > startI + (sizeI - startI) / 2) { pDown[i][j] = 0; continue; }//TODO обнуление этой части массива (сейчас она -1)
				if (i == startI || i == startI + (sizeI - startI) / 2 || j == startJ || j == sizeJ - 1) pDown[i][j] = roadSide;
				else
				{
					pDown[i][j] = tHouse;
					pHouse[i][j] = numHouse;
				}
			}

		}
		break;
	case 3:
		for (int i = startI; i < sizeI; i++)
		{
			for (int j = startJ; j < sizeJ; j++)
			{
				if (j > startJ + (sizeJ - startJ) / 2) { pDown[i][j] = 0; continue; }//TODO обнуление этой части массива (сейчас она -1)
				if (i == startI || i == sizeI - 1 || j == startJ || j == startJ + (sizeJ - startJ) / 2) pDown[i][j] = roadSide;
				else
				{
					pDown[i][j] = tHouse;
					pHouse[i][j] = numHouse;
				}
			}

		}
		break;
	case 4:
		for (int i = startI; i < sizeI; i++)
		{
			for (int j = startJ; j < sizeJ; j++)
			{
				if (i > startI + (sizeI - startI) / 2 && j > startJ + (sizeJ - startJ) / 2) { pDown[i][j] = 0; continue; }//TODO обнуление этой части массива (сейчас она -1)
				if (i == startI || j == startJ || (i == startI + (sizeI - startI) / 2 && j >= startJ + (sizeJ - startJ) / 2) ||
					(j == sizeJ - 1 && i <= startI + (sizeI - startI) / 2) || (j == startJ + (sizeJ - startJ) / 2 && i >= startI + (sizeI - startI) / 2) ||
					(i == sizeI - 1 && j <= startJ + (sizeJ - startJ) / 2)) pDown[i][j] = roadSide;
				else
				{
					pDown[i][j] = tHouse;
					pHouse[i][j] = numHouse;
				}
			}

		}
		break;
	case 5:
		for (int i = startI; i < sizeI; i++)
		{
			for (int j = startJ; j < sizeJ; j++)
			{
				if (i > startI + (sizeI - startI) / 2 && j < startJ + (sizeJ - startJ) / 2) { pDown[i][j] = 0; continue; }//TODO обнуление этой части массива (сейчас она -1)
				if (i == startI || j == startJ || j == sizeJ - 1 || (i <= startI + (sizeI - startI) / 2 && j == startJ) ||
					(i == startI + (sizeI - startI) / 2 && j <= startJ + (sizeJ - startJ) / 2) || (i >= startI + (sizeI - startI) / 2 && j == startJ + (sizeJ - startJ) / 2)
					|| (i == sizeI - 1 && j >= startJ + (sizeJ - startJ) / 2)) pDown[i][j] = roadSide;
				else
				{
					pDown[i][j] = tHouse;
					pHouse[i][j] = numHouse;
				}
			}

		}
		break;
	case 6:
		for (int i = startI; i < sizeI; i++)
		{
			for (int j = startJ; j < sizeJ; j++)
			{
				if (i<startI + (sizeI - startI) / 2 && j>startJ + (sizeJ - startJ) / 2) { pDown[i][j] = 0; continue; }//TODO обнуление этой части массива (сейчас она -1)
				if (j == startJ || i == sizeI - 1 || (i == startI && j <= startJ + (sizeJ - startJ) / 2) || (i <= startI + (sizeI - startI) / 2 && j == startJ + (sizeJ - startJ) / 2)
					|| (i == startI + (sizeI - startI) / 2 && j >= startJ + (sizeJ - startJ) / 2) || (i >= startI + (sizeI - startI) / 2 && j == sizeJ - 1)) pDown[i][j] = roadSide;
				else
				{
					pDown[i][j] = tHouse;
					pHouse[i][j] = numHouse;
				}
			}

		}
		break;
	}
}
//поиск места под дом
bool FindPlaceForHouse(int** pDown, int** pHouse, managerH* cr, int min = 8, int max = 50)
{
	int startI = 0, startJ = 0;
	int countColum = 0, countRows = 0;
	int tempLine = 0;
	bool flag = false;
	while (min)
	{
		countColum = 0;
		countRows = 0;
		tempLine = 0;
		flag = false;
		for (int i = 1; i < row; i++)
		{

			for (int j = 1; j < colum; j++)
			{
				if (countColum >= max)
				{
					flag = true; break;
				}
				if (countColum && !pDown[i][j])
					countColum++;
				else if (countColum >= min && pDown[i][j])
				{
					flag = true; break;
				}
				else if (countColum && pDown[i][j])
					countColum = 0;
				if (!countColum && !pDown[i][j])
				{
					startI = i;
					startJ = j;
					countColum++;
				}
			}
			if (flag)break;
		}
		if (!flag) return false;
		for (int i = startI; i < row; i++)
		{
			tempLine = 0;
			for (int j = startJ; j < (startJ + countColum) && j < colum; j++)
			{
				if (pDown[i][j] == 0)
				{
					tempLine++;
					pDown[i][j] = -1;
				}
			}
			if (tempLine < countColum) break;
			countRows++;
			if (countRows >= max) break;
		}
		if (countRows >= min)
		{
			//вызываем создателя дома
			BuildHouse(pDown, pHouse, cr, startI, startJ, countColum, countRows, min, max);
			return true;
		}
	}
	return false;
}
//главный строитель домов
void BuildBuilding(int** pDown, int** pHouse, managerH* cr)
{
	while (FindPlaceForHouse(pDown, pHouse, cr));
}
void FreeBuilding(managerH* cr)
{
	if (cr->pFirst == NULL)
	{
		delete cr;
		return;
	}
	house* temp = cr->pFirst;
	house* tempNext = NULL;
	while (temp)
	{
		tempNext = temp->pNext;
		delete temp;
		temp = tempNext;
	}
	delete cr;
}

//--------------------------------NEWLEVELS=NULL-------------------------------
int** CreateNewBigLevel()
{
	int** ptr = new int* [row];
	for (int i = 0; i < row; i++)
	{
		ptr[i] = new int[colum];
		for (int j = 0; j < colum; j++)
			ptr[i][j] = 0;
	}
	return ptr;
}
void FreeBigLevel(int** ptr)
{
	for (int i = 0; i < row; i++)
		delete[] ptr[i];
	delete[] ptr;
}

//--------------------------------ROADS----------------------------------------
//дорога без обочины_ тест
int _Road(int** ptr, int iStart, int jStart)
{
	int count = 0;
	int tempI = iStart, tempJ = jStart;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (tempI > 5 && tempI < row - 6 && tempJ>7 && tempJ < colum - 8 && ptr[tempI][tempJ] != road)
			{
				ptr[tempI][tempJ] = road;
				count++;
			}
			tempJ++;
		}
		tempI++;
		tempJ = jStart;
	}
	return count;
}
//строим дорогу
//участок 5 на 5
int Road(int** ptr, int iStart, int jStart)
{
	int count = 0;
	int tempI = iStart - 2, tempJ = jStart - 2;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			if (tempI > 5 && tempI < row - 6 && tempJ>7 && tempJ < colum - 8)
			{
				if (ptr[tempI][tempJ] != road)
				{
					if (tempI >= iStart && tempI < iStart + 5 && tempJ >= jStart && tempJ < jStart + 7)
					{
						ptr[tempI][tempJ] = road;
						count++;
					}
					else
						ptr[tempI][tempJ] = roadSide;
				}

			}
			tempJ++;
		}
		tempI++;
		tempJ = jStart - 2;
	}
	return count;
}
//окружная
void AroundRoad(int** ptr, int vector[][2], unsigned int& temp)
{
	int rez;
	int iStart = 6, jStart = 8;
	int vec = 0;
	for (; vec < 4;)
	{
		rez = Road(ptr, iStart, jStart);
		temp += rez;
		while (rez == 35)
		{
			jStart += vector[vec][1];
			iStart += vector[vec][0];
			rez = Road(ptr, iStart, jStart);
			temp += rez;
		}
		vec++;
		if (vec == 1) { iStart = 11; jStart = colum - 15; }
		else if (vec == 2) { iStart = row - 11; jStart = colum - 22; }
		else if (vec == 3) { iStart = row - 16; jStart = 8; }
	}
}
void _AroundRoad(int** ptr, int vector[][2], unsigned int& temp)
{
	int rez;
	int iStart = 1, jStart = 1;
	int vec = 0;
	for (; vec < 4;)
	{
		rez = Road(ptr, iStart, jStart);
		temp += rez;
		while (rez == 35)
		{
			jStart += vector[vec][1];
			iStart += vector[vec][0];
			rez = Road(ptr, iStart, jStart);
			temp += rez;
		}
		vec++;
		if (vec == 1) { iStart = 6; jStart = colum - 8; }
		else if (vec == 2) { iStart = row - 6; jStart = colum - 15; }
		else if (vec == 3) { iStart = row - 11; jStart = 1; }
	}
}
//проверка на наличие соседних дорог
bool OtherRoad(int** ptr, int iStart, int jStart, int vec)
{
	switch (vec)
	{
	case 0:
		for (int i = iStart - 10; i < iStart + 15; i++)
		{
			for (int j = jStart; j < jStart + 21; j++)
			{
				if (i >= 0 && j >= 0 && i < row && j < colum)
				{
					if (ptr[i][j] == road)
						return false;
				}
			}
		}break;
	case 1:
		for (int i = iStart; i < iStart + 15; i++)
		{
			for (int j = jStart - 14; j < jStart + 21; j++)
			{
				if (i >= 0 && j >= 0 && i < row && j < colum)
				{
					if (ptr[i][j] == road)
						return false;
				}
			}
		}break;
	case 2:
		for (int i = iStart - 10; i < iStart + 15; i++)
		{
			for (int j = jStart - 14; j < jStart; j++)
			{
				if (i >= 0 && j >= 0 && i < row && j < colum)
				{
					if (ptr[i][j] == road)
						return false;
				}
			}
		}break;
	case 3:
		for (int i = iStart - 10; i < iStart; i++)
		{
			for (int j = jStart - 14; j < jStart + 21; j++)
			{
				if (i >= 0 && j >= 0 && i < row && j < colum)
				{
					if (ptr[i][j] == road)
						return false;
				}
			}
		}break;
	}
	return true;
}
//внутренние дороги
void InsideRoad(int** ptr, managerTL* cr, int vector[][2], int iStart, int jStart, int vec, unsigned int& temp)
{
	int rez = 0;
	int flagPovorot = 1;
	int tempVec = vec, tempVec2;
	bool flag = false;

	rez = Road(ptr, iStart, jStart);
	temp += rez;
	while (rez == 35)
	{
		jStart += vector[tempVec][1];
		iStart += vector[tempVec][0];

		rez = Road(ptr, iStart, jStart);
		temp += rez;

		flagPovorot = rand() % 6;
		flag = false;
		if (!flagPovorot)
		{
			flagPovorot = rand() % 3;
			switch (flagPovorot)
			{
			case 0:
				tempVec2 = !vec % 2 + 2 * rand() % 2;
				if (OtherRoad(ptr, iStart + vector[tempVec2][0], jStart + vector[tempVec2][1], tempVec2))
					tempVec = tempVec2;
				break;
			case 1:
				tempVec2 = !vec % 2 + 2 * rand() % 2;
				if (OtherRoad(ptr, iStart + vector[tempVec2][0], jStart + vector[tempVec2][1], tempVec2))
				{
					InsideRoad(ptr, cr, vector, iStart + vector[tempVec2][0], jStart + vector[tempVec2][1], tempVec2, temp);
					flag = true;
				}
				break;
			case 2:
				if (tempVec % 2)
				{
					tempVec2 = 0;
					if (OtherRoad(ptr, iStart + vector[tempVec2][0], jStart + vector[tempVec2][1], tempVec2))
					{
						InsideRoad(ptr, cr, vector, iStart + vector[tempVec2][0], jStart + vector[tempVec2][1], tempVec2, temp);
						flag = true;
					}
					tempVec2 = 2;
					if (OtherRoad(ptr, iStart + vector[tempVec2][0], jStart + vector[tempVec2][1], tempVec2))
					{
						InsideRoad(ptr, cr, vector, iStart + vector[tempVec2][0], jStart + vector[tempVec2][1], tempVec2, temp);
						if (flag)
							PositionTraf(cr, iStart, jStart, tempVec);
						flag = true;
					}
				}
				else
				{
					tempVec2 = 1;
					if (OtherRoad(ptr, iStart + vector[tempVec2][0], jStart + vector[tempVec2][1], tempVec2))
					{
						InsideRoad(ptr, cr, vector, iStart + vector[tempVec2][0], jStart + vector[tempVec2][1], tempVec2, temp);
						flag = true;
					}
					tempVec2 = 3;
					if (OtherRoad(ptr, iStart + vector[tempVec2][0], jStart + vector[tempVec2][1], tempVec2))
					{
						InsideRoad(ptr, cr, vector, iStart + vector[tempVec2][0], jStart + vector[tempVec2][1], tempVec2, temp);
						if (flag)
							PositionTraf(cr, iStart, jStart, tempVec);
						flag = true;
					}
				}
				break;
			}
		}
		if (flag)
		{
			if (!OtherRoad(ptr, iStart + vector[tempVec][0], jStart + vector[tempVec][1], tempVec)) return;

			PositionTraf(cr, iStart, jStart, tempVec);
		}
	}
	PositionTraf(cr, iStart, jStart, tempVec, rez);
}
/*void __InsideRoad( int ** ptr, int vector[][2], int iStart, int jStart, int vec, unsigned int & temp)
{
int rez=0;
int flagPovorot=1;
int tempVec=vec, tempVec2;
bool flag=false;

rez=Road(ptr,  iStart, jStart);
temp+=rez;
while(rez==35)
{
jStart+=vector[tempVec][1];
iStart+=vector[tempVec][0];

rez=Road(ptr, iStart, jStart);
temp+=rez;

flagPovorot=rand()%6;
flag=false;
if(!flagPovorot)
{
flagPovorot=rand()%3;
switch(flagPovorot)
{
case 0:
tempVec2=!vec%2+2*rand()%2;
if(OtherRoad(ptr, iStart+vector[tempVec2][0], jStart+vector[tempVec2][1], tempVec2))
tempVec=tempVec2;
break;
case 1:
tempVec2=!vec%2+2*rand()%2;
if(OtherRoad(ptr, iStart+vector[tempVec2][0], jStart+vector[tempVec2][1], tempVec2))
{
InsideRoad(ptr, vector, iStart+vector[tempVec2][0], jStart+vector[tempVec2][1], tempVec2, temp);
flag=true;
}
break;
case 2:
if(tempVec%2)
{
tempVec2=0;
if(OtherRoad(ptr, iStart+vector[tempVec2][0], jStart+vector[tempVec2][1], tempVec2))
{
InsideRoad(ptr, vector, iStart+vector[tempVec2][0], jStart+vector[tempVec2][1], tempVec2, temp);
flag=true;
}
tempVec2=2;
if(OtherRoad(ptr, iStart+vector[tempVec2][0], jStart+vector[tempVec2][1], tempVec2))
{
InsideRoad(ptr, vector, iStart+vector[tempVec2][0], jStart+vector[tempVec2][1], tempVec2, temp);
flag=true;
}
}
else
{
tempVec2=1;
if(OtherRoad(ptr, iStart+vector[tempVec2][0], jStart+vector[tempVec2][1], tempVec2))
{
InsideRoad(ptr, vector, iStart+vector[tempVec2][0], jStart+vector[tempVec2][1], tempVec2, temp);
flag=true;
}
tempVec2=3;
if(OtherRoad(ptr, iStart+vector[tempVec2][0], jStart+vector[tempVec2][1], tempVec2))
{
InsideRoad(ptr, vector, iStart+vector[tempVec2][0], jStart+vector[tempVec2][1], tempVec2, temp);
flag=true;
}
}
break;
}
}
if(flag)
{
if(!OtherRoad(ptr, iStart+vector[tempVec2][0], jStart+vector[tempVec2][1], tempVec2)) break;
}
}
}*/
void FindRoadStart(int** ptr, int& iStart, int& jStart, int& vec)
{
	do
	{
		vec = rand() % 4;
		switch (vec)
		{
		case 0:
			jStart = 15; iStart = 16 + rand() % (row - 26); break;
		case 1:
			iStart = 11; jStart = 22 + rand() % (colum - 29); break;
		case 2:
			jStart = colum - 22; iStart = 16 + rand() % (row - 26); break;
		case 3:
			iStart = row - 16; jStart = 22 + rand() % (colum - 29); break;
		}
	} while (!OtherRoad(ptr, iStart, jStart, vec));
}
void _FindRoadStart(int** ptr, int& iStart, int& jStart, int& vec)
{
	do
	{
		vec = rand() % 4;
		switch (vec)
		{
		case 0:
			jStart = 8; iStart = rand() % row; break;
		case 1:
			iStart = 6; jStart = rand() % colum; break;
		case 2:
			jStart = colum - 15; iStart = rand() % row; break;
		case 3:
			iStart = row - 11; jStart = rand() % colum; break;
		}
	} while (!OtherRoad(ptr, iStart, jStart, vec));
}
//главный строитель дорог
void BuildRoad(int** ptr, managerTL* cr)
{
	int vector[4][2] = { 0,7,5,0,0,-7,-5,0 };
	unsigned int numberOfRoad = row * colum * 20 / 100;
	unsigned int temp = 0;

	int iStart;
	int jStart;
	int vec;

	AroundRoad(ptr, vector, temp);
	FindRoadStart(ptr, iStart, jStart, vec);
	PositionTrafStart(cr, iStart, jStart, vec);
	InsideRoad(ptr, cr, vector, iStart, jStart, vec, temp);
	while (temp < numberOfRoad)
	{
		FindRoadStart(ptr, iStart, jStart, vec);
		PositionTrafStart(cr, iStart, jStart, vec);
		InsideRoad(ptr, cr, vector, iStart, jStart, vec, temp);
	}

}

//----------------------------------TREES-------------------------------------
//строитель деревьев
void BuildTreeAround(int** pDown)
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < colum; j++)
			if (i<6 || i>row - 7 || j<8 || j>colum - 9) pDown[i][j] = tree;
}

void BuildTree(int** ptr)
{
	bool flag1 = true;
	int count = 0;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < row; j++)
		{
			if (flag1)
			{
				if (ptr[i][j] == 0 || ptr[i][j] == -1)
				{
					ptr[i][j] = tree;
					flag1 = false;
					count = 0;
				}
			}
			else
			{
				if (ptr[i][j] == 0 || ptr[i][j] == -1)
				{
					ptr[i][j] = roadSide;
					if (count)
						flag1 = true;
					else count++;
				}
			}
		}
	}
}


//---------------------------------CROSSWALK----------------------------------
//размещение пешеходного перехода по флагу если тру то вертикальный если фалс то горизонтальный
void CreatCrosswalk(int** pDown, int iStart, int jStart, bool vectorOfCross = false)
{
	if (vectorOfCross)
	{
		for (int i = iStart; i < iStart + 5; i++)
		{
			for (int j = jStart; j < jStart + 2; j++)
			{
				if (i == iStart + 1 || i == iStart + 3) pDown[i][j] = crosswalkGrey;
				else pDown[i][j] = crosswalkWhite;
			}
		}
		return;
	}
	for (int i = iStart; i < iStart + 2; i++)
	{
		for (int j = jStart; j < jStart + 7; j++)
		{
			if (j == jStart + 1 || j == jStart + 3 || j == jStart + 5) pDown[i][j] = crosswalkGrey;
			else pDown[i][j] = crosswalkWhite;
		}
	}

}
//определение места под пешеходный переход
void AnalizPositionOfCross(int** pDown, managerTL* ct)
{
	int tempI, tempJ;
	trafLight* temp = ct->pFirst;
	while (temp)
	{
		//left
		tempI = temp->centrI; tempJ = temp->centrJ - 4;
		if (pDown[tempI - 3][tempJ] == roadSide && pDown[tempI][tempJ] == road && pDown[tempI + 3][tempJ] == roadSide)
			CreatCrosswalk(pDown, tempI - 2, tempJ - 1, true);
		//right
		tempI = temp->centrI; tempJ = temp->centrJ + 4;
		if (pDown[tempI - 3][tempJ] == roadSide && pDown[tempI][tempJ] == road && pDown[tempI + 3][tempJ] == roadSide)
			CreatCrosswalk(pDown, tempI - 2, tempJ, true);
		//up
		tempI = temp->centrI - 3; tempJ = temp->centrJ;
		if (pDown[tempI][tempJ - 4] == roadSide && pDown[tempI][tempJ] == road && pDown[tempI][tempJ + 4] == roadSide)
			CreatCrosswalk(pDown, tempI - 1, tempJ - 3);
		//down
		tempI = temp->centrI + 3; tempJ = temp->centrJ;
		if (pDown[tempI][tempJ - 4] == roadSide && pDown[tempI][tempJ] == road && pDown[tempI][tempJ + 4] == roadSide)
			CreatCrosswalk(pDown, tempI, tempJ - 3);
		temp = temp->pNext;
	}
}

//---------------------------------GENERAL GENERATOR--------------------------


void MaineBuilder(int** pDown, int** pHouse, int** pCars, int** pTraf, int** pMen, managerH* ch, managerTL* ct, managerC* cr, managerM* cm)
{
	std::cout << "Creating roads...";
	BuildRoad(pDown, ct);
	std::cout << "COMPLITE!\nCreating building's...";
	BuildBuilding(pDown, pHouse, ch);
	std::cout << "COMPLITE!\nCreating tree's...";
	BuildTreeAround(pDown);
	BuildTree(pDown);
	std::cout << "COMPLITE!\nCreating crosswalk's...";
	AnalizPositionOfCross(pDown, ct);
	std::cout << "COMPLITE!\nCreating car's...";
	BuildCars(cr, pDown, pCars, ch->count);
	std::cout << "COMPLITE!\nCreating traffingLight's...";
	BuildTraf(ct, pTraf);
	std::cout << "COMPLITE!\nCreating people...";
	BuildMen(cm, pDown, pMen, ch->count);
	std::cout << "COMPLITE!\nCity complite. Press any key to start!\n";
	_getch();
}
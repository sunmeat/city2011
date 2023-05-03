//-----------------------------------TRAFFICLIGHTS---------------------------
//переключение светофоров test
void _testSwitchTraf(managerTL* cr)
{
	cr->horizont = redTraf;
	cr->vertical = redTraf;
}
void SwitchTraf(managerTL* cr)
{
	static int countLight = 0;
	switch (countLight)
	{
	case 0:
	case 3:
		cr->horizont = yellowTraf; cr->vertical = yellowTraf;
		break;
	case 1:
	case 2:
		cr->horizont = redTraf; cr->vertical = greenTraf;
		break;
	case 4:
	case 5:
		cr->horizont = greenTraf; cr->vertical = redTraf;
		break;
	}
	if (countLight >= 5) countLight = 0;
	else countLight++;
}
void testSwitchTraf(managerTL* cr)
{
	static int countLight = 0;
	//static int flag=0;
	if (countLight % 4)
	{
		switch (cr->horizont)
		{
		case greenTraf:
			cr->horizont = yellowTraf; cr->vertical = yellowTraf;
			break;
		case redTraf:
			cr->horizont = yellowTraf; cr->vertical = yellowTraf;
			break;
		case yellowTraf:
			cr->horizont = redTraf; cr->vertical = greenTraf;
			break;
		}
		//flag=0;
	}
	else
	{
		switch (cr->horizont)
		{
		case greenTraf:
			cr->horizont = yellowTraf; cr->vertical = yellowTraf;
			break;
		case redTraf:
			cr->horizont = yellowTraf; cr->vertical = yellowTraf;
			break;
		case yellowTraf:
			cr->horizont = greenTraf; cr->vertical = redTraf;
			break;
		}
		//flag=1;
	}
	if (countLight >= 4)
	{
		countLight = 0; return;
	}
	countLight++;
}


//-----------------------------------CARS-------------------------------------

//анализатор возможности движения вперед на 1 шаг TODO учет светофора(баг при просмотре светофора вылазит за прдел массива)
bool AnalizeMoveForward(int** pCars, int** pTraf, int** pDown, int** pMen, managerTL* cr, int startI, int startJ, movingVec vec)
{
	switch (vec)
	{
	case rightMove:
		if (!(pDown[startI][startJ + 4] == road || pDown[startI][startJ + 4] == crosswalkGrey || pDown[startI][startJ + 4] == crosswalkWhite) || pCars[startI][startJ + 4] || pCars[startI + 1][startJ + 4] || pCars[startI + 1][startJ + 5] || pCars[startI][startJ + 5]
			|| (pTraf[startI - 1][startJ + 8] == trafficLightVertic && (cr->vertical == redTraf || cr->vertical == yellowTraf)) || pMen[startI][startJ + 4] || pMen[startI + 1][startJ + 4]) return false;
		break;
	case leftMove:
		if (!(pDown[startI][startJ - 1] == road || pDown[startI][startJ - 1] == crosswalkGrey || pDown[startI][startJ - 1] == crosswalkWhite) || pCars[startI][startJ - 1] || pCars[startI + 1][startJ - 1] || pCars[startI + 1][startJ - 2] || pCars[startI][startJ - 2]
			|| (pTraf[startI + 2][startJ - 5] == trafficLightVertic && (cr->vertical == redTraf || cr->vertical == yellowTraf)) || pMen[startI][startJ - 1] || pMen[startI + 1][startJ - 1]) return false;
		break;
	case upMove:
		if (!(pDown[startI - 1][startJ] == road || pDown[startI - 1][startJ] == crosswalkGrey || pDown[startI - 1][startJ] == crosswalkWhite) || pCars[startI - 1][startJ] || pCars[startI - 1][startJ + 2] || pCars[startI - 2][startJ] || pCars[startI - 2][startJ + 2]
			|| (pTraf[startI - 4][startJ - 1] == trafficLightHorizont && (cr->horizont == redTraf || cr->horizont == yellowTraf)) || pMen[startI - 1][startJ] || pMen[startI - 1][startJ + 1] || pMen[startI - 1][startJ + 2]) return false;
		break;
	case downMove:
		if (!(pDown[startI + 3][startJ] == road || pDown[startI + 3][startJ] == crosswalkGrey || pDown[startI + 3][startJ] == crosswalkWhite) || pCars[startI + 3][startJ] || pCars[startI + 4][startJ] || pCars[startI + 3][startJ + 2] || pCars[startI + 4][startJ + 2]
			|| (pTraf[startI + 6][startJ + 3] == trafficLightHorizont && (cr->horizont == redTraf || cr->horizont == yellowTraf)) || pMen[startI + 3][startJ] || pMen[startI + 3][startJ + 1] || pMen[startI + 3][startJ + 2]) return false;
		break;
	}
	return true;
}
//передвижение на 1 шаг вперед
void MoveForvardOneStep(int** pCars, car* pc)
{
	switch (pc->defaultVec)
	{
	case rightMove:
		for (int i = pc->startI; i < pc->startI + 2; i++)
		{
			pCars[i][pc->startJ + 4] = pc->numberOfCar;
			pCars[i][pc->startJ] = 0;
		}
		pc->startJ++;
		break;
	case leftMove:
		for (int i = pc->startI; i < pc->startI + 2; i++)
		{
			pCars[i][pc->startJ - 1] = pc->numberOfCar;
			pCars[i][pc->startJ + 3] = 0;
		}
		pc->startJ--;
		break;
	case upMove:
		for (int i = pc->startJ; i < pc->startJ + 3; i++)
		{
			pCars[pc->startI - 1][i] = pc->numberOfCar;
			pCars[pc->startI + 2][i] = 0;
		}
		pc->startI--;
		break;
	case downMove:
		for (int i = pc->startJ; i < pc->startJ + 3; i++)
		{
			pCars[pc->startI + 3][i] = pc->numberOfCar;
			pCars[pc->startI][i] = 0;
		}
		pc->startI++;
		break;
	}
}

//анализатор поворотов по направлениям
movingVec AnalizePossiblePriorityMoveUp(car* pc, int** pDown)
{
	int keyLeft = 0, keyRight = 0, keyForvard = 0;
	if ((pDown[pc->startI][pc->startJ + 3] == road || pDown[pc->startI][pc->startJ + 3] == crosswalkGrey || pDown[pc->startI][pc->startJ + 3] == crosswalkWhite) && pDown[pc->startI + 1][pc->startJ + 3] == roadSide || pDown[pc->startI + 1][pc->startJ + 3] == tree) keyRight = 1;
	if (pDown[pc->startI][pc->startJ - 5] == road || pDown[pc->startI][pc->startJ + 3] == crosswalkGrey || pDown[pc->startI][pc->startJ + 3] == crosswalkWhite) keyLeft = 4;
	if (pDown[pc->startI - 5][pc->startJ] == road || pDown[pc->startI - 5][pc->startJ] == crosswalkGrey || pDown[pc->startI - 5][pc->startJ] == crosswalkWhite) keyForvard = 2;
	switch (keyLeft + keyRight + keyForvard)
	{
	case 1:
		return rightMove;
	case 2:
		return upMove;
	case 3:
		return rand() % 2 ? upMove : rightMove;
	case 4:
		return leftMove;
	case 5:
		return rand() % 2 ? leftMove : rightMove;
	case 6:
		return rand() % 2 ? leftMove : upMove;
	case 7:
		switch (rand() % 3)
		{
		case 0:
			return upMove;
		case 1:
			return leftMove;
		case 2:
			return rightMove;
		}
	}
	return upMove;
}
movingVec AnalizePossiblePriorityMoveDown(car* pc, int** pDown)
{
	int keyLeft = 0, keyRight = 0, keyForvard = 0;
	if ((pDown[pc->startI + 2][pc->startJ - 1] == road || pDown[pc->startI + 2][pc->startJ - 1] == crosswalkGrey || pDown[pc->startI + 2][pc->startJ - 1] == crosswalkWhite) && (pDown[pc->startI + 1][pc->startJ - 1] == roadSide || pDown[pc->startI + 1][pc->startJ - 1] == tree)) keyLeft = 4;
	if (pDown[pc->startI + 2][pc->startJ + 7] == road || pDown[pc->startI + 2][pc->startJ + 7] == crosswalkGrey || pDown[pc->startI + 2][pc->startJ + 7] == crosswalkWhite) keyRight = 1;
	if (pDown[pc->startI + 7][pc->startJ] == road || pDown[pc->startI + 7][pc->startJ] == crosswalkGrey || pDown[pc->startI + 7][pc->startJ] == crosswalkWhite) keyForvard = 2;
	switch (keyLeft + keyRight + keyForvard)
	{
	case 1:
		return rightMove;
	case 2:
		return downMove;
	case 3:
		return rand() % 2 ? downMove : rightMove;
	case 4:
		return leftMove;
	case 5:
		return rand() % 2 ? leftMove : rightMove;
	case 6:
		return rand() % 2 ? leftMove : downMove;
	case 7:
		switch (rand() % 3)
		{
		case 0:
			return downMove;
		case 1:
			return leftMove;
		case 2:
			return rightMove;
		}
	}
	return downMove;
}
movingVec AnalizePossiblePriorityMoveLeft(car* pc, int** pDown)
{
	int keyUp = 0, keyDown = 0, keyForvard = 0;
	if ((pDown[pc->startI - 1][pc->startJ] == road || pDown[pc->startI - 1][pc->startJ] == crosswalkGrey || pDown[pc->startI - 1][pc->startJ] == crosswalkWhite) && (pDown[pc->startI - 1][pc->startJ - 1] == roadSide || pDown[pc->startI - 1][pc->startJ - 1] == tree)) keyUp = 1;
	if (pDown[pc->startI + 5][pc->startJ] == road || pDown[pc->startI + 5][pc->startJ] == crosswalkGrey || pDown[pc->startI + 5][pc->startJ] == crosswalkWhite) keyDown = 2;
	if (pDown[pc->startI][pc->startJ - 7] == road || pDown[pc->startI][pc->startJ - 7] == crosswalkGrey || pDown[pc->startI][pc->startJ - 7] == crosswalkWhite) keyForvard = 4;
	switch (keyUp + keyDown + keyForvard)
	{
	case 1:
		return upMove;
	case 2:
		return downMove;
	case 3:
		return rand() % 2 ? downMove : upMove;
	case 4:
		return leftMove;
	case 5:
		return rand() % 2 ? leftMove : upMove;
	case 6:
		return rand() % 2 ? leftMove : downMove;
	case 7:
		switch (rand() % 3)
		{
		case 0:
			return downMove;
		case 1:
			return leftMove;
		case 2:
			return upMove;
		}
	}
	return leftMove;
}
movingVec AnalizePossiblePriorityMoveRight(car* pc, int** pDown)
{
	int keyUp = 0, keyDown = 0, keyForvard = 0;
	if (pDown[pc->startI - 4][pc->startJ + 3] == road || pDown[pc->startI - 4][pc->startJ + 3] == crosswalkGrey || pDown[pc->startI - 4][pc->startJ + 3] == crosswalkWhite) keyUp = 1;
	if ((pDown[pc->startI + 2][pc->startJ + 3] == road || pDown[pc->startI + 2][pc->startJ + 3] == crosswalkGrey || pDown[pc->startI + 2][pc->startJ + 3] == crosswalkWhite) && (pDown[pc->startI + 2][pc->startJ + 2] == roadSide || pDown[pc->startI + 2][pc->startJ + 2] == tree)) keyDown = 2;
	if (pDown[pc->startI][pc->startJ + 10] == road || pDown[pc->startI][pc->startJ + 10] == crosswalkGrey || pDown[pc->startI][pc->startJ + 10] == crosswalkWhite) keyForvard = 4;
	switch (keyUp + keyDown + keyForvard)
	{
	case 1:
		return upMove;
	case 2:
		return downMove;
	case 3:
		return rand() % 2 ? downMove : upMove;
	case 4:
		return rightMove;
	case 5:
		return rand() % 2 ? rightMove : upMove;
	case 6:
		return rand() % 2 ? rightMove : downMove;
	case 7:
		switch (rand() % 3)
		{
		case 0:
			return downMove;
		case 1:
			return rightMove;
		case 2:
			return upMove;
		}
	}
	return rightMove;
}

//анализ на возможность маневра(включение приоритета поворота или прямого проезда перекрестка)
bool AnalizePossiblePriorityMove(car* pc, int** pDown)
{
	movingVec temp = pc->defaultVec;
	switch (pc->defaultVec)
	{
	case upMove:
		temp = AnalizePossiblePriorityMoveUp(pc, pDown);
		break;
	case downMove:
		temp = AnalizePossiblePriorityMoveDown(pc, pDown);
		break;
	case leftMove:
		temp = AnalizePossiblePriorityMoveLeft(pc, pDown);
		break;
	case rightMove:
		temp = AnalizePossiblePriorityMoveRight(pc, pDown);
		break;
	}
	if (temp == pc->defaultVec) return false;
	pc->priorityVec = temp;
	return true;
}

//анализ на выполнение маневра(нахождение машины в нужной позиции для совершения маневра)
bool AnalizeDoPriorityMove(car* pc, int** pDown)
{
	switch (pc->defaultVec)
	{
	case upMove:
		switch (pc->priorityVec)
		{
		case leftMove:
			if ((pDown[pc->startI - 1][pc->startJ - 5] == roadSide || pDown[pc->startI - 1][pc->startJ - 5] == tree) && (pDown[pc->startI][pc->startJ - 5] == road || pDown[pc->startI][pc->startJ - 5] == crosswalkGrey || pDown[pc->startI][pc->startJ - 5] == crosswalkWhite)) return true;
			break;
		case rightMove:
			if ((pDown[pc->startI + 1][pc->startJ + 3] == road || pDown[pc->startI + 1][pc->startJ + 3] == crosswalkGrey || pDown[pc->startI + 1][pc->startJ + 3] == crosswalkWhite) && (pDown[pc->startI + 2][pc->startJ + 3] == tree || pDown[pc->startI + 2][pc->startJ + 3] == roadSide)) return true;
			break;
		}
		break;
	case downMove:
		switch (pc->priorityVec)
		{
		case leftMove:
			if ((pDown[pc->startI + 1][pc->startJ - 1] == road || pDown[pc->startI + 1][pc->startJ - 1] == crosswalkGrey || pDown[pc->startI + 1][pc->startJ - 1] == crosswalkWhite) && (pDown[pc->startI][pc->startJ - 1] == roadSide || pDown[pc->startI][pc->startJ - 1] == tree)) return true;
			break;
		case rightMove:
			if ((pDown[pc->startI + 3][pc->startJ + 7] == roadSide || pDown[pc->startI + 3][pc->startJ + 7] == tree) && (pDown[pc->startI + 2][pc->startJ + 7] == road || pDown[pc->startI + 2][pc->startJ + 7] == crosswalkGrey || pDown[pc->startI + 2][pc->startJ + 7] == crosswalkWhite)) return true;
			break;
		}
		break;
	case leftMove:
		switch (pc->priorityVec)
		{
		case upMove:
			if ((pDown[pc->startI - 1][pc->startJ + 2] == road || pDown[pc->startI - 1][pc->startJ + 2] == crosswalkGrey || pDown[pc->startI - 1][pc->startJ + 2] == crosswalkWhite) && (pDown[pc->startI - 1][pc->startJ + 3] == roadSide || pDown[pc->startI - 1][pc->startJ + 3] == tree)) return true;
			break;
		case downMove:
			if ((pDown[pc->startI + 5][pc->startJ - 1] == roadSide || pDown[pc->startI + 5][pc->startJ - 1] == tree) && (pDown[pc->startI + 5][pc->startJ] == road || pDown[pc->startI + 5][pc->startJ] == crosswalkGrey || pDown[pc->startI + 5][pc->startJ] == crosswalkWhite)) return true;
			break;
		}
		break;
	case rightMove:
		switch (pc->priorityVec)
		{
		case upMove:
			if ((pDown[pc->startI - 4][pc->startJ + 4] == roadSide || pDown[pc->startI - 4][pc->startJ + 4] == tree) && (pDown[pc->startI - 4][pc->startJ + 3] == road || pDown[pc->startI - 4][pc->startJ + 3] == crosswalkGrey || pDown[pc->startI - 4][pc->startJ + 3] == crosswalkWhite)) return true;
			break;
		case downMove:
			if ((pDown[pc->startI + 2][pc->startJ + 1] == road || pDown[pc->startI + 2][pc->startJ + 1] == crosswalkGrey || pDown[pc->startI + 2][pc->startJ + 1] == crosswalkWhite) && (pDown[pc->startI + 2][pc->startJ] == roadSide || pDown[pc->startI + 2][pc->startJ] == tree)) return true;
			break;
		}
		break;
	}
	return false;
}
//выполнение приоритетного действия
void DoPriorityMove(car* pc, int** pCars)
{
	switch (pc->defaultVec)
	{
	case upMove:
		switch (pc->priorityVec)
		{
		case leftMove:
			for (int i = 0; i < 3; i++)
				pCars[pc->startI + 2][pc->startJ + i] = 0;
			for (int i = 0; i < 2; i++)
				pCars[pc->startI + i][pc->startJ - 1] = pc->numberOfCar;
			pc->startJ--;
			break;
		case rightMove:
			for (int i = 0; i < 3; i++)
				pCars[pc->startI + 2][pc->startJ + i] = 0;
			for (int i = 0; i < 2; i++)
				pCars[pc->startI + i][pc->startJ + 3] = pc->numberOfCar;
			break;
		}
		break;
	case downMove:
		switch (pc->priorityVec)
		{
		case leftMove:
			for (int i = 0; i < 3; i++)
				pCars[pc->startI][pc->startJ + i] = 0;
			for (int i = 0; i < 2; i++)
				pCars[pc->startI + 1 + i][pc->startJ - 1] = pc->numberOfCar;
			pc->startI++;
			pc->startJ--;
			break;
		case rightMove:
			for (int i = 0; i < 3; i++)
				pCars[pc->startI][pc->startJ + i] = 0;
			for (int i = 0; i < 2; i++)
				pCars[pc->startI + 1 + i][pc->startJ + 3] = pc->numberOfCar;
			pc->startI++;
			break;
		}
		break;
	case rightMove:
		switch (pc->priorityVec)
		{
		case upMove:
			for (int i = 0; i < 3; i++)
				pCars[pc->startI - 1][pc->startJ + 1 + i] = pc->numberOfCar;
			for (int i = 0; i < 2; i++)
				pCars[pc->startI + i][pc->startJ] = 0;
			pc->startI--;
			pc->startJ++;
			break;
		case downMove:
			for (int i = 0; i < 3; i++)
				pCars[pc->startI + 2][pc->startJ + 1 + i] = pc->numberOfCar;
			for (int i = 0; i < 2; i++)
				pCars[pc->startI + i][pc->startJ] = 0;
			pc->startJ++;
			break;
		}
		break;
	case leftMove:
		switch (pc->priorityVec)
		{
		case upMove:
			for (int i = 0; i < 3; i++)
				pCars[pc->startI + 2][pc->startJ + i] = pc->numberOfCar;
			for (int i = 0; i < 2; i++)
				pCars[pc->startI + i][pc->startJ + 3] = 0;
			break;
		case downMove:
			for (int i = 0; i < 3; i++)
				pCars[pc->startI + 2][pc->startJ + i] = pc->numberOfCar;
			for (int i = 0; i < 2; i++)
				pCars[pc->startI + i][pc->startJ + 3] = 0;
			break;
		}
		break;
	}
	pc->defaultVec = pc->priorityVec;
	pc->flagPriority = false;
	pc->countStepAfterPriority = 0;
}
//Помощь при долгом стопе(костыль) принуждение повернуть
void HelpWhenCarStop(car* pc, int** pCars)
{
	switch (pc->defaultVec)
	{
	case upMove:
		pc->priorityVec = leftMove; break;
	case downMove:
		pc->priorityVec = rightMove; break;
	case leftMove:
		pc->priorityVec = downMove; break;
	case rightMove:
		pc->priorityVec = upMove; break;
	}
	DoPriorityMove(pc, pCars);
}
//анализ на ситуацию когда машина упирается в стенку
bool AnalizeLookingForward(car* pc, int** pDown)
{
	switch (pc->defaultVec)
	{
	case upMove:
		if (pDown[pc->startI - 1][pc->startJ] == tree || pDown[pc->startI - 1][pc->startJ] == roadSide) return true;
		break;
	case downMove:
		if (pDown[pc->startI + 3][pc->startJ] == tree || pDown[pc->startI + 3][pc->startJ] == roadSide) return true;
		break;
	case rightMove:
		if (pDown[pc->startI][pc->startJ + 4] == tree || pDown[pc->startI][pc->startJ + 4] == roadSide) return true;
		break;
	case leftMove:
		if (pDown[pc->startI][pc->startJ - 1] == tree || pDown[pc->startI][pc->startJ - 1] == roadSide) return true;
		break;
	}
	return false;
}
//движение по умолчанию с набором скорости и переключением на приоритет
void DefaultCarMove(car* pc, managerTL* cr, int** pCars, int** pTraf, int** pDown, int** pMen, bool flag = false)
{

	if (flag)
	{
		if (AnalizeDoPriorityMove(pc, pDown))
		{
			DoPriorityMove(pc, pCars);
			return;
		}
	}

	int tempSpeed = 0;
	int countStep = 0;
	switch (pc->carSpeed)
	{
	case stop:
		tempSpeed = 1; break;
	case lightMove:
		tempSpeed = 2; break;
	case middleMove:
	case fastMove:
		tempSpeed = 3; break;
	}
	for (int i = 0; i < tempSpeed; i++)
	{
		if (AnalizeMoveForward(pCars, pTraf, pDown, pMen, cr, pc->startI, pc->startJ, pc->defaultVec))
		{
			MoveForvardOneStep(pCars, pc);
			countStep++;
		}
		if (!flag)
		{
			pc->flagPriority = AnalizePossiblePriorityMove(pc, pDown); flag = true;
		}
		if (flag)
		{
			if (AnalizeDoPriorityMove(pc, pDown))
			{
				DoPriorityMove(pc, pCars);
				return;
			}

		}

	}
	switch (countStep)
	{
	case 0:
		pc->carSpeed = stop;
		if (pc->countOfStop >= STOPTIME) HelpWhenCarStop(pc, pCars);
		else if (AnalizeLookingForward(pc, pDown)) HelpWhenCarStop(pc, pCars);
		else pc->countOfStop++;
		break;
	case 1:
		pc->carSpeed = lightMove;
		pc->countOfStop = 0;
		break;
	case 2:
		pc->carSpeed = middleMove;
		pc->countOfStop = 0;
		break;
	case 3:
		pc->carSpeed = fastMove;
		pc->countOfStop = 0;
		break;
	}
}
void _DefaultCarMove(car* pc, managerTL* cr, int** pCars, int** pTraf, int** pDown, int** pMen, bool flag = false)
{
	int tempSpeed = 0;
	int countStep = 0;
	switch (pc->carSpeed)
	{
	case stop:
		tempSpeed = 1; break;
	case lightMove:
		tempSpeed = 2; break;
	case middleMove:
	case fastMove:
		tempSpeed = 3; break;
	}
	for (int i = 0; i < tempSpeed; i++)
	{
		if (AnalizeMoveForward(pCars, pTraf, pDown, pMen, cr, pc->startI, pc->startJ, pc->defaultVec))
		{
			MoveForvardOneStep(pCars, pc);
			countStep++;
			pc->countStepAfterPriority++;
		}
	}
	switch (countStep)
	{
	case 0:
		pc->carSpeed = stop;
		if (pc->countOfStop >= STOPTIME) HelpWhenCarStop(pc, pCars);
		else if (AnalizeLookingForward(pc, pDown)) HelpWhenCarStop(pc, pCars);
		else pc->countOfStop++;
		break;
	case 1:
		pc->carSpeed = lightMove;
		pc->countOfStop = 0;
		break;
	case 2:
		pc->carSpeed = middleMove;
		pc->countOfStop = 0;
		break;
	case 3:
		pc->carSpeed = fastMove;
		pc->countOfStop = 0;
		break;
	}
}
void GeneralMoveCar(managerC* cr, managerTL* ct, int** pCars, int** pTraf, int** pDown, int** pMen)
{
	car* temp = cr->pFirst;
	while (temp)
	{
		if (temp->countStepAfterPriority >= 3)
		{
			if (temp->flagPriority)
			{
				DefaultCarMove(temp, ct, pCars, pTraf, pDown, pMen, true);
			}
			else
			{
				if (AnalizePossiblePriorityMove(temp, pDown))
				{
					temp->flagPriority = true;
					DefaultCarMove(temp, ct, pCars, pTraf, pDown, pMen, true);
				}
				DefaultCarMove(temp, ct, pCars, pTraf, pDown, pMen);
			}
		}
		else _DefaultCarMove(temp, ct, pCars, pTraf, pDown, pMen);
		temp = temp->pNext;
	}
}

//---------------------------------------PEOPLE----------------------------------------
//тест режим свободной прогулки без пешеходных переходов и без заходов в дома
void testFreeWalkingPeople(managerM* cm, managerTL* ct, int** pDown, int** pMen, int** pCars)
{
	men* temp = cm->pFirst;
	while (temp)
	{
		int tempI = temp->startI + movVector[temp->vectorOfMove][0];
		int tempJ = temp->startJ + movVector[temp->vectorOfMove][1];
		if (pMen[tempI][tempJ] == 0)
		{
			if (pDown[tempI][tempJ] == roadSide)
			{
				pMen[tempI][tempJ] = pMen[temp->startI][temp->startJ];
				pMen[temp->startI][temp->startJ] = 0;
				temp->startI = tempI;
				temp->startJ = tempJ;
			}
			else if (pDown[tempI][tempJ] == crosswalkGrey || pDown[tempI][tempJ] == crosswalkWhite)
			{
				if (!pCars[tempI][tempJ])
				{
					if (pDown[temp->startI][temp->startJ] == crosswalkGrey || pDown[temp->startI][temp->startJ] == crosswalkWhite)
					{
						pMen[tempI][tempJ] = pMen[temp->startI][temp->startJ];
						pMen[temp->startI][temp->startJ] = 0;
						temp->startI = tempI;
						temp->startJ = tempJ;
					}
					else if (pDown[temp->startI][temp->startJ] == roadSide)
					{
						switch (temp->vectorOfMove)
						{
						case upMove:
						case downMove:
							if (ct->horizont == greenTraf)
							{
								pMen[tempI][tempJ] = pMen[temp->startI][temp->startJ];
								pMen[temp->startI][temp->startJ] = 0;
								temp->startI = tempI;
								temp->startJ = tempJ;
							}
							break;
						case leftMove:
						case rightMove:
							if (ct->vertical == greenTraf)
							{
								pMen[tempI][tempJ] = pMen[temp->startI][temp->startJ];
								pMen[temp->startI][temp->startJ] = 0;
								temp->startI = tempI;
								temp->startJ = tempJ;
							}
							break;
						}
					}
				}
			}
			else temp->vectorOfMove = RandomVector();
		}
		else temp->vectorOfMove = RandomVector();
		temp = temp->pNext;
	}
}


//---------------------------------------TAIMER----------------------------------------
void DisplayTimer(myTime* t)
{
	scrn.X = 91; scrn.Y = 2;
	SetConsoleCursorPosition(hCon, scrn);
	if (t->hours > 9)
		std::cout << t->hours << ":" << t->minutes << " ";
	else
		std::cout << "0" << t->hours << ":" << t->minutes << " ";

}
void StepMyTimer()
{
	long long int t;
	for (t = 1; t < DELAY; ++t);
}
void UpdateMyTimer(struct myTime* t)
{
	/*t->seconds++;
	if(t->seconds==60)
	{
	t->seconds=0;
	t->minutes++;
	}*/
	t->minutes++;
	if (t->minutes == 60)
	{
		t->minutes = 0;
		t->hours++;
	}
	if (t->hours == 24) t->hours = 0;
	//StepMyTimer();
}

void WriteLogOfCity(managerC* cr, int** pDown, char* path)
{
	FILE* fs = NULL;
	fopen_s(&fs, path, "w");
	if (!fs) return;
	car* temp = cr->pFirst;
	char buf[500] = {};
	while (temp)
	{
		if (temp->countOfStop)
		{
			_itoa_s(temp->numberOfCar, buf, 10);
			fputs(buf, fs);
			fputs("\n-koordinati:-> ", fs);
			_itoa_s(temp->startI, buf, 10);
			fputs(buf, fs);
			fputs(" - ", fs);
			_itoa_s(temp->startJ, buf, 10);
			fputs(buf, fs);
			fputs("\n-DEFvector-> ", fs);
			switch (temp->defaultVec)
			{
			case upMove:
				fputs("UP", fs); break;
			case downMove:
				fputs("DOWN", fs); break;
			case leftMove:
				fputs("LEFT", fs); break;
			case rightMove:
				fputs("RIGHT", fs); break;
			}
			if (temp->flagPriority) fputs("\n-flagPRIORITY-> TRUE", fs);
			else fputs("\n-flagPRIORITY-> FALSE", fs);
			fputs("\n-PRIORITYvector-> ", fs);
			switch (temp->priorityVec)
			{
			case upMove:
				fputs("UP", fs); break;
			case downMove:
				fputs("DOWN", fs); break;
			case leftMove:
				fputs("LEFT", fs); break;
			case rightMove:
				fputs("RIGHT", fs); break;
			}
			_itoa_s(temp->countOfStop, buf, 10);
			fputs("\n-OJIDANIE-> ", fs);
			fputs(buf, fs);
			fputs("\n-AnalizeToDoPriority-> ", fs);
			_itoa_s(int(AnalizeDoPriorityMove(temp, pDown)), buf, 10);
			fputs(buf, fs);
			fputs("\n\n", fs);
		}
		temp = temp->pNext;
	}
	fclose(fs);
}
//-------------------------------------GENERAL MANAGER ----------------------------------
void EveryBodyMoveNow(int** pCars, int** pTraf, int** pDown, int** pMen, managerTL* ct, managerC* cr, managerM* cm, myTime* t, char* path)
{
	static int count = 0;
	UpdateMyTimer(t);
	DisplayTimer(t);
	if (count == 3)
	{
		SwitchTraf(ct);
		count = 0;
	}
	GeneralMoveCar(cr, ct, pCars, pTraf, pDown, pMen);
	testFreeWalkingPeople(cm, ct, pDown, pMen, pCars);
	count++;
	WriteLogOfCity(cr, pDown, path);
}
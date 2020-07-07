#include "dSystems.h"

int leftL = 150, rightL = 3000;

bool Create_Task(double prob)
{
	
	if (rand() % 100 + 1 <= prob)
		return 0;
	else
		return 1;
}

bool Time_Out(dProcessor system[K], int time)
{
	int i, f = 0;
	for (i = 0;i < K;i++)
	{
		if (system[i].time >= time)
			f++;
	}
	if (f == 5)
		return true;
	else
		return false;
}

int Get_Best(dProcessor system[K], dTask *task, int time)
{
	int i, best, best_time;
	best = -1;
	best_time = time * 10;
	for (i = 0;i < K;i++)
	{
		if (task->proc[i] == true)
		{
			if (task->complexity / system[i].power + system[i].time + 1 < best_time)
			{
				best = i;
				best_time = task->complexity / system[i].power + system[i].time + 1;
			}
		}
	}
	return best;
}

//для випадку з перервами на планування вважаємо що черга задач розподілена на 5 різних
int Temp_Controller(dProcessor system[K], int tLimit, int taskChance, int tCont, int tProc)
{
	int tPeriod, result = 0;
	int best, i, cProc;
	dTask *current;

	cProc = 0;
	for (i = 1;i < K;i++)								//знаходження керуючого процесора
	{
		if (system[i].power > system[cProc].power)
			cProc = i;
	}

	int rest[K] = { 0 };								//масив часів що залишились для обробки всіх заданих завдань з даного моменту
	tPeriod = tCont + tProc;

	for (i = 0; i < tCont;i++)
	{
		best = -1;
		if (Create_Task(taskChance) == true)
		{
			current = new dTask(leftL, rightL);
			best = Get_Best(system, current, tLimit);
			if (best == -1)
			{
				for (i = 0; i < K;i++)
				{
					if (rest[i] > 0)
						rest[i]--;
				}
				//printf("no sufficent processor")
				continue;
			}
			system[best].update(current, tLimit);
			rest[best] += current->complexity / system[best].power + 1;
			//	printf("P %d : +%3d; time: %5d;    [%d %d %d %d %d] \n", best, current->complexity / system[best].power + 1, system[best].time, rest[0], rest[1], rest[2], rest[3], rest[4]);
		}
		if (best != -1)
		{
			for (i = 0; i < K;i++)							//компенсація часу роботи під час планування
			{
				if (i != best && rest[i] > 0)
					rest[i]--;
			}
		}
	}
	//system[cProc].time += tCont;
	for (i = 0; i < K;i++)
	{
		if (i == cProc && rest[i] < tProc)
			system[i].time += (tProc - rest[i]);
		else if (rest[i] < tPeriod)
			system[i].time += (tPeriod - rest[i]);		//додавання часу 
	}
	while (Time_Out(system, tLimit) == false)
	{
		for (i = 0; i < K;i++)							//відлік часу роботи за період
		{
			if (i == cProc)
			{
				if (rest[i] > tProc)
					rest[i] -= tProc;
				else
					rest[i] = 0;
			}
			else
			{
				if (rest[i] > tPeriod)
					rest[i] -= tPeriod;
				else
					rest[i] = 0;
			}
		}
		for (i = 0; i < tPeriod;i++)						//окрім випадку на початку роботи всі завдання згенеровані за період роздаються на початку періоду планування
		{
			if (Create_Task(taskChance) == true)
			{
				current = new dTask(leftL, rightL);
				best = Get_Best(system, current, tLimit);
				if (best == -1)
				{
					//printf("no sufficent processor")
					continue;
				}
				system[best].update(current, tLimit);
				rest[best] += current->complexity / system[best].power + 1;
				//	printf("P %d : +%3d; time: %5d;    [%d %d %d %d %d] \n", best, current->complexity / system[best].power + 1, system[best].time, rest[0], rest[1], rest[2], rest[3], rest[4]);
			}
		}
		//system[cProc].time += tCont;
		for (i = 0; i < K;i++)
		{
			if (i == cProc && rest[i] < tProc)
				system[i].time += (tProc - rest[i]);
			else if (rest[i] < tPeriod)
				system[i].time += (tPeriod - rest[i]);		//додавання часу 
		}

	}
	for (i = 0;i < K;i++)									//розрахунок кінцевої к-сті операцій 
	{
		if (i != cProc)
		{
			system[i].f_op -= (system[i].time - tLimit)*system[i].power;
			result += system[i].power*tLimit;
		}
		else
		{
			system[i].time += system[i].time*((float)tCont / tPeriod);
			system[i].f_op -= (system[i].time - tLimit)*system[i].power*((float)tProc/ tPeriod);
			result += system[i].power* tProc* (1 + tLimit / tPeriod);
		}
	}
	return result;											//очікувана реальна к-сть виконаних операцій 
}


int Sep_Controller(dProcessor system[K], int tLimit, int taskChance)
{
	int  timer, result = 0;
	int i, cProc, best;
	dTask *current;

	timer = 0;
	cProc = 0;
	for (i = 1;i < K;i++)
	{
		if (system[i].power < system[cProc].power)
			cProc = i;
	}
	system[cProc].time = ++tLimit;

	while (Time_Out(system, tLimit) == false)
	{
		timer++;
		if (Create_Task(taskChance) == true)
		{
			current = new dTask(leftL, rightL, cProc);
			best = Get_Best(system, current, tLimit);
			if (best == -1)
			{
				//printf("no sufficent processor")
				continue;
			}
			system[best].update(current, tLimit);
			//	printf("P %d : +%3d; time: %5d; \n", best, current->complexity / system[best].power + 1, system[best].time);
				/*best = -1;
				best_time = time*10;
				for (i = 0;i < K;i++)		//можна додати перевірку на процесор-планувальник
				{
					if (current->proc[i] == true)
					{
						if (current->complexity / system[i].power + system[i].time + 1 < best_time)
						{
							best = i;
							best_time = current->complexity / system[i].power + system[i].time + 1;
						}
					}
				}

				system[best].f_op += current->complexity;
				system[best].time = best_time;
				if (best_time<time)
					system[best].f_tsk++;*/
		}
		for (i = 0;i < K;i++)									//врахування часу очікування (опціональне)
		{
			if (i != cProc && system[i].time < timer)
				system[i].time++;
		}
	}
	for (i = 0;i < K;i++)
	{
		if (i != cProc)
		{
			system[i].f_op -= (system[i].time - tLimit)*system[i].power;
			result += system[i].power*tLimit;
		}
	}
	return result;												//очікувана реальна к-сть виконаних операцій 
}


int FIFO_NoController(dProcessor system[K], int tLimit, int taskChance)
{
	int i, target;
	int target_rest, result = 0;
	dTask *current;
	int skip = 0;
	int rest[K] = { 0 };

	while (Time_Out(system, tLimit) == false)
	{
		if (Create_Task(taskChance) == true)
		{
			if (skip != 0)
			{
				//printf("skip: %d \n", skip);
				skip = 0;
			}

			current = new dTask(leftL, rightL);
			target = -1;
			target_rest = 10 * tLimit;
			for (i = 0;i < K;i++)
			{
				if (current->proc[i] == true)
				{
					if (rest[i] < target_rest)
					{
						target = i;
						target_rest = rest[i];
					}
				}
			}
			if (target == -1)
			{
				//printf("no sufficent processor")
				continue;
			}
			for (i = 0;i < K;i++)
			{
				if (rest[i] < target_rest)
				{
					system[i].time += (target_rest - rest[i]);		//додавання часу очікуванння
					rest[i] = 0;									//відлік часу роботи
				}
				else
					rest[i] -= target_rest;							//відлік часу роботи
			}
			system[target].update(current, tLimit);
			rest[target] += current->complexity / system[target].power + 1;
			//	printf("P %d : +%3d; time: %5d;    [%d %d %d %d %d] \n", target, current->complexity / system[target].power + 1, system[target].time, rest[0], rest[1], rest[2], rest[3], rest[4]);
		}
		else
		{
			for (i = 0;i < K;i++)
			{
				if (rest[i] == 0)
					system[i].time++;
				else
					rest[i]--;							//відлік часу роботи
			}
			skip++;
		}
	}
	for (i = 0;i < K;i++)
	{
		system[i].f_op -= (system[i].time - tLimit)*system[i].power;
		result += system[i].power*tLimit;
	}
	return result;										//очікувана реальна к-сть виконаних операцій 
}
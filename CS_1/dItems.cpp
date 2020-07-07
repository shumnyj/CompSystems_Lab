#include "dItems.h"

dTask::dTask(int Lb, int Rb, int skip)
{
	int p=0;
	complexity = rand() % (Rb-Lb) + Lb;
	for (int i = 0;i < K;i++)
	{
		if (i != skip)
			// proc[i] = rand()%2;
			if (rand() % 100 + 1 - 12 * p > 50)
			{
				proc[i] = true;
				p++;
			}
			else
			{
				proc[i] = false;
				p--;
			}
		else
		{
			proc[i] = false;
			p--;
		}
	}
}


dProcessor::dProcessor()
{
	power = rand() % 75 + 15;
	f_op = 0;
	f_tsk = 0;
	last = NULL;
	time = 0;
}

void dProcessor::reset()
{
	if (last != NULL)
		delete last;
	last = NULL;
	f_op = 0;
	f_tsk = 0;
	time = 0;
}

int dProcessor::update(dTask *next, int tLimit)
{
	f_op += next->complexity;
	time += next->complexity/power +1;				//додавання часу обробки
	if (time < tLimit)
		f_tsk++;
	if (last != NULL)
		delete last;
	last = next;
	return 0;
}

void dProcessor::dataout() 
{
	using namespace std;
	cout << "Oper: " << f_op << "; Tasks: " << f_tsk << endl;
}
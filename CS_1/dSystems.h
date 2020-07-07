#pragma once

#include "dItems.h"
#include <ctime>

bool Create_Task(double prob);
bool Time_Out(dProcessor system[K], int time);
int Get_Best(dProcessor system[K], dTask *task, int time);
int Temp_Controller(dProcessor system[K], int tLimit, int taskChance, int tCont, int tProc);
int Sep_Controller(dProcessor system[K], int tLimit, int taskChance);
int FIFO_NoController(dProcessor system[K], int tLimit, int taskChance);
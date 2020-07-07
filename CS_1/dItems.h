#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#define K 5

class dTask
{
public:
	int complexity;
	bool proc[K];
	dTask(int Lb, int Rb, int skip=K);
};

class dProcessor
{
public:
	int power;
	int time;
	int f_op;
	int f_tsk;
	dTask *last;

	dProcessor();
	void reset();
	int update(dTask *next, int tLimit);
	void dataout();
};
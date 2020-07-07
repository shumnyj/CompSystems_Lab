#include "dSystems.h"

extern int leftL, rightL;

int main()
{
	using namespace std;
	dProcessor system[K];

	int i, total = 0, tasks =0,  expected = 0, peak = 0;
	char f;
	int maxtime = 10000;	
	int taskprob = 60;

	srand(time(0));
	cout.precision(6);
	printf("Modify launch? (Y/N)\n");
	cin >> f;
	if (f == 'Y' || f == 'y')
	{
		printf("Enter probability of  task creation (\%)\n");
		cin >> taskprob ;
		printf("Enter comlexity limits (min max)\n");
		cin >> leftL >> rightL;
		printf("Enter processors perfomance values (%d split by enter)\n", K);
		for (i = 0;i < K;i++)
			cin >> system[i].power;
	}
	printf("\nOperating time : %d ms;  Task creation chance : %d; Complexity range : %d - %d;\nProcessor powers : ", maxtime, taskprob, leftL, rightL);
	for (i = 0;i < K;i++)
	{
		printf("%d ", system[i].power);
		peak += system[i].power*maxtime;
	}
	printf("\nAbsolute system perfomance: %d\n", peak);

	printf("\nScheme with separate controller\n");
	expected = Sep_Controller(system, maxtime, taskprob);
	for (i = 0;i < K;i++)
	{
		system[i].dataout();
		total += system[i].f_op;
		tasks += system[i].f_tsk;
	}
	printf("Total operations: %d; Expected maximum: %d;Total tasks: %d Efficiency: %.6f; Efficiency\': %.6f\n", total, expected, tasks, (float)total / peak, (float)total / expected);

	tasks = 0;
	total = 0;
	for (i = 0;i < K;i++)
		system[i].reset();
	printf("\nScheme with periodical control (%d:%d)\n", 4, 20);
	expected = Temp_Controller(system, maxtime, taskprob, 4, 20);
	for (i = 0;i < K;i++)
	{
		system[i].dataout();
		total += system[i].f_op;
		tasks += system[i].f_tsk;
	}
	printf("Total operations: %d; Expected maximum: %d;Total tasks: %d Efficiency: %.6f; Efficiency\': %.6f\n", total, expected, tasks, (float)total / peak, (float)total / expected);

	tasks = 0;
	total = 0;
	for (i = 0;i < K;i++)
		system[i].reset();
	printf("\nScheme with periodical control (%d:%d)\n", 4, 40);
	expected = Temp_Controller(system, maxtime, taskprob, 4, 40);
	for (i = 0;i < K;i++)
	{
		system[i].dataout();
		total += system[i].f_op;
		tasks += system[i].f_tsk;
	}
	printf("Total operations: %d; Expected maximum: %d;Total tasks: %d Efficiency: %.6f; Efficiency\': %.6f\n", total, expected, tasks, (float)total / peak, (float)total / expected);

	tasks = 0;
	total = 0;
	for (i = 0;i < K;i++)
		system[i].reset();
	expected = peak;
	printf("\nQueue/FIFO scheme without controller\n");
	FIFO_NoController(system, maxtime, taskprob);
	for (i = 0;i < K;i++)
	{
		system[i].dataout();
		total += system[i].f_op;
		tasks += system[i].f_tsk;
	}
	printf("Total operations: %d; Expected maximum: %d;Total tasks: %d Efficiency: %.6f; Efficiency\': %.6f\n", total, expected, tasks, (float)total / peak, (float)total / expected);

	getchar();
	getchar();
	return 0;
}
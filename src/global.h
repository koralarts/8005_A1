#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

#define MIN_WORKERS	5
#define MIN_TIMES	1000
#define DEF_FILENAME	"delay.txt"
#define BUFFER_LEN	65000
#define USAGE		"USAGE: %s -w [number of Workers >= 5] -t [number of times >= 1000] -f [file name]\n"

typedef struct {
	FILE* pFile;
	FILE* delay;
	int times;
	int finished;
	struct timeval start;
} PARAMS, *PPARAMS;

void* calculate(void* params);
long delay (struct timeval t1, struct timeval t2);
unsigned int fib(unsigned int n);
void writeTime(struct timeval start, FILE* pFile, const char* type);
void createWorkers(int workers, PPARAMS params);

/*
-- SOURCE FILE: a1.c
--
-- PROGRAM: Process vs. Threads
--
-- FUNCTIONS:
-- void* calculate(void* params);
-- long delay (struct timeval t1, struct timeval t2);
-- unsigned int fib(unsigned int n);
-- void writeTime(struct timeval start, FILE* pFile, const char* type);
-- void createWorkers(int workers, PPARAMS params);
--
-- DATE: January 20, 2012
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Karl Castillo
--
-- PROGRAMMER: Karl Castillo
--
-- NOTES:
-- This file will be used by both the thread and process version of the program
-- by using compile-time defines.
--
-- To compile each of the versions define "process" or "thread" depending on the
-- version desired.
*/

#ifdef thread
#include "thread.h"
#endif

#ifdef process
#include "process.h"
#endif

#include "global.h"

/*
-- FUNCTION: main
--
-- DATE: January 20, 2012
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Karl Castillo
--
-- PROGRAMMER: Karl Castillo
--
-- INTERFACE: int main(int argc, char** argv)
--				argc - number of arguments
--				argv - the arguments
--
-- RETURNS: int
--		EXIT_SUCESS - success
--		EXIT_FAILURE - error
--
-- NOTES:
-- This is the main function where the arguments are parsed and proper 
-- preparations are done. This is also where the workers are spawned, and the
-- delay is written in a file.
*/
int main(int argc, char* argv[])
{
	char* fileName = DEF_FILENAME;
	int option = 0;
	int workers = MIN_WORKERS;
	PPARAMS params = malloc(sizeof(PARAMS));

	params->times = MIN_TIMES;

	#ifdef process
	int status;
	#endif
	
	if(argc > 7) {
		fprintf(stderr, "Too many arguments.\n");
		fprintf(stderr, USAGE, argv[0]);
		return EXIT_FAILURE;
	} /* if(argc > 3) */

	while((option = getopt(argc, argv, "w:t:f:")) != -1) {
		switch(option) {
		case 'w':
			if((workers = atoi(optarg)) == 0 || workers < MIN_WORKERS) {
				fprintf(stderr, "-w: Invalid input.\n");
				fprintf(stderr, USAGE, argv[0], MIN_WORKERS, MIN_TIMES);
				return EXIT_FAILURE;
			}
			break;
		case 't':
			if((params->times = atoi(optarg)) == 0 || params->times < MIN_TIMES) {
				fprintf(stderr, "-t: Invalid input.\n");
				fprintf(stderr, USAGE, argv[0], MIN_WORKERS, MIN_TIMES);
				return EXIT_FAILURE;
			}

			break;
		case 'f':
			fileName = optarg;
			break;
		default:
			fprintf(stderr, "Invalid input.\n");
			fprintf(stderr, USAGE, argv[0], MIN_WORKERS, MIN_TIMES);
			return EXIT_FAILURE;
		} /* switch(option) */
	} /* while((option = getopt(argc, argv, ":i:")) != -1) */

	gettimeofday(&params->start, NULL);
	
	params->finished = 0;
	params->pFile = fopen("results.txt", "w");
	params->delay = fopen(fileName, "w");

	printf("Num of Workers: %d\n", workers);
	printf("Num of Times: %d\n", params->times);
	printf("Delay File: %s\n", fileName);

	#ifdef process
	createWorkers(workers, params);
	writeTime(params->start, params->delay, "Setup Delay");
	while(1) {
		waitpid(-1, &status, 0);
		if(WIFEXITED(status)) {
			params->finished++;
		} /* if(WIFEXITED(status)) */
		if(params->finished == workers) {
			break;
		} /* if((++finished) == 5) */
	} /* while(1) */
	#endif

	#ifdef thread
	createWorkers(workers, params);
	writeTime(params->start, params->delay, "Setup Delay");
	while(params->finished != workers) {}
	#endif

	writeTime(params->start, params->delay, "Full Delay");
	fclose(params->delay);
	fclose(params->pFile);

	return EXIT_SUCCESS;
}

/*
 * FUNCTION: calculate
 *
 * DATE: Januray 20, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * Designer: Karl Castillo
 *
 * Programmer: Karl Castillo
 *
 * INTERFACE: void* calculate(void* params)
 *				params - pointer to the parameters needed for
 *					the I/O and calculations to work.
 *
 * RETURNS: NULL
 *
 * NOTES:
 * This is used by both the thread and process versions of the program for
 * consistency in the function calls.
 */
void* calculate(void* params)
{
	struct timeval start;
	int i;
	PPARAMS p = (PPARAMS)params;
	char* buffer = malloc(sizeof(char) * BUFFER_LEN);
	unsigned int result = 0;

	gettimeofday(&start, NULL);

	for(i = 0; i <= p->times; i++) {
		result = fib(20);
		sprintf(buffer, "Result: %u\n", result);
		fputs(buffer, p->pFile);
	} /* for(i = 0; i <= p->times; i++) */

	writeTime(start, p->delay, "Delay");

	#ifdef thread
	p->finished++;
	#endif

	return NULL;
}

/*
 * FUNCTION: delay
 *
 * DATE: Januray 20, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * Designer: Aman Abdulla
 *
 * Programmer: Karl Castillo
 *
 * INTERFACE: long delay (struct timeval t1, struct timeval t2)
 *				t1 - timeval struct that holds the starting time
 *				t2 - timeval struct that holds the ending time
 *
 * RETURNS: long - the total delay
 *
 * NOTES:
 * This is used by both the thread and process versions of the program to
 * calculate the delay.
 */
long delay (struct timeval t1, struct timeval t2)
{
	long d;

	d = (t2.tv_sec - t1.tv_sec) * 1000;
	d += ((t2.tv_usec - t1.tv_usec + 500) / 1000);
	return(d);
}

/*
 * FUNCTION: fib
 *
 * DATE: Januray 20, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * Designer: Karl Castillo
 *
 * Programmer: Karl Castillo
 *
 * INTERFACE: unsigned int fib(unsigned int n)
 *				n - the starting number to do fibonacci on
 *
 * RETURNS: unsigned int - the result of the calculation
 *
 * NOTES:
 * The classic recursive fibonacci sequence function that is used by both
 * versions of the program.
 */
unsigned int fib(unsigned int n)
{
	if(n < 2) {
		return n;
	}
	return fib(n - 1) + fib(n - 2);
}

/*
 * FUNCTION: writeTime
 *
 * DATE: Januray 20, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * Designer: Karl Castillo
 *
 * Programmer: Karl Castillo
 *
 * INTERFACE: void writeTime(struct timeval start, FILE* pFile, const char* type)
 *				start - timeval struct that holds the starting time
 *				pFile - pointer to the file that the time will
 *					be written in
 *				type - the format in which the time will be written
 *
 * RETURNS: NULL
 *
 * NOTES:
 * This is used by both the thread and process versions of the program where
 * the delay times will be written.
 */
void writeTime(struct timeval start, FILE* pFile, const char* type)
{
	struct timeval end;
	char* buffer = malloc(sizeof(char) * BUFFER_LEN);

	memset(buffer, '\0', BUFFER_LEN);
	gettimeofday(&end, NULL);
	sprintf(buffer, "%s: %lums\n", type, delay(start, end));
	fputs(buffer, pFile);
}

/*
 * FUNCTION: createWorkers
 *
 * DATE: Januray 20, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * Designer: Karl Castillo
 *
 * Programmer: Karl Castillo
 *
 * INTERFACE: void createWorkers(int workers, PPARAMS params)
 *				workers - the number of workers to be made
 *				params - the struct holding the necessary
 *					variables for the job
 *
 * RETURNS: NULL
 *
 * NOTES:
 * This is used by both the thread and process versions of the program for
 * consistency in the function calls.
 */
void createWorkers(int workers, PPARAMS params)
{
	int i;

	#ifdef process
	int pid;
	#endif

	#ifdef thread
	pthread_t* threads;
	threads = malloc(sizeof(pthread_t) * workers);
	#endif

	for(i = 0; i < workers; i++) {
		#ifdef process
		if((pid = fork()) < 0) { /* error */
			perror("Error: fork()\n");
			exit(EXIT_FAILURE);
		} /* if((pid = fork()) < 0) */
		if(pid == 0) { /* child process */ 
			calculate(params);
			exit(EXIT_SUCCESS);
		} /* if(pid == 0) */
		#endif

		#ifdef thread
		if(pthread_create(&threads[i], NULL, calculate, (void*)params) != 0) {
			perror("Error: pthread_create()\n");
			exit(EXIT_FAILURE);
		} /* end if */
		#endif
	} /* for(int i = 0; i < workers; i++) */
}

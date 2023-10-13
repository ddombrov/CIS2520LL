#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "fasta.h"
#include "fasta_read.c"
#include "LLvNode.h"
#include "LLvNode.c"

int processFasta(char *filename, double *timeTaken) {

	FILE *fp;
	FASTArecord fRecord;
	int lineNumber = 0, recordNumber = 0, status;
	int eofSeen = 0;
	clock_t startTime, endTime;

	LLvNode *l_head = NULL;

	//open file
	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Failure opening %s : %s\n", filename, strerror(errno));
		return -1;
	}

	/** record the time now, before we do the work */
	startTime = clock();

	do {
		/** print a '.' every 10,000 records so
		* we know something is happening */
		if ((recordNumber % 10000) == 0) {
			printf(".");
			fflush(stdout);
		}

		//Allocate and initialize a new FASTA record
		fastaInitializeRecord(&fRecord);

		//status = lines read
		//ensure the buffer allocated here is sufficient for that length
		status = fastaReadRecord(fp, &fRecord);

		//if no lines read
		if (status == 0) {
			eofSeen = 1;

		//if lines read
		} else if (status > 0) {
			lineNumber += status;
			recordNumber++;
			//fastaPrintRecord(stdout, &fRecord);
			//fastaClearRecord(&fRecord);

			//make a node
			void *value = malloc(sizeof(FASTArecord));
			memcpy(value, &fRecord, sizeof(FASTArecord));

			LLvNode *l_node = llNewNode(fRecord.description, value);

			//add node to end
			l_head = llAppend(l_head, l_node);

			//printf("%s - ", l_node->key);
			

		//if line read error
		} else {
			fprintf(stderr, "status = %d\n", status);
			fprintf(stderr, "Error: failure at line %d of '%s'\n", lineNumber, filename);
			return -1;
		}

	} while ( ! eofSeen);

	printf(" %d FASTA records\n", recordNumber);

	/** record the time now, when the work is done,
	 *  and calculate the difference*/
	endTime = clock();

	(*timeTaken) = ((double) (endTime - startTime)) / CLOCKS_PER_SEC;

	fclose(fp);

/*
	for (LLvNode *next = l_head; next!=NULL; next=l_head ) {
		l_head=l_head->next;
		fastaDeallocateRecord(next->value);
		free(next);
	}
	*/
	LLvNode *next;

	for ( ; l_head != NULL; l_head = next) {

		/** hang on to the next pointer */
		next = l_head->next;

		/** free the list node itself */
		fastaDeallocateRecord(l_head->value);
		free(l_head);
	}

	return recordNumber;

}

/**
 * 
 */
int processFastaRepeatedly(char *filename, long repeatsRequested) {

	double timeThisIterationInSeconds;
	double totalTimeInSeconds = 0;
	int minutesPortion;
	int status;
	long i;

	//repeat processing for number of repeats
	for (i = 0; i < repeatsRequested; i++) {
		status = processFasta(filename, &timeThisIterationInSeconds);
		if (status < 0)	return -1;
		totalTimeInSeconds += timeThisIterationInSeconds;
	}

	//time display information
	printf("%lf seconds taken for processing total\n", totalTimeInSeconds);

	totalTimeInSeconds /= (double) repeatsRequested;

	minutesPortion = (int) (totalTimeInSeconds / 60);
	totalTimeInSeconds = totalTimeInSeconds - (60 * minutesPortion);
	printf("On average: %d minutes, %.3f second per run\n",  minutesPortion, totalTimeInSeconds);

	return status;

}

/**
 * Usage explanation
 */
void usage(char *progname) {

	fprintf(stderr, "%s [<OPTIONS>] <file> [ <file> ...]\n", progname);
	fprintf(stderr, "\n");
	fprintf(stderr, "Prints timing of loading and storing FASTA records.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Options: \n");
	fprintf(stderr, "-R <REPEATS> : Number of times to repeat load.\n");
	fprintf(stderr, "             : Time reported will be average time.\n");
	fprintf(stderr, "\n");

}

/**
 * Program mainline
 */
int main(int argc, char **argv) {
	
	int i, recordsProcessed = 0;
	long repeatsRequested = 1;

	//go through arguments
	for (i = 1; i < argc; i++) {

		//if they use -
		if (argv[i][0] == '-') {

			//if they use R
			if (argv[i][1] == 'R') {

				//if R is too late
				if (i >= argc) {
					fprintf(stderr, "Error: need argument for repeats requested\n");
					return 1;
				}

				//if scanning error
				if (sscanf(argv[++i], "%ld", &repeatsRequested) != 1) {
					fprintf(stderr, "Error: cannot parse repeats requested from '%s'\n", argv[i]);
					return 1;
				}
			
			//if R not used
			} else {
				fprintf(stderr, "Error: unknown option '%s'\n", argv[i]);
				usage(argv[0]);
			}
		
		//if - not used
		} else {
			recordsProcessed = processFastaRepeatedly(argv[i], repeatsRequested);

			//if file not processed
			if (recordsProcessed < 0) {
				fprintf(stderr, "Error: Processing '%s' failed -- exitting\n", argv[i]);
				return 1;
			}

			//if file processed
			printf("%d records processed from '%s'\n", recordsProcessed, argv[i]);
		}
	}

	//if nothing processed
	if ( recordsProcessed == 0 ) {
		fprintf(stderr, "No data processed -- provide the name of a file on the command line\n");
		usage(argv[0]);
		return 1;
	}

	return 0;

}


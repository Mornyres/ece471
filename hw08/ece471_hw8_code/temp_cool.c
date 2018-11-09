#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "sensor_name.h"

static double read_temp(char *filename) {

	double result=0.0;

	/* Your code here */
	
	int i,j=0;
	int size;
	char buf_1[255], buf_2[255], temp[255], milliC[255];

	FILE *inputFile;

	// open for read only
	inputFile = fopen(filename, "r");

	// only read first two lines
	fscanf(inputFile,"%[^\n]\n %[^\n]\n", buf_1, buf_2);

	size = strlen(buf_1);
	i = size;
	while(buf_1[i] != ' ') i--; // rewind file pointer to first available blank space
	i++;

	// go to end of line
	while((buf_1[i]) != '\0'){
		temp[j]=buf_1[i];
		i++;
		j++;
	}
	temp[j] = '\0';

	// YES in line 1 denotes data has passed CRC and is stored at EOL
	if (strcmp(temp,"YES") != 0){
		printf("Error -- couldn't validate raw temp.\n");
		return -1;
	}

	i=0;
	j=0;

	while(buf_2[i] != '=') i++;
	i++;

	// iterate to the end of the next string (raw temp reading) and copy to working variable
	while(buf_2[i] != '\0'){
		milliC[j] = buf_2[i];
		i++;
		j++;
	}

	//ensure null termination
	milliC[j] = '\0';
	
	result = (atoi(milliC))/1000.0;	// go from milliC to C

	return result;
}

int main(int argc, char **argv) {

	double tempC, tempF, tempK;

	while(1) {

		tempC=read_temp(SENSOR_NAME);
		tempK= tempC + 273.15; // kelvin
		tempF = (tempC + (9.0/5.0)) + 32.0; //fahrenheit

		printf("%.2lfC\n",tempC);
		printf("%.2lfK\n",tempK);
		printf("%.2lfF\n",tempF);
		sleep(1);
	}

	return 0;
}

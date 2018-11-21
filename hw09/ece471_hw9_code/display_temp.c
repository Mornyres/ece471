#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <float.h>

#include "sensor_name.h"
#include "lcd.h"

// setting errorcode to max neg. double value as defined in float.h
#define ERRORCODE -DBL_MAX

// preprocessor defined constants __FILE__, __LINE__ used here
void ErrorHandler(char* file, int line);

static double read_temp(char* filename);

int main(int argc, char **argv) {
	
	double tempC=0.0, tempF=0.0, tempK=0.0;
	int manualvalue=0;
	
	// T is for test, like test 999, etc works
	// convert string arg to float
	if ((argc==3)
		&&
	   (strcmp(argv[1],"-T") == 0)){
		
		tempC=atof(argv[2]);
		printf("Accepting hardcoded value %.2f\r\n",tempC); 
		//set manualvalue flag so sensor not polled
		manualvalue = 1;
	}
	

	// main loop begin
	while(1) {

		// only poll sensor if manual value not set
		if (manualvalue==0){
			
			// make sure sensor filedesc exists
			if ( access( SENSOR_NAME, F_OK ) != -1){
				// successful access
				tempC=read_temp(SENSOR_NAME);
			}
			else {
				//break out of the whole loop because the device identifier is wrong or inaccessible
				printf("Error -- couldn't access specified file desc.\r\n");
				ErrorHandler(__FILE__, __LINE__);
				break;
			}
		}
			
		if (tempC == ERRORCODE){
			// something has gone horribly wrong, which will also be printed by above func.
			// we can continue with the main loop though, because it may be a momentary problem
			printf("Error -- couldn't obtain temp. reading for this iteration\r\n");
			ErrorHandler(__FILE__, __LINE__);
		}
		
		// valid temp reading
		else{
			tempK= tempC + 273.15; // kelvin
			tempF = (tempC * (9.0/5.0) ) + 32.0; //fahrenheit

			printf("-------------\r\n");
			printf("%.4lfC\r\n",tempC); // printing to console just for debug
			printf("%.4lfK\r\n",tempK);
			printf("%.4lfF\r\n",tempF);

			tempDisplay(tempC);

		}
		sleep(1);
	}

	return 0;
}

static double read_temp(char *filename) {

	double result=0.0;
	
	int i,j=0;
	int size;
	char buf_1[255], buf_2[255], temp[255], milliC[255];

	FILE *inputFile;

	// open for read only
	inputFile = fopen(filename, "r");
	
	if (inputFile == NULL){
		printf("Error -- couldn't open file for reading.\r\n");
		ErrorHandler(__FILE__, __LINE__);
		return ERRORCODE;
	}

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
		printf("Error -- couldn't validate raw temp.\r\n");
		ErrorHandler(__FILE__, __LINE__);
		return ERRORCODE;
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

void ErrorHandler(char* file, int line){
	printf("Error context: %s:%d\r\n",file,line); 
}

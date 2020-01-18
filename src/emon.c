/*
 * emon.c
 *
 *  Created on: 6 févr. 2015
 *      Author: Bertrand
 */


#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <stdarg.h>

//
//  http://emoncms.org/input/post.json?json={power:200}&apikey=33d19b54e3e8cc180aa03bf0eeee8518
//



void emon_log_data(char* name, float value)
{
	//if(fork()==0)
	{
		char cmdline[1024];
		sprintf(cmdline,"curl  \"http://emonpi/input/post.json?json={%s:%f}&apikey=9c58f6f0491bf90f7c5580de8a7e7999\" -o emon -s",name,value);
		system(cmdline);
		//printf("%s\n",cmdline);
	}

}


int logData(char* dbtype,char* dbname,time_t date,float data)
{
	char filename[256];

	sprintf(filename,"%s_%s",dbname,dbtype);

	emon_log_data(filename, data);
}

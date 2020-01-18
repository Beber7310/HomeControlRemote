/*
 * utils.c
 *
 *  Created on: 25 déc. 2013
 *      Author: Bertrand
 */

#define MAIN_CODE

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "utils.h"
#include "SerialManagement.h"
#include "Math.h"

#define NBR_MSG_MAX	10000
message_t msg_log[NBR_MSG_MAX];

int last_msg = 0;

void sendSMS(char* text)
{
	char szTmp[1024];
	sprintf(szTmp, "wget \"https://smsapi.free-mobile.fr/sendmsg?user=12497092&pass=HO6ri0tIuaVLYY&msg=%s\"  -o tmp_file ;", text);
	system(szTmp);
	system("rm -f sendmsg*");
}

void log_msg(const char* level, char* tag, char *format, va_list args)
{

	strncpy(msg_log[last_msg].level, level, sizeof(msg_log[0].level));
	strncpy(msg_log[last_msg].tag, tag, sizeof(msg_log[0].tag));
	vsprintf(msg_log[last_msg].message, format, args);
	msg_log[last_msg].date = time(NULL);

	last_msg++;
	if (last_msg >= NBR_MSG_MAX)
		last_msg = 0;

}

int get_http_msg(char* bufhttp, int buflen)
{
	int len = 0;
	int ii;
	char buf[512];
	ii = (last_msg - 1);

	while (ii != last_msg)
	{
		if (ii < 0)
			ii = NBR_MSG_MAX - 1;

		if (msg_log[ii].date > 0)
		{
			sprintf(buf, "[%s] [%s] %s\n", msg_log[ii].level, msg_log[ii].tag, msg_log[ii].message);
			strcpy(&bufhttp[len], buf);
			len += strlen(buf);
			printf("%i\n", len);
		}
		if (len > buflen - 512)
		{
			return len;
		}
		ii--;
	}
	return len;
}

int get_http_sts_short(char* bufhttp, int buflen)
{
	int len = 0;
	int ii, jj, kk;
	char buf[512];
	int index_prog;

	index_prog = get_pgm_index();

	sprintf(buf, "<html><head><title>HC</title><meta http-equiv=\"refresh\" content=\"1\" /></head><body>");
	strcpy(&bufhttp[len], buf);
	len += strlen(buf);

	sprintf(buf, "<pre>\n");
	strcpy(&bufhttp[len], buf);
	len += strlen(buf);

	/////////////////////////////////////////////////////////////////
	// Power

	sprintf(buf, "[POWER]\n");
	strcpy(&bufhttp[len], buf);
	len += strlen(buf);

	sprintf(buf, "POWER %f\n", power.power);
	strcpy(&bufhttp[len], buf);
	len += strlen(buf);
	if (len > buflen - 512)
	{
		return len;
	}

	return len;
}

int get_http_sts(char* bufhttp, int buflen)
{
	int len = 0;
	int ii, jj, kk;
	char buf[512];
	int index_prog;

	index_prog = get_pgm_index();

	return len;
}

float parse_http_temp(char* cmd)
{
	char* pch;
	float temp = 0;
	pch = strtok(cmd, "?");
	pch = strtok(NULL, "?");

	return temp;
}

float parse_http_amp(char* cmd)
{

	return power.current;
}

float parse_http_thermostat_target(char* cmd)
{
	char* pch;
	float res;

	pch = strtok(cmd, "?");
	info("HTTP", pch);
	pch = strtok(NULL, "/");
	info("HTTP", pch);
	pch = strtok(NULL, "/");
	info("HTTP", pch);
	pch = strtok(NULL, "/");
	info("HTTP", pch);

	if (!pch)
		return 0.0f;

	if (strncmp("off", pch, strlen("off")) == 0)
	{
		return 15.0f;
	}

	if (strncmp("comfort", pch, strlen("comfort")) == 0)
	{
		return 20.0f;
	}

	if (strncmp("no-frost", pch, strlen("no-frost")) == 0)
	{
		return 15.0f;
	}

	res = atof(pch);

	if (isnan(res))
		res = 15.0f;

	if (res < 15.0f)
		res = 15.0f;
	return res;

}

int parse_http_thermostat_status(char* cmd)
{
	char* pch;
	float temp = 0;
	pch = strtok(cmd, "?");
	pch = strtok(NULL, "/");
	pch = strtok(NULL, "/");

	if (!pch)
		return 0;
	if (strncmp("status", pch, strlen("status")) == 0)
	{
		//info("HTTP","Command thermostat get status");
		return 1;
	}

	//info("HTTP","Command thermostat set target");
	return 0;

}

//  hc_cmd?RAD_Homecinema=20

int parse_http_cmd(char* cmd)
{
	char* pch;
	pch = strtok(cmd, "?");
	pch = strtok(NULL, "?");
	while (pch != NULL)
	{
		parse_http_cmd_token(pch);
		pch = strtok(NULL, "?");
	}
	return 0;
}

int parse_http_cmd_token(char* cmd)
{
	int ii;

	//RAD_Garage=on
	if (strncmp("FILP_", cmd, strlen("FILP_")) == 0)
	{
		parse_http_cmd_FILP_token(&cmd[sizeof("FILP_") - 1]);
	}

}

int get_http_cmd(char* bufhttp, int buflen)
{
	int len = 0;
	int ii;
	char buf[512];
	struct tm * timeinfo;

	sprintf(buf, "<html><head><title>Sample \"Hello, World\" Application</title></head>\n");
	strcpy(&bufhttp[len], buf);
	len += strlen(buf);

	sprintf(buf, "<body bgcolor=white>\n");
	strcpy(&bufhttp[len], buf);
	len += strlen(buf);

	sprintf(buf, "<p><p><p>[FIL PILOTE]\n");
	strcpy(&bufhttp[len], buf);
	len += strlen(buf);


	sprintf(buf, "<p>\n <a href=\"hc_cmd?FILP_DATA=20\">hc_cmd?FILP_DATA=20</a> ");
	strcpy(&bufhttp[len], buf);
	len += strlen(buf);

	sprintf(buf, "</body></html>\n");
	strcpy(&bufhttp[len], buf);
	len += strlen(buf);

	return len;
}

int parse_http_cmd_FILP_token(char* cmd)
{
	int ii;
	int data_filp = -1;

	char * pch;
	pch = strtok(cmd, "=");
	if (pch)
		pch = strtok(NULL, "=");
	if (pch)
		data_filp = atoi(pch);

	info("HTTP", "Command receive FILP : %i", data_filp);
	if(data_filp>=0)
		SerialFilPiloteSendCommande(data_filp);

	/*
	 for(ii=0;ii<RD_LAST;ii++)
	 {
	 if(strncmp(radiateur[ii].name,cmd,strlen(radiateur[ii].name))==0)
	 {
	 info("HTTP","Command receive: : %s",radiateur[ii].name);

	 radiateur[ii].http_req_time=time(NULL);
	 radiateur[ii].http_req_temp=target_temp;
	 sem_post(&sem_capteur_data_available);
	 }
	 }
	 */
}

void info(char* tag, char *format, ...)
{
	va_list args;
	va_start(args, format);

	printf("\e[32m[INFO] \e[0m");
	printf("[%s] ", tag);
	vprintf(format, args);
	printf("\n");
	va_end(args);

	va_start(args, format);
	log_msg("INFO", tag, format, args);
	va_end(args);

}

void warning(char* tag, char *format, ...)
{
	va_list args;
	va_start(args, format);

	printf("\e[33m[WARNING] \e[0m");
	printf("[%s] ", tag);
	vprintf(format, args);
	printf("\n");
	va_end(args);

	va_start(args, format);
	log_msg("WARNING", tag, format, args);
	va_end(args);
}

void init_msg(void)
{
	int ii;

	for (ii = 0; ii < NBR_MSG_MAX; ii++)
	{
		msg_log[ii].date = 0;
		msg_log[ii].level[0] = 0;
		msg_log[ii].message[0] = 0;
		msg_log[ii].tag[0] = 0;
	}

}

int get_pgm_index(void)
{
	time_t rawtime;
	struct tm * timeinfo;
	int index_prog;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	index_prog = ((timeinfo->tm_wday * 24 * 60) + (timeinfo->tm_hour * 60) + timeinfo->tm_min) / 15;

}

char hextochar(char hex)
{

	if (hex >= 0 && hex <= 9)
	{
		return hex + '0';
	}
	else if (hex >= 0xa && hex <= 0xf)
	{
		return hex + 'a' - 10;
	}
	else
	{
		return -1;   // getting here is bad: it means the character was invalid
	}
}

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>


#include "SerialManagement.h"
#include "http.h"
#include "utils.h"

char dash_duracel[] = "6c:56:97:1f:82:77";

int mainB()
{
	while (1)
	{
		sleep(5);
		system("./Homecontrol.a");
	}
}

int main()
{
	pthread_t th_radiateur;
	pthread_t th_uart;
	pthread_t th_uart_fp;
	pthread_t th_http;
	pthread_t th_dash_1;

	init_msg();

	info("START UP", "\n\n    ---- Home control Remote---- \n");
	setpriority(PRIO_PROCESS, 0, -10);


	if (pthread_create(&th_uart_fp, NULL, uart_filPilote_loop, 0) < 0)
	{
		info("START UP", "pthread_create error for thread uart_filPilote_loop");
		exit(1);
	}

	if (pthread_create(&th_http, NULL, http_loop, 0) < 0)
	{
		info("START UP", "pthread_create error for thread http_loop");
		exit(1);
	}
	info("START UP", "Thread http created");



	while (1)
	{
		sleep(1);
	}
	return 0;
}

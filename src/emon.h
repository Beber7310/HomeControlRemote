/*
 * emon.h
 *
 *  Created on: 6 févr. 2015
 *      Author: Bertrand
 */

#ifndef EMON_H_
#define EMON_H_

void emon_log_data(char* name, float value);
int logData(char* dbtype,char* dbname,time_t date,float data);

#endif /* EMON_H_ */

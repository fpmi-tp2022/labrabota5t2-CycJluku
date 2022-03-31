#ifndef server_h
#define server_h

#define _CRT_SECURE_NO_WARNINGS
#define TRUE 1
#define FALSE 0

#include "stdio.h"
#include "stdlib.h"
#include "sqlite3.h" 
#include "string.h"

static short access = FALSE;
static short isCommander = FALSE;
static char current_buff[100];

void Registraion(sqlite3* db);
void Login(sqlite3* db);
short executeSQL(sqlite3* db, const char* sql, int(*callback)(void*, int, char**, char**), void* data, short success_inform);

#endif

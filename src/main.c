#include "sqlite3.h" 
#include "../include/server.h"
#include "stdio.h"
#include "stdlib.h"

int main()
{
    sqlite3* db;
    char* zErrMsg = 0;
    int result;

    result = sqlite3_open("doc/air_cub.db", &db);

    if (result)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    char buff[20];
    int des;
    start:
    while (!access)
    {
        printf("\nChoose the action:\n"
            "1. REGISTRATION;\n"
            "2. LOGIN;\n"
            "3. ADMIN ACTIONS;\n"
            "4. EXIT;\n");

        fgets(buff, 20, stdin);
        des = atoi(buff);
        switch (des)
        {
        case 1:
            Registraion(db);
            break;
        case 2:
            Login(db);
            break;
        case 3:

            break;
        case 4:
            sqlite3_close(db);
            return 0;
        default:
            fprintf(stderr, "\nWrong parameter");
        }
    }
    
     while (TRUE)
    {
        printf("Choose the action:\n"
                "1. GET INFO;\n"
                "2. EXIT\n");

        fgets(buff, 20, stdin);
        des = atoi(buff);

        switch (des)
        {
        case 1:
            getInfo(db);
            break;
        case 2:
            strcpy(current_ID, "0");
            access = FALSE;
            isCommander = FALSE;
            goto start;
            break;
        default:
            fprintf(stderr, "\nWrong parameter");
        }
    }
}

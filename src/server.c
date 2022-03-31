#include "../include/server.h"

exec = FALSE;

int printTable(void* data, int argc, char** argv, char** azColName)
{
    for (int i = 0; i < argc; i++) {

        printf("%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");

    return 0;
}

short executeSQL(sqlite3* db, const char* sql, int(*callback)(void*, int, char**, char**), void* data, short success_inform)
{
    char* ErrMsg;
    int rc = sqlite3_exec(db, sql, callback, data, &ErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\nSQL error: %s\n", ErrMsg);
        sqlite3_free(ErrMsg);
        return FALSE;
    }
    else if(success_inform){
        fprintf(stdout, "\nOperation done successfully\n");
        return TRUE;
    }
    return TRUE;
}


void AskParameter(char* msg, char* sql, short isInt, short isEnd)
{
    char buff[100];
    if (sql)
    {
        printf("%s", msg);
    }
    fgets(buff, 100, stdin);
    int size = strlen(buff);
    if (size > 1)
        buff[size - 1] = '\0';
    if (!strcmp(buff, "\n"))
    {
        if (!isEnd)
            strcat(sql, "null, ");
        else
            strcat(sql, "null);");
        return;
    }

    if (!isInt)
    {
        strcat(sql, "'");
        strcat(sql, buff);
        if (!isEnd)
            strcat(sql, "', ");
        else
            strcat(sql, "');");
    }
    else
    {
        strcat(sql, buff);
        if (!isEnd)
            strcat(sql, ", ");
        else
            strcat(sql, ");");
    }
}
int findLogin(void* data, int argc, char** argv, char** azColName)
{
    for (int i = 0; i < argc; i++) {
        access = TRUE;
        break;
    }
    return 0;
}
void AskParameterByID(sqlite3 *db, char* sql_print, char* msg, char* sql_aim, short isEnd)
{
    printf(msg);
    executeSQL(db, sql_print, printTable, NULL, FALSE);
    printf("\ninput>");
    AskParameter(NULL, sql_aim, TRUE, isEnd);
}

void Registraion(sqlite3* db)
{
    char sql[1000] = "INSERT INTO Pilots values(null, ";

    AskParameter("\nEnter Surname: ", sql, FALSE, FALSE);

    AskParameterByID(db, "Select * from Positions;", "\nChoose position by ID:\n", sql, FALSE);

    AskParameter("\nEnter experience: ", sql, TRUE, FALSE);

    AskParameter("\nEnter your address: ", sql, TRUE, FALSE);

    AskParameter("\nEnter your birth year: ", sql, TRUE, FALSE);

    AskParameterByID(db, "Select ID, Brand from Helicopters;", "\nChoose helicopter by ID:\n", sql, FALSE);

    AskParameter("\nEnter your login: ", sql, FALSE, FALSE);

    AskParameter("\nEnter your password: ", sql, FALSE, TRUE);

    if (executeSQL(db, sql, NULL, NULL, TRUE))
        access = TRUE;
}
void Login(sqlite3* db)
{
    char login[50], password[50];
    printf("\nEnter Login: ");
    fgets(login, 50, stdin);
    printf("\nEnter Password: ");
    fgets(password, 50, stdin);

    int size_l = strlen(login), size_p = strlen(password);
    if (size_l > 1)
    {
        login[size_l - 1] = '\0';
    }
    if (size_p > 1)
    {
        password[size_p - 1] = '\0';
    }
    char sql[500] = "Select ID from Pilots where login=";

    strcat(sql, login);
    strcat(sql, " and password=");
    strcat(sql, password);
    strcat(sql, ";");

    executeSQL(db, sql, findLogin, NULL, TRUE);
}

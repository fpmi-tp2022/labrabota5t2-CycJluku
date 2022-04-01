#include "../include/server.h"


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
    strcpy(current_buff, buff);
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
        strcpy(current_ID, argv[i]);
        break;
    }
    return 0;
}

int CheckIsCommander(void* data, int argc, char** argv, char** azColName)
{
    for (int i = 0; i < argc; i++) {
        if (!strcmp("Commander", argv[i]))
        {
            isCommander = TRUE;
            break;
        }
    }
    return 0;
}

int getCurrentID(void* data, int argc, char** argv, char** azColName)
{
    for (int i = 0; i < argc; i++) {
        if (!strcmp("Commander", argv[i]))
        {
            strcpy(current_ID, argv[i]);
            break;
        }
    }
    return 0;
}

void AskParameterByID(sqlite3 *db, char* sql_print, char* msg, char* sql_aim, short isEnd)
{
    printf("%s", msg);
    executeSQL(db, sql_print, printTable, NULL, FALSE);
    printf("\ninput>");
    AskParameter(NULL, sql_aim, TRUE, isEnd);
}

void Registraion(sqlite3* db)
{
    char sql[1000] = "INSERT INTO Pilots values(null, ";

    AskParameter("\nEnter Surname: ", sql, FALSE, FALSE);

    AskParameterByID(db, "Select * from Positions;", "\nChoose position by ID:\n", sql, FALSE);
    char position_choice[100];
    strcpy(position_choice, current_buff);
    AskParameter("\nEnter experience: ", sql, TRUE, FALSE);

    AskParameter("\nEnter your address: ", sql, TRUE, FALSE);

    AskParameter("\nEnter your birth year: ", sql, TRUE, FALSE);

    AskParameterByID(db, "Select ID, Brand from Helicopters;", "\nChoose helicopter by ID:\n", sql, FALSE);

    AskParameter("\nEnter your login: ", sql, FALSE, FALSE);

    AskParameter("\nEnter your password: ", sql, FALSE, TRUE);

    if (executeSQL(db, sql, NULL, NULL, TRUE))
    {
        access = TRUE;
        char sql_position[500] = "Select name from Positions where ID=";
        strcat(sql_position, position_choice);
        strcat(sql_position, ";");
        executeSQL(db, sql_position, CheckIsCommander, NULL, FALSE);

        char sql_id[] = "Select Max(id) from Pilots;";
        executeSQL(db, sql_id, getCurrentID, NULL, FALSE);
    }

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
    if (executeSQL(db, sql, findLogin, NULL, TRUE))
    {
        char sql_position[500] = "Select Positions.name from Positions inner join Pilots on Positions.ID=Pilots.position_id where Pilots.login=";
        strcat(sql_position, login);
        strcat(sql_position, " and Pilots.password=");
        strcat(sql_position, password);
        strcat(sql_position, ";");
        executeSQL(db, sql_position, CheckIsCommander, NULL, FALSE);
    }
}

void getInfo(sqlite3* db)
{
    char buff[20];
    char sql[500];
    int des;
    if (isCommander)
    {
        printf("Choose info type:\n"
                "1. MY DATA\n"
                "2. HELICOPTERS;\n"
                "3. CREWS;\n"
                "4. FLIGHTS\n");

        fgets(buff, 20, stdin);
        des = atoi(buff);

        switch (des)
        {
        case 1:
            strcpy(sql, "Select Pilots.id, Pilots.surname, Positions.name, Pilots.experience, Pilots.address, Pilots.birth_year, "
                "Pilots.helicopter_id from Pilots inner join Positions on Pilots.position_id=Positions.ID where Pilots.ID=");
            strcat(sql, current_ID);
            strcat(sql, ";");
            break;
        case 2:
            strcpy(sql, "Select * from Helicopters;");
            break;
        case 3:
            strcpy(sql, "Select Pilots.id, Pilots.surname, Positions.name, Pilots.experience, Pilots.address, Pilots.birth_year, "
                "Pilots.helicopter_id from Pilots inner join Positions on Pilots.position_id=Positions.id order by Pilots.helicopter_id;");
            break;
        case 4:
            strcpy(sql, "Select Flights.id, Flights_helicopter_id, Flights.date, Flights.mass_cargo, Flights.duration, Flights.price, "
                "Types.name from Flights inner join Types where Flights.type_id=Types.id;");
            break;
        default:
            printf("\nWrong parameter");
            return;
        }

        executeSQL(db, sql, printTable, NULL, FALSE);
    }
    else
    {
        printf("Choose info type:\n"
            "1. MY DATA;\n"
            "2. MY HELICOPTER;\n");

        fgets(buff, 20, stdin);
        des = atoi(buff);

        switch (des)
        {
        case 1:
            strcpy(sql, "Select Pilots.id, Pilots.surname, Positions.name, Pilots.experience, Pilots.address, Pilots.birth_year, "
                "Pilots.helicopter_id from Pilots inner join Positions on Pilots.position_id=Positions.ID where Pilots.ID=");
            strcat(sql, current_ID);
            strcat(sql, ";");
            break;
        case 2:
            strcpy(sql, "Select Helicopters.* from Helicopters inner join Pilots on Helicopters.id=Pilots.helicopter_id where Pilots.id=");
            strcat(sql, current_ID);
            strcat(sql, ";");
            break;
        default:
            printf("\nWrong parameter");
            return;
        }
    }

}

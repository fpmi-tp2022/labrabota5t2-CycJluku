#include "../include/server.h"
#include "../include/variables.h"

#define MAIN_FILE
short access;
short isCommander;
char current_buff[100];
char current_ID[10];
char entered_helicopter_id[10];
int duration_check_passed;

int printTable(void *data, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {

        printf("%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");

    return 0;
}

short executeSQL(sqlite3 *db, const char *sql, int(*callback)(void *, int, char **, char **), void *data,
                 short success_inform) {
    char *ErrMsg;
    int rc = sqlite3_exec(db, sql, callback, data, &ErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "\nSQL error: %s\n", ErrMsg);
        sqlite3_free(ErrMsg);
        return FALSE;
    } else if (success_inform) {
        fprintf(stdout, "\nOperation done successfully\n");
    }
    return TRUE;
}


void AskParameter(char *msg, char *sql, short isInt, short isEnd) {
    char buff[100];
    if (msg) {
        printf("%s", msg);
    }
    fgets(buff, 100, stdin);
    int size = strlen(buff);
    if (size > 1)
        buff[size - 1] = '\0';
    strcpy(current_buff, buff);
    if (!strcmp(buff, "\n")) {
        if (!isEnd)
            strcat(sql, "null, ");
        else
            strcat(sql, "null);");
        return;
    }

    if (!isInt) {
        strcat(sql, "'");
        strcat(sql, buff);
        if (!isEnd)
            strcat(sql, "', ");
        else
            strcat(sql, "');");
    } else {
        strcat(sql, buff);
        if (!isEnd)
            strcat(sql, ", ");
        else
            strcat(sql, ");");
    }
    strcpy(entered_helicopter_id, buff);
}

int findLogin(void *data, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        access = TRUE;
        printf("\nSuccessfully\n");
        strcpy(current_ID, argv[i]);
        return 0;
    }
    printf("\nAccess denied\n");
    return 0;
}

int CheckIsCommander(void *data, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        if (!strcmp("Commander", argv[i])) {
            isCommander = TRUE;
            break;
        }
    }
    return 0;
}

int getCurrentID(void *data, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        if (!strcmp("Commander", argv[i])) {
            strcpy(current_ID, argv[i]);
            break;
        }
    }
    return 0;
}

void AskParameterByID(sqlite3 *db, char *sql_print, char *msg, char *sql_aim, short isEnd) {
    printf("%s", msg);
    executeSQL(db, sql_print, printTable, NULL, FALSE);
    printf("\ninput>");
    AskParameter(NULL, sql_aim, TRUE, isEnd);
}

void Registration(sqlite3 *db) {
    char sql[1000] = "INSERT INTO Pilots values(null, ";

    AskParameter("\nEnter Surname: ", sql, FALSE, FALSE);

    AskParameterByID(db, "Select * from Positions;", "\nChoose position by ID:\n", sql, FALSE);
    char position_choice[100];
    strcpy(position_choice, current_buff);
    AskParameter("\nEnter experience: ", sql, TRUE, FALSE);

    AskParameter("\nEnter your address: ", sql, FALSE, FALSE);

    AskParameter("\nEnter your birth year: ", sql, TRUE, FALSE);

    AskParameterByID(db, "Select ID, Brand from Helicopters;", "\nChoose helicopter by ID:\n", sql, FALSE);

    AskParameter("\nEnter your login: ", sql, FALSE, FALSE);

    AskParameter("\nEnter your password: ", sql, FALSE, TRUE);

    if (executeSQL(db, sql, NULL, NULL, TRUE)) {
        access = TRUE;
        char sql_position[500] = "Select name from Positions where ID=";
        strcat(sql_position, position_choice);
        strcat(sql_position, ";");
        executeSQL(db, sql_position, CheckIsCommander, NULL, FALSE);

        char sql_id[] = "Select Max(id) from Pilots;";
        executeSQL(db, sql_id, getCurrentID, NULL, FALSE);
    }

}

void Login(sqlite3 *db) {
    char login[50], password[50];
    printf("\nEnter Login: ");
    fgets(login, 50, stdin);
    printf("\nEnter Password: ");
    fgets(password, 50, stdin);

    int size_l = strlen(login), size_p = strlen(password);
    if (size_l > 1) {
        login[size_l - 1] = '\0';
    }
    if (size_p > 1) {
        password[size_p - 1] = '\0';
    }
    char sql[500] = "Select ID from Pilots where login='";

    strcat(sql, login);
    strcat(sql, "' and password='");
    strcat(sql, password);
    strcat(sql, "';");
    if (executeSQL(db, sql, findLogin, NULL, FALSE)) {
        char sql_position[500] = "Select Positions.name from Positions inner join Pilots on Positions.ID=Pilots.position_id where Pilots.login='";
        strcat(sql_position, login);
        strcat(sql_position, "' and Pilots.password='");
        strcat(sql_position, password);
        strcat(sql_position, "';");
        executeSQL(db, sql_position, CheckIsCommander, NULL, FALSE);
    }
}

void getInfo(sqlite3 *db) {
    char buff[20];
    char sql[500];
    int des;
    if (isCommander) {
        printf("Choose info type:\n"
               "1. MY DATA\n"
               "2. HELICOPTERS;\n"
               "3. CREWS;\n"
               "4. FLIGHTS;\n"
               "5. TOTAL SPECIAL FLIGHTS STATISTICS;\n"
               "6. TOTAL FLIGHT TIME AFTER REPAIR AND USABILITY TIME FOR EACH HELICOPTER\n"
               "7. LIST FLIGHTS, TOTAL CARGO WEIGHT AND PEOPLE AMOUNT IN SELECTED PERIOD\n"
               "8. TOTAL CARGO WEIGHT, PEOPLE AMOUNT, TOTAL MONEY FOR HELICOPTERS' COMMON FLIGHTS\n"
               "9. TOTAL CARGO WEIGHT, PEOPLE AMOUNT, TOTAL MONEY FOR HELICOPTERS' SPECIAL FLIGHTS\n"
               "10. FOR HELICOPTER WITH MAX FLIGHTS AMOUNT GET LIST OF DONE FLIGHTS\n"
               "11. GET ALL DONE FLIGHTS INFO BY HELICOPTER ID OR PILOT ID\n"
               "12. GET LIST OF DONE FLIGHTS FOR CREW THAT EARNED MAX AMOUNT OF MONEY\n");

        fgets(buff, 20, stdin);
        des = atoi(buff);

        switch (des) {
            case 1:
                strcpy(sql,
                       "Select Pilots.id, Pilots.surname, Positions.name, Pilots.experience, Pilots.address, Pilots.birth_year, "
                       "Pilots.helicopter_id from Pilots inner join Positions on Pilots.position_id=Positions.ID where Pilots.ID=");
                strcat(sql, current_ID);
                strcat(sql, ";");
                break;
            case 2:
                strcpy(sql, "Select * from Helicopters;");
                break;
            case 3:
                strcpy(sql,
                       "Select Pilots.id, Pilots.surname, Positions.name, Pilots.experience, Pilots.address, Pilots.birth_year, "
                       "Pilots.helicopter_id from Pilots inner join Positions on Pilots.position_id=Positions.id order by Pilots.helicopter_id;");
                break;
            case 4:
                strcpy(sql,
                       "Select Flights.id, Flights_helicopter_id, Flights.date, Flights.mass_cargo, Flights.duration, Flights.price, "
                       "Types.name from Flights inner join Types where Flights.type_id=Types.id;");
                break;
            case 5:
                strcpy(sql,
                       "Select Count(Flights.id) as total_flights_amount, Sum(Flights.mass_cargo) as total_mass_cargo, Sum(Flights.price) as total_price from Flights "
                       " inner join Types on Flights.type_id = Types.id where Types.id=Special");
                break;
            case 6:
                strcpy(sql, "SELECT Helicopters.id, Helicopters.brand, Helicopters.flights_resource, "
                            "sum(Flights.duration) as total_duration FROM Helicopters INNER JOIN Flights on "
                            "Helicopters.id = Flights.helicopter_id WHERE Helicopters.last_overhaul_date < Flights.date "
                            "GROUP BY Helicopters.id;");
                break;
            case 7: {
                printf("Enter date(yyyy-mm-dd) of period start: ");
                fgets(buff, 20, stdin);
                strcpy(sql, "SELECT * FROM Flights WHERE Flights.date BETWEEN '");
                strcat(sql, buff);
                strcat(sql, "' AND '");
                printf("Enter date(yyyy-mm-dd) of period finish: ");
                char buff1[20];
                fgets(buff1, 20, stdin);
                strcat(sql, buff1);
                strcat(sql, "' ORDER BY helicopter_id;");
                printf("List of flights:\n");
                executeSQL(db, sql, printTable, NULL, FALSE);


                strcpy(sql, "SELECT helicopter_id, Helicopters.brand, sum(Flights.mass_cargo) as total_cargo_weight,"
                            " sum(Flights.people_amount) as total_people FROM Flights LEFT JOIN Helicopters on "
                            "Helicopters.id = Flights.helicopter_id WHERE Flights.date BETWEEN '");
                strcat(sql, buff);
                strcat(sql, "' AND '");
                strcat(sql, buff1);
                strcat(sql, "' GROUP BY helicopter_id;");
                printf("Total cargo weight and people amount:\n");
            }
                break;
            case 8:
                strcpy(sql,
                       "SELECT count(Flights.id) as flight_number, sum(Flights.mass_cargo) as total_cargo_weight, "
                       "sum(Flights.people_amount) as total_people_amount, sum(Flights.price) as total_money FROM Flights"
                       " WHERE Flights.type_id = 2 GROUP BY Flights.helicopter_id;");
                break;
            case 9:
                strcpy(sql,
                       "SELECT count(Flights.id) as flight_number, sum(Flights.mass_cargo) as total_cargo_weight, "
                       "sum(Flights.people_amount) as total_people_amount, sum(Flights.price) as total_money FROM Flights"
                       " WHERE Flights.type_id = 1 GROUP BY Flights.helicopter_id;");
                break;
            case 10:
                strcpy(sql, "SELECT Pilots.*, sum(Flights.price * Types.salary_ratio) as total_money_earned "
                            "FROM Flights LEFT JOIN Pilots on Pilots.helicopter_id = Flights.helicopter_id LEFT JOIN "
                            "Types on Flights.type_id = Types.id WHERE Flights.helicopter_id IN (SELECT "
                            "Flights.helicopter_id FROM Flights GROUP BY Flights.helicopter_id ORDER BY count(Flights.id)"
                            " DESC LIMIT 1) GROUP BY Pilots.id;");
                break;
            case 11:
                while(TRUE){
                    printf("SELECT by:\n"
                           "1. helicopter id\n"
                           "2. pilot id\n");
                    {
                        char choice_buff[10];
                        fgets(choice_buff, 10, stdin);
                        int choice = atoi(choice_buff);
                        switch(choice){
                            case 1:
                                printf("Enter the helicopter id\n");
                                fgets(buff, 20, stdin);
                                strcpy(sql, "SELECT * FROM Flights WHERE Flights.helicopter_id = ");
                                strcat(sql, buff);
                                strcat(sql, ";");
                                break;
                            case 2:
                                printf("Enter the pilot id\n");
                                fgets(buff, 20, stdin);
                                strcpy(sql, "SELECT Flights.* FROM Flights LEFT JOIN Pilots ON Flights.helicopter_id = "
                                            "Pilots.helicopter_id WHERE Pilots.id = ");
                                strcat(sql, buff);
                                strcat(sql, ";");
                                break;
                            default:
                                printf("Wrong parameter\n");
                                continue;
                        }
                    }
                    break;
                }
                break;
            case 12:
                strcpy(sql, "SELECT * FROM Flights WHERE Flights.helicopter_id IN (SELECT helicopter_id FROM "
                            "Flights LEFT JOIN Types on Flights.type_id = Types.id GROUP BY helicopter_id ORDER BY "
                            "Flights.price * Types.salary_ratio DESC LIMIT 1);");
                break;
            default:
                printf("Wrong parameter\n");
                return;
        }
    } else {
        printf("Choose info type:\n"
               "1. MY DATA;\n"
               "2. MY HELICOPTER;\n"
               "3. MY HELICOPTER TOTAL FLIGHT TIME AFTER REPAIR AND USABILITY TIME\n"
               "4. LIST FLIGHTS, TOTAL CARGO WEIGHT AND PEOPLE AMOUNT IN SELECTED PERIOD\n"
               "5. GET MY DONE FLIGHTS\n");

        fgets(buff, 20, stdin);
        des = atoi(buff);

        switch (des) {
            case 1:
                strcpy(sql,
                       "Select Pilots.id, Pilots.surname, Positions.name, Pilots.experience, Pilots.address, Pilots.birth_year, "
                       "Pilots.helicopter_id from Pilots inner join Positions on Pilots.position_id=Positions.ID where Pilots.ID=");
                strcat(sql, current_ID);
                strcat(sql, ";");
                break;
            case 2:
                strcpy(sql,
                       "Select Helicopters.* from Helicopters inner join Pilots on Helicopters.id=Pilots.helicopter_id where Pilots.id=");
                strcat(sql, current_ID);
                strcat(sql, ";");
                break;
            case 3:
                strcpy(sql, "SELECT Helicopters.id, Helicopters.brand, Helicopters.flights_resource, "
                            "sum(Flights.duration) as total_duration FROM Flights LEFT JOIN Helicopters LEFT JOIN Pilots "
                            "on Helicopters.id = Flights.helicopter_id and Flights.helicopter_id = Pilots.helicopter_id "
                            "WHERE Helicopters.last_overhaul_date < Flights.date and Pilots.id = ");
                strcat(sql, current_ID);
                strcat(sql, ";");
                break;
            case 4: {
                printf("Enter date(yyyy-mm-dd) of period start: ");
                fgets(buff, 20, stdin);
                strcpy(sql, "SELECT Flights.* FROM Flights LEFT JOIN Pilots on Pilots.helicopter_id = "
                            "Flights.helicopter_id WHERE Flights.date BETWEEN '");
                strcat(sql, buff);
                strcat(sql, "' AND '");
                printf("Enter date(yyyy-mm-dd) of period finish: ");
                char buff1[20];
                fgets(buff1, 20, stdin);
                strcat(sql, buff1);
                strcat(sql, "' and Pilots.id = ");
                strcat(sql, current_ID);
                strcat(sql, ";");
                printf("List of flights:\n");
                executeSQL(db, sql, printTable, NULL, FALSE);


                strcpy(sql, "SELECT Flights.helicopter_id, Helicopters.brand, sum(Flights.mass_cargo) as "
                            "total_cargo_weight, sum(Flights.people_amount) as total_people FROM Flights LEFT JOIN "
                            "Helicopters LEFT JOIN Pilots on Pilots.helicopter_id = Flights.helicopter_id and Helicopters.id"
                            " = Flights.helicopter_id WHERE Flights.date BETWEEN '");
                strcat(sql, buff);
                strcat(sql, "' AND '");
                strcat(sql, buff1);
                strcat(sql, "' and Pilots.id = ");
                strcat(sql, current_ID);
                strcat(sql, ";");
                printf("Total cargo weight and people amount:\n");
            }
                break;
            case 5:
                strcpy(sql, "SELECT Flights.* FROM Flights LEFT JOIN Pilots ON Flights.helicopter_id = "
                            "Pilots.helicopter_id WHERE Pilots.id = ");
                strcat(sql, current_ID);
                strcat(sql, ";");
                break;
            default:
                printf("Wrong parameter\n");
                return;
        }
    }

    executeSQL(db, sql, printTable, NULL, FALSE);

}

int check_helicopter(void *data, int argc, char **argv, char **azColName) {
    if (!argv[0]){
        duration_check_passed = FALSE;
    }
    duration_check_passed = TRUE;
    return 0;
}

void check_helicopter_flight_time(sqlite3* db, char* helicopter_id, char* duration){
    char sql[300];
    strcpy(sql,"SELECT * FROM Helicopters WHERE Helicopters.id = ");
    strcat(sql, helicopter_id);
    strcat(sql, " AND Helicopters.flights_resource > ");
    strcat(sql, duration);
    strcat(sql, ";");
    executeSQL(db, sql, check_helicopter, NULL, FALSE);
}

int AskFlightDuration(const char* msg, char* sql, sqlite3* db, char* helicopter_id){
    printf("%s", msg);
    char buffer[10];
    fgets(buffer, 10, stdin);
    check_helicopter_flight_time(db, helicopter_id, buffer);
    if (duration_check_passed == FALSE){
        return -1;
    }
    strcat(sql, buffer);
    strcat(sql, ", ");
    return 0;
}

void InsertIntoDB(sqlite3 *db) {
    char sql[100] = "INSERT INTO ";

    char buff[20];
    int des;
    printf("\nChoose the table:\n"
           "1. Helicopters;\n"
           "2. Flights;\n"
           "3. Types;\n"
           "4. Positions\n");


    fgets(buff, 20, stdin);
    des = atoi(buff);

    if (des == 1) {
        strcat(sql, " Helicopters values(null, ");

        AskParameter("\nEnter brand: ", sql, FALSE, FALSE);

        AskParameter("\nEnter production date(yyyy-mm-dd): ", sql, FALSE, FALSE);

        AskParameter("\nEnter load capacity: ", sql, TRUE, FALSE);

        AskParameter("\nEnter last overhaul date: ", sql, FALSE, FALSE);

        AskParameter("\nEnter flight resource: ", sql, TRUE, TRUE);
    } else if (des == 2) {
        strcat(sql, " Flights values(null, ");

        AskParameterByID(db, "Select id, brand from Helicopters;", "Choose helicopter by ID:", sql, FALSE);
        char helicopter_id[10];
        strcpy(helicopter_id, entered_helicopter_id);

        AskParameter("\nEnter date(yyyy-mm-dd): ", sql, FALSE, FALSE);

        AskParameter("\nEnter mass cargo: ", sql, TRUE, FALSE);

        AskParameter("\nEnter people amount: ", sql, TRUE, FALSE);

        AskParameter("\nEnter price: ", sql, TRUE, FALSE);

        if (AskFlightDuration("\nEnter flight duration: ", sql, db, helicopter_id) == -1){
            printf("Unable to insert flight with such duration to this helicopter\n");
            return;
        }

        AskParameterByID(db, "Select * from Types;", "\nChoose type by ID:", sql, TRUE);

    } else if (des == 3) {
        strcat(sql, " Types values(null, ");

        AskParameter("\nEnter name: ", sql, FALSE, FALSE);

        AskParameter("\nEnter salary ratio: ", sql, TRUE, TRUE);
    } else if (des == 4) {
        strcat(sql, " Positions values(null, ");

        AskParameter("\nEnter name: ", sql, FALSE, TRUE);
    } else {
        printf("\nWrong parameter\n");
        return;
    }
    executeSQL(db, sql, NULL, NULL, TRUE);
}

void SelectDeleteFromDB(sqlite3 *db, short to_delete) {
    char buff[20], sql[1000];
    if (to_delete)
        strcpy(sql, "DELETE FROM ");
    else
        strcpy(sql, "SELECT * FROM ");

    int des;
    printf("\nChoose the table:\n"
           "1. Helicopters;\n"
           "2. Flights;\n"
           "3. Pilots;\n"
           "4. Types;\n");

    fgets(buff, 20, stdin);
    des = atoi(buff);

    if (des == 1) {
        strcat(sql, "Helicopters");
    } else if (des == 2) {
        strcat(sql, "Flights");
    } else if (des == 3) {
        strcat(sql, "Pilots");
    } else if (des == 4) {
        strcat(sql, "Types");
    } else {
        printf("\nWrong parameter");
        return;
    }

    printf("BY:\n"
           "1. ID;\n"
           "2. All\n");

    fgets(buff, 20, stdin);
    des = atoi(buff);

    if (des == 1) {
        printf("\nEnter ID: ");
        fgets(buff, 20, stdin);
        strcat(sql, " where ID=");
        strcat(sql, buff);
        strcat(sql, ";");
    } else if (des == 2) {
        strcat(sql, ";");
    } else {
        printf("\nWrong parameter");
        return;
    }
    if (to_delete) {
        executeSQL(db, sql, NULL, NULL, TRUE);
    } else {
        executeSQL(db, sql, printTable, NULL, TRUE);
    }
}

void AdminAction(sqlite3 *db) {
    char buff[20];
    int des;
    printf("\nChoose the action:\n"
           "1. INSERT;\n"
           "2. SELECT;\n"
           "3. DELETE;\n"
           "4. UPDATE;\n");

    fgets(buff, 20, stdin);
    des = atoi(buff);

    switch (des) {
        case 1:
            InsertIntoDB(db);
            break;
        case 2:
            SelectDeleteFromDB(db, FALSE);
        case 3:
            SelectDeleteFromDB(db, TRUE);
            break;
        case 4:
            break;
        default:
            printf("\nWrong parameter");
            break;
    }
}



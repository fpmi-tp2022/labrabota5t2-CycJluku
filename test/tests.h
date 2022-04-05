#ifndef Tests_h
#define Tests_h

#include "../include/server.h"
#include "sqlite3.h"

#define S_OK (0)
#define E_FAIL (-1)

int test_get_crews_info()
{
    sqlite3* db;
    int result = sqlite3_open("../doc/air_cab.db", &db);
    if (result == FALSE)
        return E_FAIL;
    char sql[500];

    get_crews_info(sql);

    if(executeSQL(db, sql, NULL, NULL, FALSE))
    {
        sqlite3_close(db);
        return S_OK;
    }
    else {
        sqlite3_close(db);
        return E_FAIL;
    }
}

int test_get_commander_info()
{
    sqlite3* db;
    int result = sqlite3_open("../doc/air_cab.db", &db);
    if (result == FALSE)
        return E_FAIL;
    char sql[500];

    get_commander_info(sql, "2");

    if(executeSQL(db, sql, NULL, NULL, FALSE))
    {
        sqlite3_close(db);
        return S_OK;
    }
    else {
        sqlite3_close(db);
        return E_FAIL;
    }
}

int test_get_helicopter_info()
{
    sqlite3* db;
    int result = sqlite3_open("../doc/air_cab.db", &db);
    if (result == FALSE)
        return E_FAIL;
    char sql[500];

    get_helicopters_info(sql);

    if(executeSQL(db, sql, NULL, NULL, FALSE))
    {
        sqlite3_close(db);
        return S_OK;
    }
    else {
        sqlite3_close(db);
        return E_FAIL;
    }
}

int test_get_flights_info()
{
    sqlite3* db;
    int result = sqlite3_open("../doc/air_cab.db", &db);
    if (result == FALSE)
        return E_FAIL;
    char sql[500];

    get_flights_info(sql);

    if(executeSQL(db, sql, NULL, NULL, FALSE))
    {
        sqlite3_close(db);
        return S_OK;
    }
    else {
        sqlite3_close(db);
        return E_FAIL;
    }
}

int test_get_special_flights_info()
{
    sqlite3* db;
    int result = sqlite3_open("../doc/air_cab.db", &db);
    if (result == FALSE)
        return E_FAIL;
    char sql[500];

    get_special_flights_info(sql);

    if(executeSQL(db, sql, NULL, NULL, FALSE))
    {
        sqlite3_close(db);
        return S_OK;
    }
    else {
        sqlite3_close(db);
        return E_FAIL;
    }
}

int test_get_repair_and_resource_info()
{
    sqlite3* db;
    int result = sqlite3_open("../doc/air_cab.db", &db);
    if (result == FALSE)
        return E_FAIL;
    char sql[500];

    get_repair_and_resource_info(sql);

    if(executeSQL(db, sql, NULL, NULL, FALSE))
    {
        sqlite3_close(db);
        return S_OK;
    }
    else {
        sqlite3_close(db);
        return E_FAIL;
    }
}

int test_get_common_flights_cargo_and_people_info()
{
    sqlite3* db;
    int result = sqlite3_open("../doc/air_cab.db", &db);
    if (result == FALSE)
        return E_FAIL;
    char sql[500];

    get_common_flights_cargo_and_people_info(sql);

    if(executeSQL(db, sql, NULL, NULL, FALSE))
    {
        sqlite3_close(db);
        return S_OK;
    }
    else {
        sqlite3_close(db);
        return E_FAIL;
    }
}

int  test_get_special_flights_cargo_and_people_info()
{
    sqlite3* db;
    int result = sqlite3_open("../doc/air_cab.db", &db);
    if (result == FALSE)
        return E_FAIL;
    char sql[500];

    get_special_flights_cargo_and_people_info(sql);

    if(executeSQL(db, sql, NULL, NULL, FALSE))
    {
        sqlite3_close(db);
        return S_OK;
    }
    else {
        sqlite3_close(db);
        return E_FAIL;
    }
}

int test_get_flights_of_max_flight_amount_helicopter()
{
    sqlite3* db;
    int result = sqlite3_open("../doc/air_cab.db", &db);
    if (result == FALSE)
        return E_FAIL;
    char sql[500];

    get_flights_of_max_flight_amount_helicopter(sql);

    if(executeSQL(db, sql, NULL, NULL, FALSE))
    {
        sqlite3_close(db);
        return S_OK;
    }
    else {
        sqlite3_close(db);
        return E_FAIL;
    }
}
int test_get_flights_of_max_money_crew()
{

    sqlite3* db;
    int result = sqlite3_open("../doc/air_cab.db", &db);
    if (result == FALSE)
        return E_FAIL;
    char sql[500];

    get_flights_of_max_money_crew(sql);

    if(executeSQL(db, sql, NULL, NULL, FALSE))
    {
        sqlite3_close(db);
        return S_OK;
    }
    else {
        sqlite3_close(db);
        return E_FAIL;
    }
}

#endif
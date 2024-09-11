#ifndef DATA_FETCHER_H
#define DATA_FETCHER_H

#include <stdbool.h>
#include <mariadb/mysql.h>

void list_doencas(MYSQL* connection, bool report);
void search_symptoms();

#endif // DATA_FETCHER_H

#ifndef DATA_FETCHER_H
#define DATA_FETCHER_H

#include <mariadb/mysql.h>

void list_doencas(MYSQL* connection);
void search_symptoms();

#endif // DATA_FETCHER_H

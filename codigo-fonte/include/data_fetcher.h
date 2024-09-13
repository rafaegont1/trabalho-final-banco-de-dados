#ifndef DATA_FETCHER_H
#define DATA_FETCHER_H

#include <mariadb/mysql.h>

void list_doencas(MYSQL* connection);
void diagnostic(MYSQL* connection);
void make_report(MYSQL* connection);

#endif // DATA_FETCHER_H

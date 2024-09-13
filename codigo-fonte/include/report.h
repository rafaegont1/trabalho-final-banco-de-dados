#ifndef REPORT_H
#define REPORT_H

#include <mariadb/mysql.h>

void create_doenca_report(MYSQL_ROW* doenca);
void create_diagnostic_report(MYSQL_RES* result);

#endif // REPORT_H

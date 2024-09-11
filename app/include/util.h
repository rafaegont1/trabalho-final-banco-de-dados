#ifndef UTIL_H
#define UTIL_H

#include <mariadb/mysql.h>

#define COLOR_RED "\e[31m"
#define COLOR_YELLOW "\e[33m"
#define COLOR_RESET "\e[m"
#define DASHED_LINE "----------------------------------------\n"
#define DOUBLE_DASHED_LINE "========================================\n"

enum log_type {
    LOG_CADASTRO  = 0,
    LOG_CONSULTA  = 1,
    LOG_RELATORIO = 2
};

void clear_stdin();
int printf_error(const char* fmt, ...);
void scanf_and_clear_stdin(const char* fmt, void* dst, const char* msg);
int read_line(MYSQL* connection, char* str);
void write_log(enum log_type type);
void mariadb_error_handler(MYSQL* connection);

#endif // UTIL_H

#ifndef UTIL_H
#define UTIL_H

#include <mariadb/mysql.h>

enum log_type {
    LOG_CADASTRO = 0,
    LOG_CONSULTAS = 1,
    LOG_EMISSAO_RELATORIOS = 2
};

void clear_buffer();
void mariadb_error(MYSQL* connection);
MYSQL* init();
void write_log(enum log_type type);

#endif // UTIL_H

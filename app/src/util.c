#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void clear_stdin() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int printf_error(const char* fmt, ...) {
    int ret;
    va_list args;

    va_start(args, fmt);

    fprintf(stderr, COLOR_RED);
    ret = vfprintf(stderr, fmt, args);
    fprintf(stderr, COLOR_RESET);

    va_end(args);

    return ret;
}

void scanf_and_clear_stdin(const char* fmt, void* dst, const char* msg) {
    printf("%s: ", msg);
    while (scanf(fmt, dst) != 1) {
        clear_stdin();
        printf(
            COLOR_YELLOW "scanf() falhou\n"
            COLOR_RESET  "%s novamente: ", msg
        );
    }
    clear_stdin();
}

// Return 0 if it's successful; return 1 if it fails
int read_line(MYSQL* connection, char* str) {
    char tmp[256];

    if (fgets(tmp, sizeof(tmp), stdin) == NULL) {
        printf_error("fgets() falhou\n");
        str[0] = '\0';
        return 1;
    }

    tmp[strcspn(tmp, "\n")] = '\0';
    mysql_real_escape_string(connection, str, tmp, strlen(tmp));

    return 0;
}

void write_log(const enum log_type type) {
    static const char* LOG_TYPE_STR[] = {
        "cadastro", "consulta", "emissão de relatório",
    };

    FILE* fp;
    time_t now;
    struct tm* local_time;

    if ((fp = fopen("log.txt", "a")) == NULL) {
        printf_error("Erro ao tentar abrir o log para gravar\n");
    }

    time(&now);
    local_time = localtime(&now);

    fprintf(fp,
        "Operação \"%s\" realizada na data %02d/%02d/%04d"
        " às %02d:%02d:%02d horas\n", LOG_TYPE_STR[type],
        local_time->tm_mday,
        local_time->tm_mon + 1,
        local_time->tm_year + 1900,
        local_time->tm_hour,
        local_time->tm_min,
        local_time->tm_sec
    );

    fclose(fp);
}

void mariadb_error_handler(MYSQL* connection) {
    printf_error("mariadb error: %s\n", mysql_error(connection));
}

MYSQL_RES* mariadb_get_result(MYSQL* connection, const char* query) {
    MYSQL_RES* result;

    if (mysql_query(connection, query) != 0) {
        mariadb_error_handler(connection);
        result = NULL;
    } else if ((result = mysql_store_result(connection)) == NULL) {
        mariadb_error_handler(connection);
    }

    return result;
}

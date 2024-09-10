#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void clear_buffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void mariadb_error(MYSQL* connection) {
    fprintf(stderr, "\e[31mmariadb error:\e[m\n: %s\n", mysql_error(connection));
    // mysql_close(connection);
    // exit(EXIT_FAILURE);
}

MYSQL* init() {
    // Inicializa a estrutura de conexão
    MYSQL* connection = mysql_init(NULL);
    if (connection == NULL) {
        fprintf(stderr, "mysql_init() falhou\n");
        exit(EXIT_FAILURE);
    }

    // Conecta ao banco de dados
    if (mysql_real_connect(
            connection,  // Connection
            "127.0.0.1", // Host
            "rafael",    // User account
            "2432",      // User password
            "trabalho",  // Default database
            3306,        // Port number
            NULL,        // Path tot socket file
            0)           // Additional options
        == NULL)
    {
        mariadb_error(connection);
    }

    return connection;
}

// É desejável que o software mantenha um sistema de logging que registre todas
// as operações realizadas (cadastros, consultas, emissão de relatórios).
// Os logs devem incluir informações de data, hora e tipo de operação realizada.
// Os logs devem mantidos em arquivos texto puro.
void write_log(enum log_type type) {
    static const char* TYPE_STR[] = {
        "cadastro",
        "consulta",
        "emissão de relatório",
    };
    FILE* fp;
    time_t now;
    struct tm* local_time;

    if ((fp = fopen("log.txt", "a")) == NULL) {
        fprintf(stderr, "Erro ao tentar abrir o log para gravar\n");
    }

    time(&now);
    local_time = localtime(&now);

    fprintf(
        fp,
        "Operação \"%s\" realizada na data %02d/%02d/%04d às %02d:%02d:%02d horas\n",
        TYPE_STR[type],
        local_time->tm_mday,
        local_time->tm_mon + 1,
        local_time->tm_year + 1900,
        local_time->tm_hour,
        local_time->tm_min,
        local_time->tm_sec
    );

    fclose(fp);
}

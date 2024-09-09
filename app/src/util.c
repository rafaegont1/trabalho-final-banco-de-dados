#include "util.h"

#include <stdio.h>
#include <stdlib.h>

void clear_buffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void exit_error(MYSQL* connection) {
    fprintf(stderr, "mariadb error: %s\n", mysql_error(connection));
    mysql_close(connection);
    exit(EXIT_FAILURE);
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
        exit_error(connection);
    }

    return connection;
}

void write_log(const char* query) {
    FILE* fp = fopen("activity.log", "a");
    if (fp == NULL) {
        fprintf(stderr, "Erro ao tentar abrir o log para gravar\n");
    }

    fprintf(fp, "%s;", query);

    fclose(fp);
}

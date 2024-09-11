#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>
#include "util.h"
#include "data_manager.h"
#include "data_fetcher.h"

static MYSQL* mariadb_init();
static int show_menu();

int main(void) {
    MYSQL* connection = mariadb_init();
    int op;

    do {
        op = show_menu();

        switch (op) {
            case 1:  insertion_menu(connection); break;
            case 2:  list_doencas(connection); break;
            case 3:  search_symptoms(connection); break;
            case 4:  break; // Emissão de Relatórios
            case 5:  printf("Saindo...\n"); break;
            default: printf("Comando não definido\n"); break;
        }

    } while (op != 5);

    mysql_close(connection);

    return 0;
}

static MYSQL* mariadb_init() {
    MYSQL* connection = mysql_init(NULL);

    if (connection == NULL) {
        printf_error("mysql_init() falhou\n");
        exit(EXIT_FAILURE);
    }

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
        printf_error("Não foi possível conectar com os dados informados\n");
        mariadb_error_handler(connection);
        exit(EXIT_FAILURE);
    }

    return connection;
}

static int show_menu() {
    int op;

    printf(
        DOUBLE_DASHED_LINE
        "1) Gerenciamento de Dados\n"
        "2) Consulta ao Catálogo de Doenças\n"
        "3) Apoio ao Diagnóstico\n"
        "4) Emissão de Relatórios\n"
        "5) Sair\n"
        DOUBLE_DASHED_LINE
    );
    scanf_and_clear_stdin("%d", &op, "Digite o comando");
    printf(DASHED_LINE);

    return op;
}

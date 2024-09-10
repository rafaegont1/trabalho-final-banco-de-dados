#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>
#include "util.h"
#include "data_manager.h"
#include "data_fetcher.h"

int main() {
    MYSQL* connection = init();
    int op;

    do {
        static const char* MENU =
            "========================================\n"
            "1) Gerenciamento de Dados\n"
            "2) Consulta ao Catálogo de Doenças\n"
            "3) Apoio ao Diagnóstico\n"
            "4) Emissão de Relatórios\n"
            "5) Sair\n"
            "========================================\n"
            "Digite o comando: ";

        printf("%s", MENU);
        if (scanf("%d", &op) != 1) {
            fprintf(stderr, "Erro ao ler o comando, digite novamente: ");
            clear_buffer();
        }
        clear_buffer();
        printf("----------------------------------------\n");

        switch (op) {
            case 1:  insertion_menu(connection); break;
            case 2:  list_doencas(connection); break;
            case 3:  search_symptoms(); break;
            case 4:  break; // Emissão de Relatórios
            case 5:  printf("Saindo...\n"); break;
            default: printf("Comando não definido\n"); break;
        }

    } while (op != 5);

    mysql_close(connection);

    return 0;
}

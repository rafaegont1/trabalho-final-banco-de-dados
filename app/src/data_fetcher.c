#include "data_fetcher.h"

#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void search_doencas(MYSQL* connection) {
    MYSQL_RES* result = NULL;
    MYSQL_ROW row;

    static const char* DOENCAS =
    "SELECT d.id, d.nome, d.cid, p.nome AS patogeno, pt.nome AS tipo, "
    "    GROUP_CONCAT( "
    "        CONCAT(s.nome, ' (', ds.ocorrencia, ')') "
    "        ORDER BY FIELD(ds.ocorrencia, "
    "            'muito comum', 'comum', 'pouco comum', 'raro', 'muito raro'), "
    "            s.nome "
    "        SEPARATOR ', ' "
    "    ) AS sintomas "
    "FROM doencas AS d "
    "JOIN doenca_sintoma AS ds ON ds.id_doenca = d.id "
    "JOIN sintomas AS s ON ds.id_sintoma = s.id "
    "JOIN patogenos AS p ON p.id = d.id_patogeno "
    "JOIN patogeno_tipo AS pt ON pt.id = p.id_tipo "
    "GROUP BY d.id "
    "ORDER BY d.nome;";

    if (mysql_query(connection, DOENCAS) != 0) {
        exit_error(connection);
    }

    result = mysql_store_result(connection);
    if (result == NULL) {
        exit_error(connection);
    }

    printf("<DOENÇAS>\n");
    printf("\n");
    while ((row = mysql_fetch_row(result)) != NULL) {
        printf("ID: %s\n", row[0]);
        printf("Nome: %s\n", row[1]);
        printf("CID: %s\n", row[2]);
        printf("Patógeno: %s\n", row[3]);
        printf("Tipo de patógeno: %s\n", row[4]);
        printf("Sintomas: %s\n", row[5]);
        printf("\n");
    }

    mysql_free_result(result);
}

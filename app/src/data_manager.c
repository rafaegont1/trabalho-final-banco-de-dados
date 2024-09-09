#include "data_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

void insert_doenca(MYSQL* connection) {
    char query[256];
    char nome[32];
    char esc_nome[32];
    char cid[8];
    char esc_cid[32];
    int id_patogeno;

    printf("Digite o nome técnico da doença: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("Digite o CID da doença: ");
    fgets(cid, sizeof(cid), stdin);
    cid[strcspn(cid, "\n")] = '\0';

    printf("Digite o ID do patógeno causador da doença: ");
    scanf("%d", &id_patogeno);

    // Protect against SQL injection
    mysql_real_escape_string(connection, esc_nome, nome, strlen(nome));
    mysql_real_escape_string(connection, esc_cid, cid, strlen(cid));

    snprintf(
        query,
        sizeof(query),
        "INSERT INTO doencas (nome, cid, id_patogeno) VALUES"
        "    (\'%s\', \'%s\', \'%d\')",
        nome, cid, id_patogeno
    );

    if (mysql_query(connection, query) != 0) {
        exit_error(connection);
    }

    write_log(query);
}

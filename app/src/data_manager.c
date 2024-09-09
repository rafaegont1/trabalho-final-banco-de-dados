#include "data_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

void insert_doenca(MYSQL* connection) {
    doencas_t d;
    char query[256];

    printf("Digite o nome técnico da doença: ");
    fgets(d.nome, sizeof(d.nome), stdin);
    d.nome[strcspn(d.nome, "\n")] = '\0';

    printf("Digite o CID da doença: ");
    fgets(d.cid, sizeof(d.cid), stdin);
    d.cid[strcspn(d.cid, "\n")] = '\0';

    printf("Digite o ID do patógeno causador da doença: ");
    scanf("%d", &d.id_patogeno);

    // Protect against SQL injection
    mysql_real_escape_string(connection, d.esc_nome, d.nome, strlen(d.nome));
    mysql_real_escape_string(connection, d.esc_cid, d.cid, strlen(d.cid));

    snprintf(
        query,
        sizeof(query),
        "INSERT INTO doencas (nome, cid, id_patogeno) VALUES"
        "    (\'%s\', \'%s\', \'%d\')",
        d.nome, d.cid, d.id_patogeno
    );

    if (mysql_query(connection, query) != 0) {
        exit_error(connection);
    }

    write_log(query);
}

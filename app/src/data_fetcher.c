#include "data_fetcher.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mariadb_com.h"
#include "util.h"

static int show_menu() {
    int op;

    printf(
        "1) Listar todas as doenças\n"
        "2) Consultar por nome técnico\n"
        "3) Consultar por nome popular\n"
        "4) Consultar por CID\n"
        "5) Consultar por patógeno\n"
        "6) Voltar\n"
        "Digite o comando: "
    );
    if ((scanf("%d", &op) != 1) || (op < 1) || (op > 6)) {
        fprintf(stderr, "Erro ao ler o comando, digite novamente: ");
        clear_buffer();
    }
    clear_buffer();

    return op;
}

static const char* get_where_clause(MYSQL* connection, const int op) {
    static const char* DOENCA_COLUMN[] = {
        "d.nome",  // nome técnico
        "np.nome", // nome popular
        "d.cid",   // CID
        "p.id"     // patógeno
    };

    static char clause[128];
    char to_find[64];
    char esc_to_find[64];

    strcpy(clause, "WHERE ");
    strcat(clause, DOENCA_COLUMN[op - 2]);
    strcat(clause, " = ");

    printf(
        "Digite o %s da doença: ",
        op == 2 ? "nome técnico" :
        op == 3 ? "nome popular" :
        op == 4 ? "CID" :
        op == 5 ? "patógeno causador" :
        ""
    );
    fgets(to_find, sizeof(to_find), stdin);
    to_find[strcspn(to_find, "\n")] = '\0';

    mysql_real_escape_string(connection, esc_to_find, to_find, strlen(to_find));

    strcat(clause, "\'");
    strcat(clause, esc_to_find);
    strcat(clause, "\'");

    // printf("WHERE CLAUSE: %s\n", clause); // rascunho

    return clause;
}

// O software deve possibilitar ao usuário listar as doenças cadastradas e,
// também, pesquisar uma doença pelo nome técnico, nome popular, CID ou
// patógeno. Ao selecionar uma doença, devem ser exibidos todos os seus dados,
// incluindo seus sintomas.
void list_doencas(MYSQL* connection, bool report) {
    MYSQL_RES* result = NULL;
    MYSQL_ROW row;
    int op;
    char query[1024];
    const char* where_clause = NULL;

    op = show_menu();

    if (op == 6) {
        printf("Voltando...\n");
        return;
    } else if (op != 1) {
        where_clause = get_where_clause(connection, op);
    }

    printf("----------------------------------------\n");

    snprintf(query, sizeof(query),
        "SELECT d.id, d.nome, d.cid, p.nome AS pat_nome, pt.nome AS pat_tipo, "
        "    GROUP_CONCAT(DISTINCT np.nome ORDER BY np.nome SEPARATOR ', ') AS nomes_pop, "
        "    GROUP_CONCAT(CONCAT(s.nome, ' (', ds.ocorrencia, ')') "
        "        ORDER BY FIELD(ds.ocorrencia, 'muito comum', 'comum', 'pouco comum', 'raro', 'muito raro'), s.nome "
        "        SEPARATOR ', ') AS sintomas "
        "FROM doencas AS d "
        "LEFT JOIN doenca_sintoma AS ds ON ds.id_doenca = d.id "
        "LEFT JOIN sintomas AS s ON s.id = ds.id_sintoma "
        "LEFT JOIN patogenos AS p ON p.id = d.id_patogeno "
        "LEFT JOIN patogeno_tipo AS pt ON pt.id = p.id_tipo "
        "LEFT JOIN nomes_pop AS np ON np.id_doenca = d.id "
        "%s "
        "GROUP BY d.id, d.nome, d.cid, p.nome, pt.nome "
        "ORDER BY d.nome;",
        where_clause != NULL ? where_clause : ""
    );
    // printf("QUERY: ---%s---\n", query); // rascunho

    if (mysql_query(connection, query) != 0) {
        mariadb_error(connection);
        return;
    }

    if ((result = mysql_store_result(connection)) == NULL) {
        mariadb_error(connection);
        return;
    }

    while ((row = mysql_fetch_row(result)) != NULL) {
        printf("ID: %s\n", row[0]);
        printf("Nome: %s\n", row[1]);
        printf("CID: %s\n", row[2]);
        printf("Nome do patógeno: %s\n", row[3]);
        printf("Tipo de patógeno: %s\n", row[4]);
        printf("Nomes populares: %s\n", row[5]);
        printf("Sintomas: %s\n", row[6]);
        printf("----------------------------------------\n");
    }

    write_log(LOG_CONSULTAS);
    mysql_free_result(result);
}

void search_symptoms() {
    static const int MAX_COUNT = 16;

    int count = 0;
    char symptoms[1024] = "";
    char tmp[64] = "";
    char op;

    // 'febre', 'diarreia', 'tosse'
    do {
        if (count > MAX_COUNT) {
            printf("Número máximo de sintomas a pesquisar permitido é 16\n");
            break;
        }

        printf("Digite o nome do sintoma: ");
        fgets(tmp, sizeof(tmp), stdin);
        tmp[strcspn(tmp, "\n")] = '\0';

        if (count > 0) {
            strcat(symptoms, ", ");
        }

        strcat(symptoms, "\'");
        strcat(symptoms, tmp);
        strcat(symptoms, "\'");

        do {
            printf("Inserir mais um sintoma (s ou n): ");
            scanf("%c", &op);
            clear_buffer();
            op = tolower(op);
        } while (op != 's' && op != 'n');

        count++;

    } while (op != 'n');

    printf("Sintomas a pesquisar: %s\n", symptoms); // rascunho
}

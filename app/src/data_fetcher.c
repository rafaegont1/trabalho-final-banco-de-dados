#include "data_fetcher.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

// O software deve possibilitar ao usuário listar as doenças cadastradas e,
// também, pesquisar uma doença pelo nome técnico, nome popular, CID ou
// patógeno. Ao selecionar uma doença, devem ser exibidos todos os seus dados,
// incluindo seus sintomas.
void list_doencas(MYSQL* connection) {
    MYSQL_RES* result = NULL;
    MYSQL_ROW row;
    int op;
    char query[1024];
    char find_where[128];

    static const char* MENU =
        "1) Listar todas as doenças\n"
        "2) Consultar por nome técnico\n"
        "3) Consultar por nome popular\n"
        "4) Consultar por CID\n"
        "5) Consultar por patógeno\n"
        "6) Voltar\n";

    printf("%s\n", MENU);
    if ((scanf("%d", &op) != 1) || (op < 1) || (op > 6)) {
        fprintf(stderr, "Erro ao ler o comando, digite novamente: ");
        clear_buffer();
    }
    clear_buffer();

    if (op == 6) return;

    printf(
        "Digite o %s da doença: ",
        op == 2 ? "nome técnico" :
        op == 3 ? "nome popular" :
        op == 4 ? "CID" :
        op == 5 ? "patógeno causador" :
        ""
    );
    fgets(find_where, sizeof(find_where), stdin);
    find_where[strcspn(find_where, "\n")] = '\0';

    printf("----------------------------------------\n");

    static const char* OPTIONS[] = {
        "",
        "d.nome",  // nome técnico
        "np.nome", // nome popula
        "d.cid",   // CID
        "p.id"     // patógeno
    };

    if (op == 1) {
    } else {
    }
    snprintf(query, strlen(query),

    );
    if (mysql_query(connection, DOENCAS) != 0) {
      exit_error(connection);
    }

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

    snprintf(
        query, sizeof(query),
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
        "%s "
        "GROUP BY d.id "
        "ORDER BY d.nome;",
        where_clause
    );

    // if (mysql_query(connection, DOENCAS) != 0) {
    //     exit_error(connection);
    // }

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
        printf("----------------------------------------\n");
    }

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

    printf("Sintomas a pesquisar: %s\n", symptoms);
}

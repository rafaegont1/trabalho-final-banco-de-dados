#include "data_fetcher.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb_com.h>
#include "pdfgen.h"
#include "report.h"
#include "util.h"

// -----------------------------------------------------------------------------
// LISTAR DOENÇAS

static int show_menu_list_doencas() {
    int op;

    printf(
        "1) Listar todas as doenças\n"
        "2) Consultar por nome técnico\n"
        "3) Consultar por nome popular\n"
        "4) Consultar por CID\n"
        "5) Consultar por patógeno\n"
        "6) Voltar\n"
        DASHED_LINE
    );

    do {
        scanf_and_clear_stdin("%d", &op, "Digite o comando");
    } while (op < 1 || op > 6);

    return op;
}

// This function only works when 'op' is >=2 or <=5
static const char* get_where_clause(MYSQL* connection, const int op) {
    static const char* DOENCA_COLUMN[] = {
        "d.nome",  // nome técnico
        "np.nome", // nome popular
        "d.cid",   // CID
        "pt.nome"  // patógeno
    };

    static char clause[256];
    char to_find[64];

    strcpy(clause, "WHERE ");
    strcat(clause, DOENCA_COLUMN[op - 2]);
    strcat(clause, " = ");

    printf(
        "Digite o %s da doença: ",
        op == 2 ? "nome técnico" :
        op == 3 ? "nome popular" :
        op == 4 ? "CID" :
        op == 5 ? "tipo de patógeno causador" :
        ""
    );
    read_line(connection, to_find);

    strcat(clause, "\'");
    strcat(clause, to_find);
    strcat(clause, "\'");

    return clause;
}

static MYSQL_RES* get_doenca_result(MYSQL* connection, const char* where) {
    char query[1024];

    snprintf(query, sizeof(query),
        "SELECT d.id, d.nome, d.cid, p.nome AS pat_nome, pt.nome AS pat_tipo, "
        "    GROUP_CONCAT(DISTINCT np.nome ORDER BY np.nome SEPARATOR ', ') AS nomes_pop, "
        "    GROUP_CONCAT(DISTINCT CONCAT(s.nome, ' (', ds.ocorrencia, ')') "
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
        "ORDER BY d.nome",
        where != NULL ? where : ""
    );

    return mariadb_get_result(connection, query);
}

void list_doencas(MYSQL* connection) {
    MYSQL_RES* result;
    MYSQL_ROW row;
    int op;
    const char* where_clause = NULL;

    op = show_menu_list_doencas();

    if (op == 6) {
        printf("Voltando...\n");
        return;
    }

    where_clause = op == 1 ? NULL : get_where_clause(connection, op);
    result = get_doenca_result(connection, where_clause);

    printf(DASHED_LINE);
    while ((row = mysql_fetch_row(result)) != NULL) {
        printf("ID: %s\n", row[0]);
        printf("Nome: %s\n", row[1]);
        printf("CID: %s\n", row[2]);
        printf("Nome do patógeno: %s\n", row[3]);
        printf("Tipo de patógeno: %s\n", row[4]);
        printf("Nomes populares: %s\n", row[5]);
        printf("Sintomas: %s\n", row[6]);
        printf(DASHED_LINE);
    }
    printf("Fim da lista!\n");

    write_log(LOG_CONSULTA);
    mysql_free_result(result);
}

// -----------------------------------------------------------------------------
// LISTAR DOENÇAS

static const char* get_symptoms(MYSQL* connection) {
    int count = 0;
    static char symptoms[1024];
    char tmp[64] = "";
    char op;

    strcpy(symptoms, "");

    do {
        printf("Digite o nome do sintoma: ");
        read_line(connection, tmp);

        if (count > 0) {
            strcat(symptoms, ", ");
        }

        strcat(symptoms, "\'");
        strcat(symptoms, tmp);
        strcat(symptoms, "\'");

        do {
            scanf_and_clear_stdin("%c", &op,
                "Inserir mais um sintoma (s ou n)"
            );
            op = tolower(op);
        } while (op != 's' && op != 'n');

        count++;

    } while (op != 'n');

    return symptoms;
}

static MYSQL_RES* get_symptoms_result(MYSQL* connection, const char* symptoms) {
    char query[1024];

    snprintf(query, sizeof(query),
        "SELECT d.nome, d.cid, "
        "    COALESCE(SUM( "
        "        CASE "
        "            WHEN s.nome IN (%s) THEN "
        "                CASE ds.ocorrencia "
        "                    WHEN 'muito comum' THEN 5 "
        "                    WHEN 'comum' THEN 4 "
        "                    WHEN 'pouco comum' THEN 3 "
        "                    WHEN 'raro' THEN 2 "
        "                    WHEN 'muito raro' THEN 1 "
        "                END "
        "            ELSE 0 "
        "        END "
        "    ), 0) - ( "
        "        SELECT COUNT(*) "
        "        FROM sintomas s2 "
        "        WHERE s2.nome IN (%s) "
        "        AND s2.id NOT IN ( "
        "            SELECT ds.id_sintoma "
        "            FROM doenca_sintoma ds "
        "            WHERE ds.id_doenca = d.id "
        "        ) "
        "    ) AS pontuacao_final "
        "FROM doencas d "
        "LEFT JOIN doenca_sintoma ds ON d.id = ds.id_doenca "
        "LEFT JOIN sintomas s ON ds.id_sintoma = s.id "
        "GROUP BY d.id, d.nome "
        "ORDER BY pontuacao_final DESC",
        symptoms, symptoms
    );

    return mariadb_get_result(connection, query);
}

void diagnostic(MYSQL* connection) {
    MYSQL_RES* result;
    MYSQL_ROW row;
    const char* symptoms;

    symptoms = get_symptoms(connection);
    result = get_symptoms_result(connection, symptoms);

    printf(DASHED_LINE);
    while ((row = mysql_fetch_row(result)) != NULL) {
        printf("Nome: %s\n", row[0]);
        printf("CID: %s\n", row[1]);
        printf("Pontuação final: %s\n", row[2]);
        printf(DASHED_LINE);
    }
    printf("Fim da lista!\n");

    write_log(LOG_RELATORIO);
    mysql_free_result(result);
}

// -----------------------------------------------------------------------------
// FAZER RELATÓRIO

static int show_menu_make_report() {
    int op;

    printf(
        "1) Criar relatório para doença\n"
        "2) Criar relatório para diagnóstico\n"
        "   de doenças mais prováveis\n"
        "3) Voltar\n"
        DASHED_LINE
    );

    do {
        scanf_and_clear_stdin("%d", &op, "Digite o comando");
    } while (op < 1 || op > 3);

    return op;
}

static void doenca_report(MYSQL* connection) {
    MYSQL_RES* result;
    MYSQL_ROW doenca;
    char where_clause[256] = "WHERE d.cid = \'";
    char cid[64];

    printf("Digite o CID doença: ");
    read_line(connection, cid);
    strcat(where_clause, cid);
    strcat(where_clause, "\'");

    result = get_doenca_result(connection, where_clause);
    doenca = mysql_fetch_row(result);

    if (doenca != NULL) {
        create_doenca_report(&doenca);
        printf("Relatório criado!\n" );
        write_log(LOG_RELATORIO);
    } else {
        printf(COLOR_YELLOW "Doença não encontrada\n" COLOR_RESET);
    }

    mysql_free_result(result);
}

static void diagnostic_report(MYSQL* connection) {
    MYSQL_RES* result;
    const char* symptoms;

    symptoms = get_symptoms(connection);
    result = get_symptoms_result(connection, symptoms);
    create_diagnostic_report(result);

    printf("Relatório criado!\n" );
    write_log(LOG_RELATORIO);
    mysql_free_result(result);
}

void make_report(MYSQL* connection) {
    int op;

    op = show_menu_make_report();

    switch (op) {
        case 1: doenca_report(connection);     break;
        case 2: diagnostic_report(connection); break;
        case 3: printf("Saindo...\n");         break;
    }
}

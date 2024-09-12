#include "data_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysql.h"
#include "util.h"

static void insert_patogeno_tipo(MYSQL* connection) {
    char query[1024];
    char patogeno_tipo_nome[32];

    printf("Digite o nome do tipo de patógeno: ");
    read_line(connection, patogeno_tipo_nome);

    snprintf(query, sizeof(query),
        "INSERT INTO patogeno_tipo (nome) VALUES (\'%s\')",
        patogeno_tipo_nome
    );

    if (mysql_query(connection, query) != 0) {
        mariadb_error_handler(connection);
    } else {
        write_log(LOG_CADASTRO);
    }
}

static void insert_patogeno(MYSQL* connection) {
    char query[512];
    struct { char nome[32]; int id_tipo; } patogeno;

    printf("Digite o nome patógeno: ");
    read_line(connection, patogeno.nome);

    scanf_and_clear_stdin("%d", &patogeno.id_tipo,
        "Digite o ID do tipo de patógeno a ser associado"
    );

    snprintf(query, sizeof(query),
        "INSERT INTO patogenos (nome, id_tipo) VALUES (\'%s\', %d)",
        patogeno.nome, patogeno.id_tipo
    );

    if (mysql_query(connection, query) != 0) {
        mariadb_error_handler(connection);
    } else {
        write_log(LOG_CADASTRO);
    }
}

static void insert_doenca(MYSQL* connection) {
    char query[512];
    struct { char nome[32], cid[8]; int id_patogeno; } doenca;

    printf("Digite o nome técnico da doença: ");
    read_line(connection, doenca.nome);

    printf("Digite o CID da doença: ");
    read_line(connection, doenca.cid);

    scanf_and_clear_stdin("%d", &doenca.id_patogeno,
        "Digite o ID do patógeno causador da doença"
    );

    snprintf(query, sizeof(query),
        "INSERT INTO doencas (nome, cid, id_patogeno) VALUES"
        "    (\'%s\', \'%s\', \'%d\')",
        doenca.nome, doenca.cid, doenca.id_patogeno
    );

    if (mysql_query(connection, query) != 0) {
        mariadb_error_handler(connection);
    } else {
        write_log(LOG_CADASTRO);
    }
}

static void insert_sintoma(MYSQL* connection) {
    char query[512];
    char sintoma_nome[32];

    printf("Digite o nome do sintoma: ");
    read_line(connection, sintoma_nome);

    snprintf(query, sizeof(query),
        "INSERT INTO sintomas (nome) VALUES (\'%s\')", sintoma_nome
    );

    if (mysql_query(connection, query) != 0) {
        mariadb_error_handler(connection);
    } else {
        write_log(LOG_CADASTRO);
    }
}

static void insert_doenca_sintoma(MYSQL* connection) {
    static const char* OCORRENCIA_STR[] = {
        "muito raro", "raro", "pouco comum", "comum", "muito comum"
    };

    char query[512];
    struct { int id_doenca, id_sintoma, ocorr; } doenca_sintoma;

    scanf_and_clear_stdin("%d", &doenca_sintoma.id_doenca,
        "Digite o ID da doença"
    );

    scanf_and_clear_stdin("%d", &doenca_sintoma.id_sintoma,
        "Digite o ID do sintoma"
    );

    for (int i = 0; i < 5; i++) {
        printf("%d | %s\n", i, OCORRENCIA_STR[i]);
    }
    do {
        scanf_and_clear_stdin("%d", &doenca_sintoma.ocorr,
            "Digite o valor de ocorrência (0 a 4): "
        );
    } while (doenca_sintoma.ocorr < 0 || doenca_sintoma.ocorr > 4);

    snprintf(query, sizeof(query),
        "INSERT INTO doenca_sintoma VALUES (%d, %d, \'%s\')",
        doenca_sintoma.id_doenca, doenca_sintoma.id_sintoma,
        OCORRENCIA_STR[doenca_sintoma.ocorr]
    );

    if (mysql_query(connection, query) != 0) {
        mariadb_error_handler(connection);
    } else {
        write_log(LOG_CADASTRO);
    }
}

static void insert_nomes_pop(MYSQL* connection) {
    char query[512];
    struct { int id_doenca; char nome[32]; } nomes_pop;

    scanf_and_clear_stdin("%d", &nomes_pop.id_doenca,
        "Digite o ID da doença"
    );

    printf("Digite o nome popular da doença: ");
    read_line(connection, nomes_pop.nome);

    snprintf(query, sizeof(query),
        "INSERT INTO nomes_pop (id_doenca, nome) VALUES (%d, \'%s\')",
        nomes_pop.id_doenca, nomes_pop.nome
    );

    if (mysql_query(connection, query) != 0) {
        mariadb_error_handler(connection);
    } else {
        write_log(LOG_CADASTRO);
    }
}

void insertion_menu(MYSQL* connection) {
    int op;

    printf(
        "1) Inserir novo tipo de patógeno\n"
        "2) Inserir patógeno\n"
        "3) Inserir doença\n"
        "4) Inserir sintoma\n"
        "5) Associar sintoma a uma doença\n"
        "6) Associar nome popular a uma doença\n"
        "7) Voltar\n"
        DASHED_LINE
        "Digite o comando: "
    );
    if (scanf("%d", &op) != 1) {
        fprintf(stderr, "Erro ao ler o comando, digite novamente: ");
        clear_stdin();
    }
    clear_stdin();
    printf(DASHED_LINE);

    switch (op) {
        case 1:  insert_patogeno_tipo(connection);   break;
        case 2:  insert_patogeno(connection);        break;
        case 3:  insert_doenca(connection);          break;
        case 4:  insert_sintoma(connection);         break;
        case 5:  insert_doenca_sintoma(connection);  break;
        case 6:  insert_nomes_pop(connection);       break;
        case 7:  printf("Voltando...\n");            break;
        default: printf("Comando não definido\n");   break;
    }
}

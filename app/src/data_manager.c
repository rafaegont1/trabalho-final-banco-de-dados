#include "data_manager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysql.h"
#include "util.h"

static const char* OCORRENCIA_STR[] = {
    "muito raro",
    "raro",
    "pouco comum",
    "comum",
    "muito comum"
};

static void insert_patogeno_tipo(MYSQL* connection) {
    char query[512];
    struct { char nome[32], esc_nome[32]; } pt;

    printf("Digite o nome do tipo de patógeno: ");
    fgets(pt.nome, sizeof(pt.nome), stdin);
    pt.nome[strcspn(pt.nome, "\n")] = '\0';
    mysql_real_escape_string(connection, pt.esc_nome, pt.nome, strlen(pt.nome));

    snprintf(query, sizeof(query),
        "INSERT INTO patogeno_tipo (nome) VALUES (%s)",
        pt.esc_nome
    );

    if (mysql_query(connection, query) != 0) {
        mariadb_error(connection);
        return;
    }

    write_log(LOG_CADASTRO);
}

static void insert_patogeno(MYSQL* connection) {
    char query[512];
    struct { char nome[32], esc_nome[32]; int id_tipo; } p;

    printf("Digite o nome patógeno: ");
    fgets(p.nome, sizeof(p.nome), stdin);
    p.nome[strcspn(p.nome, "\n")] = '\0';
    mysql_real_escape_string(connection, p.esc_nome, p.nome, strlen(p.nome));

    do {
        printf("Digite o ID do tipo de patógeno: ");
    } while (scanf("%d", &p.id_tipo) != 1);

    snprintf(query, sizeof(query),
        "INSERT INTO patogenos (nome, id_tipo) VALUES (%s, %d)",
        p.esc_nome, p.id_tipo
    );

    if (mysql_query(connection, query) != 0) {
        mariadb_error(connection);
        return;
    }

    write_log(LOG_CADASTRO);
}

static void insert_doenca(MYSQL* connection) {
    char query[512];
    struct {
        char nome[32], esc_nome[32], cid[8], esc_cid[8];
        int id_patogeno;
    } d;

    printf("Digite o nome técnico da doença: ");
    fgets(d.nome, sizeof(d.nome), stdin);
    d.nome[strcspn(d.nome, "\n")] = '\0';
    mysql_real_escape_string(connection, d.esc_nome, d.nome, strlen(d.nome));

    printf("Digite o CID da doença: ");
    fgets(d.cid, sizeof(d.cid), stdin);
    d.cid[strcspn(d.cid, "\n")] = '\0';
    mysql_real_escape_string(connection, d.esc_cid, d.cid, strlen(d.cid));

    do {
        printf("Digite o ID do patógeno causador da doença: ");
    } while (scanf("%d", &d.id_patogeno) != 1);

    snprintf(query, sizeof(query),
        "INSERT INTO doencas (nome, cid, id_patogeno) VALUES"
        "    (\'%s\', \'%s\', \'%d\')",
        d.esc_nome, d.esc_cid, d.id_patogeno
    );

    if (mysql_query(connection, query) != 0) {
        mariadb_error(connection);
        return;
    }

    write_log(LOG_CADASTRO);
}

static void insert_sintoma(MYSQL* connection) {
    char query[512];
    struct { char nome[32], esc_nome[32]; } s;

    printf("Digite o nome do sintoma: ");
    fgets(s.nome, sizeof(s.nome), stdin);
    s.nome[strcspn(s.nome, "\n")] = '\0';
    mysql_real_escape_string(connection, s.esc_nome, s.nome, strlen(s.nome));

    snprintf(query, sizeof(query),
        "INSERT INTO sintomas (nome) VALUES (\'%s\')",
        s.esc_nome
    );

    if (mysql_query(connection, query) != 0) {
        mariadb_error(connection);
        return;
    }

    write_log(LOG_CADASTRO);
}

static void insert_doenca_sintomas(MYSQL* connection) {
    char query[512];
    struct { int id_doenca, id_sintoma, ocorr; } ds;

    do {
        printf("Digite o ID da doença: ");
    } while (scanf("%d", &ds.id_doenca) != 1);

    do {
        printf("Digite o ID do sintoma: ");
    } while (scanf("%d", &ds.id_sintoma) != 1);

    do {
        printf("Digite o valor de ocorrência (0 a 4): ");
    } while ((scanf("%d", &ds.ocorr) != 1) ||
        (ds.ocorr < 0 || ds.ocorr > 4));

    snprintf(query, sizeof(query),
        "INSERT INTO doenca_sintoma VALUES (%d, %d, \'%s\')",
        ds.id_doenca, ds.id_sintoma, OCORRENCIA_STR[ds.ocorr]
    );

    if (mysql_query(connection, query) != 0) {
        mariadb_error(connection);
        return;
    }

    write_log(LOG_CADASTRO);
}

static void insert_nomes_pop(MYSQL* connection) {
    char query[512];
    struct { int id_doenca; char nome[32], esc_nome[32]; } np;

    do {
        printf("Digite o ID da doença: ");
    } while (scanf("%d", &np.id_doenca) != 1);

    printf("Digite o nome popular da doença: ");
    fgets(np.nome, sizeof(np.nome), stdin);
    np.nome[strcspn(np.nome, "\n")] = '\0';
    mysql_real_escape_string(connection, np.esc_nome, np.nome, strlen(np.nome));

    snprintf(query, sizeof(query),
        "INSERT INTO doenca_sintoma VALUES (%d, \'%s\')",
        np.id_doenca, np.esc_nome
    );

    if (mysql_query(connection, query) != 0) {
        mariadb_error(connection);
        return;
    }

    write_log(LOG_CADASTRO);
}

void insertion_menu(MYSQL* connection) {
    static const char* MENU =
        "1) Inserir novo tipo de patógeno\n"
        "2) Inserir patógeno\n"
        "3) Inserir doença\n"
        "4) Inserir sintoma\n"
        "5) Associar sintoma a uma doença\n"
        "6) Associar nome popular a uma doença\n"
        "7) Voltar\n";
    int op;

    printf("%s", MENU);
    if (scanf("%d", &op) != 1) {
        fprintf(stderr, "Erro ao ler o comando, digite novamente: ");
        clear_buffer();
    }
    clear_buffer();
    printf("----------------------------------------\n");

    switch (op) {
        case 1:  insert_patogeno_tipo(connection);   break;
        case 2:  insert_patogeno(connection);        break;
        case 3:  insert_doenca(connection);          break;
        case 4:  insert_sintoma(connection);         break;
        case 5:  insert_doenca_sintomas(connection); break;
        case 6:  insert_nomes_pop(connection);       break;
        case 7:  printf("Voltando...\n");            break;
        default: printf("Comando não definido\n");   break;
    }
}

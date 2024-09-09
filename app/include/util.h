#ifndef UTIL_H
#define UTIL_H

#include <mariadb/mysql.h>

// enum ocorrencia {
//     MUITO_RARO  = 1,
//     RARO        = 2,
//     POUCO_COMUM = 3,
//     COMUM       = 4,
//     MUITO_COMUM = 5
// };

// typedef struct {
//     int id;
//     char nome[32];
// } patogeno_tipo_t;

// typedef struct {
//     int id;
//     char nome[32];
//     int id_tipo;
// } patogenos_t;

typedef struct {
    char nome[32];
    char esc_nome[32];
    char cid[8];
    char esc_cid[8];
    int id_patogeno;
} doencas_t;

// typedef struct {
//     int id;
//     char nome[32];
// } sintomas_t;

// typedef struct {
//     int id_doenca;
//     int id_sintoma;
//     enum ocorrencia ocorrencia;
// } doenca_sintoma_t;

// typedef struct {
//     int id;
//     int id_doenca;
//     char nome[32];
// } nomes_pop_t;

void clear_buffer();
void exit_error(MYSQL* connection);
MYSQL* init();
void write_log(const char* query);

#endif // UTIL_H

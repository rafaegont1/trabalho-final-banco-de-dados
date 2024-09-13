#include "report.h"

#include <string.h>
#include <mariadb/mysql.h>
#include "pdfgen.h"

// -----------------------------------------------------------------------------
// RELATÓRIO SOBRE DOENÇA

void create_doenca_report(MYSQL_ROW* doenca) {
    struct pdf_doc* pdf = pdf_create(PDF_A4_WIDTH, PDF_A4_HEIGHT, NULL);
    char nome[256];
    char cid[256];
    char nome_patogeno[256];
    char tipo_patogeno[256];
    char nomes_pop[1024];
    char sintomas[1024];

    snprintf(nome, sizeof(nome), "Nome: %s", (*doenca)[1]);
    snprintf(cid, sizeof(cid), "CID: %s", (*doenca)[2]);
    snprintf(nome_patogeno, sizeof(nome_patogeno), "Patógeno causador: %s",
        (*doenca)[3]);
    snprintf(tipo_patogeno, sizeof(tipo_patogeno), "Tipo de patógeno: %s",
        (*doenca)[4]);
    snprintf(nomes_pop, sizeof(nomes_pop), "Nomes populares: %s", (*doenca)[5]);
    snprintf(sintomas, sizeof(nomes_pop), "Sintomas: %s", (*doenca)[6]);

    pdf_set_font(pdf, "Times-Roman");

    pdf_append_page(pdf);

    pdf_add_text(pdf, NULL, "Relatório - Doença", 32.0f, 165.0f, 775.0f,
        PDF_BLACK);
    pdf_add_text(pdf, NULL, nome, 12.0f, 50.0f, 700.0f, PDF_BLACK);
    pdf_add_text(pdf, NULL, cid, 12.0f, 50.0f, 675.0f, PDF_BLACK);
    pdf_add_text(pdf, NULL, nome_patogeno, 12.0f, 50.0f, 650.0f, PDF_BLACK);
    pdf_add_text(pdf, NULL, tipo_patogeno, 12.0f, 50.0f, 625.0f, PDF_BLACK);
    pdf_add_text(pdf, NULL, nomes_pop, 12.0f, 50.0f, 600.0f, PDF_BLACK);
    pdf_add_text(pdf, NULL, sintomas, 12.0f, 50.0f, 575.0f, PDF_BLACK);

    pdf_save(pdf, "Relatório-doença.pdf");
    pdf_destroy(pdf);
}

// -----------------------------------------------------------------------------
// RELATÓRIO SOBRE DIAGNÓSTICO

static void write_disease_line(struct pdf_doc* pdf, MYSQL_ROW* doenca,
        const float offset) {
    char nome[256];
    char cid[256];
    char pontuacao[256];

    snprintf(nome, sizeof(nome), "Nome: %s", (*doenca)[0]);
    snprintf(cid, sizeof(cid), "CID: %s", (*doenca)[1]);
    snprintf(pontuacao, sizeof(pontuacao), "Pontuação final: %s", (*doenca)[2]);

    pdf_add_text(pdf, NULL, nome, 12.0f, 50.0f, 700.0f - offset, PDF_BLACK);
    pdf_add_text(pdf, NULL, cid, 12.0f, 50.0f, 675.0f - offset, PDF_BLACK);
    pdf_add_text(pdf, NULL, pontuacao, 12.0f, 50.0f, 650.0f - offset,
        PDF_BLACK);

    pdf_add_line(pdf, NULL, 50.0f, 625.0f - offset, 500.0f, 625.0f - offset,
        1.0f, PDF_BLACK);
}

void create_diagnostic_report(MYSQL_RES* result) {
    struct pdf_doc* pdf = pdf_create(PDF_A4_WIDTH, PDF_A4_HEIGHT, NULL);
    MYSQL_ROW row;

    pdf_set_font(pdf, "Times-Roman");

    while ((row = mysql_fetch_row(result)) != NULL) {
        pdf_append_page(pdf);
        pdf_add_text(pdf, NULL, "Relatório - Diagnóstico", 32.0f, 145.0f,
            775.0f, PDF_BLACK);
        for (int offset = 0; offset <= 500; offset += 100) {
            write_disease_line(pdf, &row, (float)offset);
            if ((row = mysql_fetch_row(result)) == NULL) {
                break;
            }
        }
    }

    pdf_save(pdf, "Relatório-diagnóstico.pdf");
    pdf_destroy(pdf);
}

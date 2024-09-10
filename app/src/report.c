#include <stdio.h>
#include <stdlib.h>
#include "hpdf.h"

#define PAGE_WIDTH  595  // Largura da página em pontos (A4)
#define PAGE_HEIGHT 842  // Altura da página em pontos (A4)

// Função para desenhar um título
void draw_title(HPDF_Doc pdf, HPDF_Page page) {
    const char *title_text = "Título do Documento";
    
    // Define a fonte e o tamanho para o título
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica-Bold", "UTF-8");
    HPDF_Page_SetFontAndSize(page, font, 18);

    // Calcula a posição do título
    float text_width = HPDF_Page_TextWidth(page, title_text);
    float x = (PAGE_WIDTH - text_width) / 2;  // Centraliza horizontalmente
    float y = PAGE_HEIGHT - 50;  // Margem superior para o título

    // Adiciona o texto do título
    HPDF_Page_BeginText(page);
    HPDF_Page_MoveTextPos(page, x, y);
    HPDF_Page_ShowText(page, title_text);
    HPDF_Page_EndText(page);
}

// Função para desenhar uma tabela no PDF
void draw_table(HPDF_Doc pdf, HPDF_Page page) {
    float x, y;
    float cell_width = 100;
    float cell_height = 20;
    int num_columns = 4;
    int num_rows = 5;

    // Calcula a largura e altura da tabela
    float table_width = num_columns * cell_width;
    float table_height = num_rows * cell_height;

    // Calcula a posição inicial da tabela para centralizá-la
    x = (PAGE_WIDTH - table_width) / 2;
    y = PAGE_HEIGHT - 100; // Margem superior da página após o título

    // Define a fonte e o tamanho
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", "UTF-8");
    HPDF_Page_SetFontAndSize(page, font, 12);

    // Desenha as linhas da tabela
    for (int i = 0; i <= num_rows; ++i) {
        HPDF_Page_MoveTo(page, x, y - i * cell_height);
        HPDF_Page_LineTo(page, x + table_width, y - i * cell_height);
        HPDF_Page_Stroke(page);
    }

    for (int i = 0; i <= num_columns; ++i) {
        HPDF_Page_MoveTo(page, x + i * cell_width, y);
        HPDF_Page_LineTo(page, x + i * cell_width, y - table_height);
        HPDF_Page_Stroke(page);
    }

    // Preenche as células da tabela
    for (int row = 0; row < num_rows; ++row) {
        for (int col = 0; col < num_columns; ++col) {
            char cell_text[100];
            snprintf(cell_text, sizeof(cell_text), "Cell %d,%d", row + 1, col + 1);
            HPDF_Page_BeginText(page);
            HPDF_Page_MoveTextPos(page, x + col * cell_width + 5, y - row * cell_height - 15);
            HPDF_Page_ShowText(page, cell_text);
            HPDF_Page_EndText(page);
        }
    }
}

int main() {
    HPDF_Doc pdf;
    HPDF_Page page;

    // Cria um novo documento PDF
    pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        fprintf(stderr, "Failed to create PDF object\n");
        return 1;
    }

    HPDF_SetCurrentEncoder(pdf, "UTF-8");

    // Adiciona uma página ao documento
    page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    // Desenha o título na página
    draw_title(pdf, page);

    // Desenha a tabela na página
    draw_table(pdf, page);

    // Salva o documento em um arquivo
    const char *filename = "tabela.pdf";
    HPDF_SaveToFile(pdf, filename);

    // Libera recursos
    HPDF_Free(pdf);

    printf("PDF criado com sucesso: %s\n", filename);
    return 0;
}

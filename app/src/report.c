#include "pdfgen.h"

int create_report(void) {
    char* str[] = { "rafael guimarães", "neuza", "fernando", NULL };
    int i = 0;
    struct pdf_doc* pdf = pdf_create(PDF_A4_WIDTH, PDF_A4_HEIGHT, NULL);

    pdf_set_font(pdf, "Times-Roman");

    while (str[i] != NULL) {
        pdf_append_page(pdf);
        pdf_add_text(pdf, NULL, "Relatório", 22.0f, 50.0f, 775.0f, PDF_BLACK);
        pdf_add_text(pdf, NULL, str[i], 12.0f, 50.0f, 500.0f, PDF_BLACK);
        // pdf_add_line(pdf, NULL, 50, 24, 150, 24, 3, PDF_BLACK);
        i++;
    }

    pdf_save(pdf, "output.pdf");
    pdf_destroy(pdf);

    return 0;
}

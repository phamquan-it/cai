#ifndef CHEMISTRY_H
#define CHEMISTRY_H

// Công thức hóa học
typedef struct {
    char name[64];
    char formula[64];
    char description[256];
    char example[256];
} ChemistryFormula;

// Hàm phân tích câu hỏi hóa học
int is_chemistry_question(const char *input);
char* answer_chemistry_question(const char *input);

// Hàm tra cứu
char* get_chemistry_info(const char *keyword);
void load_chemistry_knowledge();

#endif

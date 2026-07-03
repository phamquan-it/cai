#ifndef PHYSICS_H
#define PHYSICS_H

// Công thức vật lý
typedef struct {
    char name[64];
    char formula[128];
    char description[256];
    char example[256];
} PhysicsFormula;

// Hàm phân tích câu hỏi vật lý
int is_physics_question(const char *input);
char* answer_physics_question(const char *input);

// Hàm tra cứu công thức
char* get_formula(const char *keyword);
void load_physics_formulas();

// THÊM: Hàm thêm công thức
void add_physics_formula(const char *name, const char *formula, 
                         const char *desc, const char *example);

#endif

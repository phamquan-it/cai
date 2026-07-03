#ifndef ATTRIBUTE_CONSTRAINT_H
#define ATTRIBUTE_CONSTRAINT_H

// Cấu trúc 1 thực thể với thuộc tính
typedef struct {
    char entity[64];           // "bò", "xe", "nhà"
    char attributes[5][32];     // ["sữa", "đen", "to"]
    int attr_count;
} EntityWithAttributes;

// Cấu trúc ràng buộc
typedef struct {
    EntityWithAttributes subjects[10];
    int subject_count;
    char action[64];
    char object[64];
    char condition[256];
} ConstraintContext;

// Hàm chính
ConstraintContext* extract_constraints(const char *input);
void free_constraint_context(ConstraintContext *cc);

// Hàm kiểm tra ràng buộc
int check_constraints(ConstraintContext *cc, const char *entity);
char* get_entity_with_attributes(ConstraintContext *cc, const char *entity);

// Hàm tích hợp
char* reasoning_with_constraints(const char *input);

#endif

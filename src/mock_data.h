#ifndef MOCK_DATA_H
#define MOCK_DATA_H

// Cấu trúc lưu mock data
typedef struct {
    char name[64];
    char language[32];
    char template_code[4096];
    char placeholder_positions[10][64];  // Vị trí cần điền code
    int placeholder_count;
} MockFunction;

// Hàm quản lý mock data
void load_mock_data();
char* get_mock_code(const char *func_name, const char *language);
char* get_placeholder(const char *func_name, int index);

// Hàm chính
int is_mock_request(const char *input);
char* answer_mock_request(const char *input);

#endif

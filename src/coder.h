#ifndef CODER_H
#define CODER_H

// Hàm phát hiện yêu cầu code
int is_coding_request(const char *input);
char* answer_coding_request(const char *input);

// Hàm sinh code mẫu
char* generate_code(const char *language, const char *task);

#endif

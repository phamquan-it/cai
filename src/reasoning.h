#ifndef REASONING_H
#define REASONING_H

// Hàm nhận diện câu hỏi
int is_reasoning_request(const char *input);

// Hàm trả lời câu hỏi đơn giản
char* answer_reasoning_request(const char *input);

// Hàm load (rỗng)
void load_formulas();

#endif

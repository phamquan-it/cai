#ifndef RULE_LEARNER_H
#define RULE_LEARNER_H

#include "reasoning_nl.h"

// Hàm học rule từ câu nói
int learn_rule_from_sentence(const char *sentence);
char* get_learned_rules_summary();

// Hàm lưu rule vào file
int save_learned_rules(const char *filename);
int load_learned_rules(const char *filename);

// Hàm kiểm tra
int is_learn_request(const char *input);
char* answer_learn_request(const char *input);

#endif

#include <stdlib.h>
#include "brain.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void brain_init(Brain *b, const char *name) {
    markov_init(&b->dict);
    strcpy(b->name, name);
    b->conversation_count = 0;
    srand(time(NULL));
}

void brain_free(Brain *b) {
    markov_free(&b->dict);
}

void brain_train(Brain *b, const char *data_file) {
    markov_load_file(&b->dict, data_file);
    if (b->dict.count == 0) {
        printf("📚 Using default knowledge...\n");
        const char *default_data[] = {
            "hello i am an artificial intelligence",
            "my name is cai and i am written in c",
            "machine learning is fun and powerful",
            "c programming language is fast and efficient",
            "neural networks are inspired by the human brain",
            "i can learn from conversations",
            "artificial intelligence can solve many problems",
            "programming requires patience and practice",
            "i enjoy learning new things every day",
            "chatgpt is a large language model"
        };
        for (int i = 0; i < 10; i++) markov_learn(&b->dict, default_data[i]);
        printf("✅ Loaded %d default knowledge items\n", 10);
    }
}

void brain_learn(Brain *b, const char *sentence) {
    if (strlen(sentence) > 10) markov_learn(&b->dict, sentence);
}

char* brain_chat(Brain *b, const char *input) {
    b->conversation_count++;
    char first_word[MAX_WORD_LEN];
    char temp[1000];
    strcpy(temp, input);
    char *token = strtok(temp, " .,!?;:\n\t");
    if (!token) return "Sorry, I didn't understand that!";
    strcpy(first_word, token);
    return markov_generate(&b->dict, first_word);
}

void brain_stats(Brain *b) {
    printf("\n╔══════════════════════════════════╗\n");
    printf("║        🧠  BRAIN STATS           ║\n");
    printf("╠══════════════════════════════════╣\n");
    printf("║ Name:      %-20s ║\n", b->name);
    printf("║ Vocabulary: %-20d ║\n", b->dict.count);
    printf("║ Chats:     %-20d ║\n", b->conversation_count);
    printf("╚══════════════════════════════════╝\n");
}

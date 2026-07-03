#include <stdio.h>
#include <string.h>
#include "brain.h"
#include "utils.h"

void print_banner() {
    printf("\n");
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║   🧠  C-AI v1.0.0                          ║\n");
    printf("║   Cognitive Artificial Intelligence         ║\n");
    printf("║   Written in Pure C                        ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("\n");
}

void print_help() {
    printf("Commands:\n");
    printf("  /help     Show this help\n");
    printf("  /stats    Show brain statistics\n");
    printf("  /clear    Clear screen\n");
    printf("  /exit     Quit\n");
    printf("\n");
}

int main() {
    print_banner();
    Brain brain;
    brain_init(&brain, "CAI");
    printf("📚 Training...\n");
    brain_train(&brain, "data/knowledge.txt");
    printf("✅ Ready! Type /help for commands.\n\n");
    char input[1000];
    while (1) {
        printf("You: ");
        fgets(input, sizeof(input), stdin);
        trim(input);
        if (strcmp(input, "/exit") == 0 || strcmp(input, "/quit") == 0) {
            printf("🤖 CAI: Goodbye! See you next time.\n");
            break;
        }
        else if (strcmp(input, "/help") == 0) {
            print_help();
            continue;
        }
        else if (strcmp(input, "/stats") == 0) {
            brain_stats(&brain);
            continue;
        }
        else if (strcmp(input, "/clear") == 0) {
            clear_screen();
            print_banner();
            continue;
        }
        if (strlen(input) == 0) continue;
        brain_learn(&brain, input);
        char *response = brain_chat(&brain, input);
        printf("🤖 CAI: %s\n\n", response);
    }
    brain_free(&brain);
    return 0;
}

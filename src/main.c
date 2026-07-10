#include <stdio.h>
#include <string.h>
#include "brain.h"
#include "utils.h"
#include "config.h"
#include "debug_logger.h"

void print_banner() {
    printf("\n");
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║   🧠  CAI - Cơ Bản                         ║\n");
    printf("║   Chatbot đơn giản với Markov Chain        ║\n");
    printf("║   Không AI, không GPU, chỉ C thuần!        ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("\n");
}

void print_help() {
    printf("╔══════════════════════════════════════════╗\n");
    printf("║  📜  DANH SÁCH LỆNH                      ║\n");
    printf("╠══════════════════════════════════════════╣\n");
    printf("║  /help      Hiển thị trợ giúp            ║\n");
    printf("║  /stats     Xem thống kê cơ bản          ║\n");
    printf("║  /config    Xem cấu hình hiện tại        ║\n");
    printf("║  /clear     Xóa màn hình                ║\n");
    printf("║  /exit      Thoát                       ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    printf("\n");
}

int main() {
    // Khởi tạo cấu hình
    Config *cfg = config_init();
    
    // Load từ file .env
    config_load_from_file(cfg, ".env");
    
    // Bật debug nếu cấu hình
    debug_set_enabled(cfg->debug_enabled);
    debug_init();
    
    print_banner();
    
    DEBUG_LOG("MAIN", "Start CAI");
    DEBUG_LOG_FMT("MAIN", "Data file: %s", cfg->data_file);
    
    Brain brain;
    brain_init(&brain, "CAI");
    
    printf("📚 Đang huấn luyện...\n");
    brain_train(&brain, cfg->data_file);
    printf("✅ Sẵn sàng! Gõ /help để xem lệnh.\n\n");
    
    if (cfg->debug_enabled) {
        printf("🐛 Debug mode: BẬT\n");
        printf("   Gõ /config để xem cấu hình chi tiết.\n\n");
    }
    
    char input[1000];
    while (1) {
        printf("🧑 Bạn: ");
        fgets(input, sizeof(input), stdin);
        trim(input);
        
        if (strcmp(input, "/exit") == 0 || strcmp(input, "/quit") == 0) {
            printf("🤖 CAI: Tạm biệt!\n");
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
        else if (strcmp(input, "/config") == 0) {
            config_print(cfg);
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
    config_free(cfg);
    return 0;
}

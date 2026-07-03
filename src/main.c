#include <stdio.h>
#include <string.h>
#include "brain.h"
#include "utils.h"

void print_banner() {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║   🧠  CAI - Cognitive Artificial Intelligence                ║\n");
    printf("║   🧠  BRIDGE - TỔNG HỢP SUY NGHĨ                           ║\n");
    printf("║   🇻🇳 TIẾNG VIỆT | 🔬 VẬT LÝ | 🧪 HÓA HỌC | 💻 CODE       ║\n");
    printf("║   \"Tôi suy nghĩ trước khi trả lời - Có thể lâu hơn 1ms\"   ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

void print_help() {
    printf("╔══════════════════════════════════════════╗\n");
    printf("║  📜  DANH SÁCH LỆNH                      ║\n");
    printf("╠══════════════════════════════════════════╣\n");
    printf("║  /help      Hiển thị trợ giúp            ║\n");
    printf("║  /stats     Xem thống kê bá đạo         ║\n");
    printf("║  /context   Xem ngữ cảnh hội thoại      ║\n");
    printf("║  /intent    Bật/tắt phân tích ý định    ║\n");
    printf("║  /attribute Bật/tắt nhận thuộc tính     ║\n");
    printf("║  /physics   Bật/tắt nhà vật lý         ║\n");
    printf("║  /chemistry Bật/tắt hóa học            ║\n");
    printf("║  /coder     Bật/tắt viết code          ║\n");
    printf("║  /knowledge Bật/tắt kiến thức C        ║\n");
    printf("║  /vietnamese Bật/tắt tiếng Việt        ║\n");
    printf("║  /bridge    Bật/tắt suy nghĩ tổng hợp  ║\n");
    printf("║  /clear     Xóa màn hình                ║\n");
    printf("║  /reset     Xóa ngữ cảnh                ║\n");
    printf("║  /exit      Thoát (tiếc nuối)          ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    printf("\n");
}

int main() {
    print_banner();
    Brain brain;
    brain_init(&brain, "CAI");
    printf("📚 Đang huấn luyện... (Bridge đang kết nối các kiến thức)\n");
    brain_train(&brain, "data/knowledge.txt");
    printf("✅ Sẵn sàng! Gõ /help để xem lệnh.\n");
    printf("   (Tôi sẽ suy nghĩ tổng hợp trước khi trả lời!)\n\n");
    char input[1000];
    char context_buf[2000];
    while (1) {
        printf("🧑 Bạn: ");
        fgets(input, sizeof(input), stdin);
        trim(input);
        if (strcmp(input, "/exit") == 0 || strcmp(input, "/quit") == 0) {
            printf("🤖 CAI: Tạm biệt! Hẹn gặp lại!\n");
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
        else if (strcmp(input, "/context") == 0) {
            brain_get_context(&brain, context_buf, sizeof(context_buf));
            printf("📝 Ngữ cảnh hội thoại:\n%s\n", context_buf);
            continue;
        }
        else if (strcmp(input, "/reset") == 0) {
            context_reset(&brain.context);
            printf("✅ Đã xóa ngữ cảnh!\n");
            continue;
        }
        else if (strcmp(input, "/intent") == 0) {
            brain.enable_intent = !brain.enable_intent;
            printf("✅ Intent: %s\n", brain.enable_intent ? "BẬT" : "TẮT");
            continue;
        }
        else if (strcmp(input, "/attribute") == 0) {
            brain.enable_attribute = !brain.enable_attribute;
            printf("✅ Attribute: %s\n", brain.enable_attribute ? "BẬT" : "TẮT");
            continue;
        }
        else if (strcmp(input, "/physics") == 0) {
            brain.enable_physics = !brain.enable_physics;
            printf("✅ Physics: %s\n", brain.enable_physics ? "BẬT 🔬" : "TẮT");
            continue;
        }
        else if (strcmp(input, "/chemistry") == 0) {
            brain.enable_chemistry = !brain.enable_chemistry;
            printf("✅ Chemistry: %s\n", brain.enable_chemistry ? "BẬT 🧪" : "TẮT");
            continue;
        }
        else if (strcmp(input, "/coder") == 0) {
            brain.enable_coder = !brain.enable_coder;
            printf("✅ Coder: %s\n", brain.enable_coder ? "BẬT 💻" : "TẮT");
            continue;
        }
        else if (strcmp(input, "/knowledge") == 0) {
            brain.enable_knowledge = !brain.enable_knowledge;
            printf("✅ Knowledge: %s\n", brain.enable_knowledge ? "BẬT 📚" : "TẮT");
            continue;
        }
        else if (strcmp(input, "/vietnamese") == 0) {
            brain.enable_vietnamese = !brain.enable_vietnamese;
            printf("✅ Vietnamese: %s\n", brain.enable_vietnamese ? "BẬT 🇻🇳" : "TẮT");
            continue;
        }
        else if (strcmp(input, "/bridge") == 0) {
            brain.enable_bridge = !brain.enable_bridge;
            printf("✅ Bridge: %s\n", brain.enable_bridge ? "BẬT 🧠" : "TẮT");
            continue;
        }
        if (strlen(input) == 0) {
            printf("🤖 CAI: Hỏi gì đi bạn!\n\n");
            continue;
        }
        brain_learn(&brain, input);
        char *response = brain_chat(&brain, input);
        printf("🤖 CAI: %s\n\n", response);
    }
    brain_free(&brain);
    return 0;
}

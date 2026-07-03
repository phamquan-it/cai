#include <stdio.h>
#include <string.h>
#include "brain.h"
#include "utils.h"

void print_banner() {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                                                              ║\n");
    printf("║   ██████  ▄████▄   ▄▄▄       ██▓███                         ║\n");
    printf("║  ▒██    ▒▒██▀ ▀█  ▒████▄    ▓██░  ██▒                       ║\n");
    printf("║  ░ ▓██▄  ▒▓█    ▄ ▒██  ▀█▄  ▓██░ ██▓▒                       ║\n");
    printf("║    ▒   ██▒▒▓▓▄ ▄██▒░██▄▄▄▄██ ▒██▄█▓▒ ▒                       ║\n");
    printf("║  ▒██████▒▒▒ ▓███▀ ░ ▓█   ▓██▒▒██▒ ░  ░                       ║\n");
    printf("║  ▒ ▒▓▒ ▒ ░░ ░▒ ▒  ░ ▒▒   ▓▒█░▒▓▒░ ░  ░                       ║\n");
    printf("║  ░ ░▒  ░ ░  ░  ▒     ▒   ▒▒ ░░▒ ░                            ║\n");
    printf("║  ░  ░  ░  ░          ░   ▒   ░░                              ║\n");
    printf("║        ░  ░ ░            ░  ░                                ║\n");
    printf("║          ░                                                    ║\n");
    printf("║                                                              ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║                                                              ║\n");
    printf("║   🧠  CAI - C-AI (Cognitive Artificial Intelligence)         ║\n");
    printf("║                                                              ║\n");
    printf("║   \"Viết bằng C - Chạy nhanh hơn cả tốc độ lướt TikTok\"      ║\n");
    printf("║                                                              ║\n");
    printf("║   ⚡ Python: \"Tôi có AI!\"                                   ║\n");
    printf("║   🚀 CAI:   \"Ờ, tôi viết bằng C, chạy xong thì Python        ║\n");
    printf("║              mới đang import tensorflow...\"                  ║\n");
    printf("║                                                              ║\n");
    printf("║   🔥 Dám cá là bạn chưa thấy AI nào chạy nhanh như này!      ║\n");
    printf("║   💪 Không GPU, không CUDA, chỉ C và lòng tự trọng!          ║\n");
    printf("║                                                              ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║                                                              ║\n");
    printf("║   🛑  TUỔI GÌ NHẢY VÔ NÉM GẠCH?                             ║\n");
    printf("║   📌  TUI LÀM CHƠI CHỨ BỘ CÓ LÀM TIỀN ĐÂU                  ║\n");
    printf("║   😤  ĐỪNG SO SÁNH VỚI CHATGPT - TUI KHÁC MÀ               ║\n");
    printf("║   🚀  NGHỊCH CHO VUI THÔI - AE ĐỪNG NÓNG                   ║\n");
    printf("║   🙏  XIN ĐỪNG NÉM GẠCH - TUI CHỈ LÀ CODE C               ║\n");
    printf("║                                                              ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║   📖 Gõ /help để xem lệnh | 🧠 AI thuần C - Cơ bản nhất   ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("  ╔═══════════════════════════════════════════════════════╗\n");
    printf("  ║  💬 \"Tôi không cần GPU, tôi cần một câu hỏi hay!\"   ║\n");
    printf("  ║  🙏  LÀM CHƠI THÔI - ĐỪNG NÉM GẠCH NGHE              ║\n");
    printf("  ║  🧠  NẾU CÓ GÌ SAI, THÌ... TẠI TÔI NGU!              ║\n");
    printf("  ╚═══════════════════════════════════════════════════════╝\n");
    printf("\n");
}

void print_help() {
    printf("╔══════════════════════════════════════════╗\n");
    printf("║  📜  DANH SÁCH LỆNH                      ║\n");
    printf("╠══════════════════════════════════════════╣\n");
    printf("║  /help      Hiển thị trợ giúp            ║\n");
    printf("║  /stats     Xem thống kê bá đạo         ║\n");
    printf("║  /context   Xem ngữ cảnh hội thoại      ║\n");
    printf("║  /clear     Xóa màn hình                ║\n");
    printf("║  /exit      Thoát (tiếc nuối)          ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    printf("\n");
}

int main() {
    print_banner();
    Brain brain;
    brain_init(&brain, "CAI");
    printf("📚 Đang huấn luyện... (Cố gắng đừng ném gạch nghe!)\n");
    brain_train(&brain, "data/knowledge.txt");
    printf("✅ Sẵn sàng! Gõ /help để xem lệnh.\n");
    printf("   (Tôi biết tôi không thông minh như ChatGPT, nhưng tôi chạy nhanh!)\n\n");
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
        if (strlen(input) == 0) {
            printf("🤖 CAI: Hỏi gì đi! Đừng có im lặng rồi ném gạch nghe!\n\n");
            continue;
        }
        brain_learn(&brain, input);
        char *response = brain_chat(&brain, input);
        printf("🤖 CAI: %s\n\n", response);
    }
    brain_free(&brain);
    return 0;
}

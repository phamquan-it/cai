#include "debug_logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#define MAX_DEBUG_ENTRIES 200

static int debug_enabled = 0;
static DebugEntry debug_entries[MAX_DEBUG_ENTRIES];
static int debug_count = 0;
static char debug_colors[][16] = {
    "\033[32m", // GREEN - ENTER
    "\033[31m", // RED - EXIT
    "\033[33m", // YELLOW - PROCESS
    "\033[36m", // CYAN - INFO
    "\033[35m", // MAGENTA - WARNING
    "\033[0m"   // RESET
};

// ============================================
//  KHỞI TẠO
// ============================================

void debug_init() {
    debug_enabled = 0;
    debug_count = 0;
    for (int i = 0; i < MAX_DEBUG_ENTRIES; i++) {
        debug_entries[i].file_name[0] = '\0';
        debug_entries[i].function_name[0] = '\0';
        debug_entries[i].stage[0] = '\0';
        debug_entries[i].message[0] = '\0';
        debug_entries[i].timestamp = 0;
    }
}

void debug_set_enabled(int enabled) {
    debug_enabled = enabled;
    if (enabled) {
        printf("🐛 DEBUG ENABLED\n");
        printf("📋 File: %s\n", __FILE__);
        printf("⏰ Time: %s", ctime(&(time_t){time(NULL)}));
        printf("─────────────────────────────────────────────\n");
    } else {
        printf("🐛 DEBUG DISABLED\n");
    }
}

int debug_is_enabled() {
    return debug_enabled;
}

// ============================================
//  HÀM LOG
// ============================================

void debug_log(const char *file, const char *function, const char *stage, const char *message) {
    if (!debug_enabled) return;
    if (debug_count >= MAX_DEBUG_ENTRIES) {
        for (int i = 0; i < MAX_DEBUG_ENTRIES - 1; i++) {
            debug_entries[i] = debug_entries[i + 1];
        }
        debug_count = MAX_DEBUG_ENTRIES - 1;
    }
    
    const char *filename = strrchr(file, '/');
    if (filename) {
        filename++;
    } else {
        filename = file;
    }
    
    strcpy(debug_entries[debug_count].file_name, filename);
    strcpy(debug_entries[debug_count].function_name, function);
    strcpy(debug_entries[debug_count].stage, stage);
    strcpy(debug_entries[debug_count].message, message ? message : "");
    debug_entries[debug_count].timestamp = time(NULL);
    debug_count++;
    
    char time_str[32];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);
    
    const char *color = debug_colors[5];
    if (strstr(stage, "ENTER") != NULL) color = debug_colors[0];
    else if (strstr(stage, "EXIT") != NULL) color = debug_colors[1];
    else if (strstr(stage, "PROCESS") != NULL) color = debug_colors[2];
    else if (strstr(stage, "INFO") != NULL) color = debug_colors[3];
    else if (strstr(stage, "WARN") != NULL) color = debug_colors[4];
    
    printf("%s[%s] 🐛 %s:%s() - %s%s\n", 
           color, time_str, filename, function, stage, debug_colors[5]);
    if (message && strlen(message) > 0) {
        printf("   💬 %s\n", message);
    }
}

void debug_log_format(const char *file, const char *function, const char *stage, const char *format, ...) {
    if (!debug_enabled) return;
    
    char message[256];
    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);
    
    debug_log(file, function, stage, message);
}

void debug_log_simple(const char *file, const char *function, const char *stage) {
    debug_log(file, function, stage, NULL);
}

// ============================================
//  HÀM HIỂN THỊ
// ============================================

void debug_show_logs() {
    if (debug_count == 0) {
        printf("📭 Chưa có log nào!\n");
        return;
    }
    
    printf("\n╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                    📋 DEBUG LOGS                         ║\n");
    printf("╠═══════════════════════════════════════════════════════════╣\n");
    
    for (int i = 0; i < debug_count; i++) {
        char time_str[32];
        struct tm *tm_info = localtime(&debug_entries[i].timestamp);
        strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);
        
        printf("║ [%s] %-20s %-15s %s\n", 
               time_str,
               debug_entries[i].file_name,
               debug_entries[i].function_name,
               debug_entries[i].stage);
        if (strlen(debug_entries[i].message) > 0) {
            printf("║   └─ %s\n", debug_entries[i].message);
        }
    }
    printf("╚═══════════════════════════════════════════════════════════╝\n");
}

void debug_clear_logs() {
    debug_count = 0;
    printf("🗑️  Đã xóa debug logs\n");
}

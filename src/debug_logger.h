#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Cấu trúc debug session
typedef struct {
    char file_name[64];
    char function_name[64];
    char stage[128];
    char message[256];
    time_t timestamp;
} DebugEntry;

// Hàm chính
void debug_init();
void debug_set_enabled(int enabled);
int debug_is_enabled();

// Hàm log
void debug_log(const char *file, const char *function, const char *stage, const char *message);
void debug_log_simple(const char *file, const char *function, const char *stage);
void debug_log_format(const char *file, const char *function, const char *stage, const char *format, ...);

// Hàm hiển thị
void debug_show_logs();
void debug_clear_logs();

// Macro tiện ích - FIX: chỉ nhận 2 tham số, dùng debug_log_format cho format
#define DEBUG_LOG(stage, msg) debug_log(__FILE__, __func__, stage, msg)
#define DEBUG_LOG_FMT(stage, ...) debug_log_format(__FILE__, __func__, stage, __VA_ARGS__)
#define DEBUG_STAGE(stage) debug_log_simple(__FILE__, __func__, stage)
#define DEBUG_ENTER() debug_log_simple(__FILE__, __func__, "ENTER")
#define DEBUG_EXIT() debug_log_simple(__FILE__, __func__, "EXIT")

#endif

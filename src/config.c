#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

static Config *global_config = NULL;

// ============================================
//  ĐỌC BIẾN MÔI TRƯỜNG (env)
// ============================================

char* get_env(const char *key) {
    return getenv(key);
}

int get_env_int(const char *key, int default_val) {
    char *val = getenv(key);
    if (val) {
        return atoi(val);
    }
    return default_val;
}

const char* get_env_string(const char *key, const char *default_val) {
    char *val = getenv(key);
    if (val) {
        return val;
    }
    return default_val;
}

int get_env_bool(const char *key, int default_val) {
    char *val = getenv(key);
    if (val) {
        char lower[16];
        strcpy(lower, val);
        for (int i = 0; lower[i]; i++) {
            lower[i] = tolower(lower[i]);
        }
        if (strcmp(lower, "true") == 0 || strcmp(lower, "1") == 0 || 
            strcmp(lower, "yes") == 0 || strcmp(lower, "on") == 0) {
            return 1;
        }
        if (strcmp(lower, "false") == 0 || strcmp(lower, "0") == 0 || 
            strcmp(lower, "no") == 0 || strcmp(lower, "off") == 0) {
            return 0;
        }
        return atoi(val) != 0;
    }
    return default_val;
}

// ============================================
//  KHỞI TẠO CẤU HÌNH
// ============================================

Config* config_init() {
    Config *cfg = (Config*)malloc(sizeof(Config));
    if (!cfg) return NULL;
    
    // Mặc định
    cfg->debug_enabled = 0;
    cfg->debug_show_logs = 0;
    
    // Tính năng - TẤT CẢ TẮT, CHỈ MARKOV + CONTEXT + VIETNAMESE
    cfg->enable_intent = 0;
    cfg->enable_attribute = 0;
    cfg->enable_physics = 0;
    cfg->enable_coder = 0;
    cfg->enable_knowledge = 0;
    cfg->enable_chemistry = 0;
    cfg->enable_vietnamese = 1;
    cfg->enable_bridge = 0;
    cfg->enable_reasoning = 0;
    cfg->enable_markov = 1;
    cfg->enable_context = 1;
    
    // Đường dẫn
    strcpy(cfg->data_file, "data/knowledge.txt");
    strcpy(cfg->log_file, "logs/cai.log");
    
    // Giới hạn
    cfg->max_context = 10;
    cfg->max_response = 100;
    cfg->max_vocab = 1000;
    
    global_config = cfg;
    
    return cfg;
}

void config_free(Config *cfg) {
    if (cfg) free(cfg);
    global_config = NULL;
}

// ============================================
//  LOAD CẤU HÌNH TỪ ENV
// ============================================

int config_load_from_env(Config *cfg) {
    if (!cfg) return 0;
    
    // Debug
    cfg->debug_enabled = get_env_bool("DEBUG_ENABLED", cfg->debug_enabled);
    cfg->debug_show_logs = get_env_bool("DEBUG_SHOW_LOGS", cfg->debug_show_logs);
    
    // Tính năng
    cfg->enable_intent = get_env_bool("INTENT_ENABLED", cfg->enable_intent);
    cfg->enable_attribute = get_env_bool("ATTRIBUTE_ENABLED", cfg->enable_attribute);
    cfg->enable_physics = get_env_bool("PHYSICS_ENABLED", cfg->enable_physics);
    cfg->enable_coder = get_env_bool("CODER_ENABLED", cfg->enable_coder);
    cfg->enable_knowledge = get_env_bool("KNOWLEDGE_ENABLED", cfg->enable_knowledge);
    cfg->enable_chemistry = get_env_bool("CHEMISTRY_ENABLED", cfg->enable_chemistry);
    cfg->enable_vietnamese = get_env_bool("VIETNAMESE_ENABLED", cfg->enable_vietnamese);
    cfg->enable_bridge = get_env_bool("BRIDGE_ENABLED", cfg->enable_bridge);
    cfg->enable_reasoning = get_env_bool("REASONING_ENABLED", cfg->enable_reasoning);
    cfg->enable_markov = get_env_bool("MARKOV_ENABLED", cfg->enable_markov);
    cfg->enable_context = get_env_bool("CONTEXT_ENABLED", cfg->enable_context);
    
    // Đường dẫn
    const char *data = get_env_string("DATA_FILE", NULL);
    if (data) strcpy(cfg->data_file, data);
    
    const char *log = get_env_string("LOG_FILE", NULL);
    if (log) strcpy(cfg->log_file, log);
    
    // Giới hạn
    cfg->max_context = get_env_int("MAX_CONTEXT", cfg->max_context);
    cfg->max_response = get_env_int("MAX_RESPONSE", cfg->max_response);
    cfg->max_vocab = get_env_int("MAX_VOCAB", cfg->max_vocab);
    
    return 1;
}

// ============================================
//  LOAD CẤU HÌNH TỪ FILE .env
// ============================================

int config_load_from_file(Config *cfg, const char *filename) {
    if (!cfg || !filename) return 0;
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("⚠️  Không tìm thấy file cấu hình: %s, dùng mặc định\n", filename);
        return 0;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;
        
        // Tách key=value
        char *eq = strchr(line, '=');
        if (!eq) continue;
        
        *eq = '\0';
        char *key = line;
        char *value = eq + 1;
        
        // Xóa khoảng trắng
        while (*key == ' ' || *key == '\t') key++;
        while (*value == ' ' || *value == '\t') value++;
        
        // Xóa newline
        char *nl = strchr(value, '\n');
        if (nl) *nl = '\0';
        nl = strchr(value, '\r');
        if (nl) *nl = '\0';
        
        // Đặt biến môi trường
        setenv(key, value, 1);
    }
    fclose(file);
    
    // Load lại từ env
    return config_load_from_env(cfg);
}

// ============================================
//  LẤY GIÁ TRỊ CẤU HÌNH
// ============================================

int config_get_int(const char *key, int default_val) {
    char *env = getenv(key);
    if (env) return atoi(env);
    return default_val;
}

const char* config_get_string(const char *key, const char *default_val) {
    char *env = getenv(key);
    if (env) return env;
    return default_val;
}

int config_get_bool(const char *key, int default_val) {
    char *env = getenv(key);
    if (env) {
        char lower[16];
        strcpy(lower, env);
        for (int i = 0; lower[i]; i++) lower[i] = tolower(lower[i]);
        if (strcmp(lower, "true") == 0 || strcmp(lower, "1") == 0 || 
            strcmp(lower, "yes") == 0 || strcmp(lower, "on") == 0) return 1;
        if (strcmp(lower, "false") == 0 || strcmp(lower, "0") == 0 || 
            strcmp(lower, "no") == 0 || strcmp(lower, "off") == 0) return 0;
        return atoi(env) != 0;
    }
    return default_val;
}

// ============================================
//  HIỂN THỊ VÀ LƯU CẤU HÌNH
// ============================================

void config_print(Config *cfg) {
    if (!cfg) return;
    
    printf("\n╔════════════════════════════════════════════════════════╗\n");
    printf("║                    📋 CẤU HÌNH                        ║\n");
    printf("╠════════════════════════════════════════════════════════╣\n");
    printf("║ DEBUG:           %-30s ║\n", cfg->debug_enabled ? "BẬT ✅" : "TẮT ❌");
    printf("║ DEBUG SHOW LOGS: %-30s ║\n", cfg->debug_show_logs ? "BẬT ✅" : "TẮT ❌");
    printf("╠════════════════════════════════════════════════════════╣\n");
    printf("║ TÍNH NĂNG:                                           ║\n");
    printf("║   Intent:       %-30s ║\n", cfg->enable_intent ? "BẬT ✅" : "TẮT ❌");
    printf("║   Attribute:    %-30s ║\n", cfg->enable_attribute ? "BẬT ✅" : "TẮT ❌");
    printf("║   Physics:      %-30s ║\n", cfg->enable_physics ? "BẬT ✅" : "TẮT ❌");
    printf("║   Coder:        %-30s ║\n", cfg->enable_coder ? "BẬT ✅" : "TẮT ❌");
    printf("║   Knowledge:    %-30s ║\n", cfg->enable_knowledge ? "BẬT ✅" : "TẮT ❌");
    printf("║   Chemistry:    %-30s ║\n", cfg->enable_chemistry ? "BẬT ✅" : "TẮT ❌");
    printf("║   Vietnamese:   %-30s ║\n", cfg->enable_vietnamese ? "BẬT ✅" : "TẮT ❌");
    printf("║   Bridge:       %-30s ║\n", cfg->enable_bridge ? "BẬT ✅" : "TẮT ❌");
    printf("║   Reasoning:    %-30s ║\n", cfg->enable_reasoning ? "BẬT ✅" : "TẮT ❌");
    printf("║   Markov:       %-30s ║\n", cfg->enable_markov ? "BẬT ✅" : "TẮT ❌");
    printf("║   Context:      %-30s ║\n", cfg->enable_context ? "BẬT ✅" : "TẮT ❌");
    printf("╠════════════════════════════════════════════════════════╣\n");
    printf("║ ĐƯỜNG DẪN:                                           ║\n");
    printf("║   Data file:   %-30s ║\n", cfg->data_file);
    printf("║   Log file:    %-30s ║\n", cfg->log_file);
    printf("╠════════════════════════════════════════════════════════╣\n");
    printf("║ GIỚI HẠN:                                            ║\n");
    printf("║   Max context: %-30d ║\n", cfg->max_context);
    printf("║   Max response:%-30d ║\n", cfg->max_response);
    printf("║   Max vocab:   %-30d ║\n", cfg->max_vocab);
    printf("╚════════════════════════════════════════════════════════╝\n");
}

void config_save(Config *cfg, const char *filename) {
    if (!cfg || !filename) return;
    
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("❌ Không thể lưu file cấu hình: %s\n", filename);
        return;
    }
    
    fprintf(file, "# CAI Configuration File\n");
    fprintf(file, "# Generated: %s", ctime(&(time_t){time(NULL)}));
    fprintf(file, "\n");
    
    fprintf(file, "# Debug\n");
    fprintf(file, "DEBUG_ENABLED=%d\n", cfg->debug_enabled);
    fprintf(file, "DEBUG_SHOW_LOGS=%d\n", cfg->debug_show_logs);
    fprintf(file, "\n");
    
    fprintf(file, "# Features\n");
    fprintf(file, "INTENT_ENABLED=%d\n", cfg->enable_intent);
    fprintf(file, "ATTRIBUTE_ENABLED=%d\n", cfg->enable_attribute);
    fprintf(file, "PHYSICS_ENABLED=%d\n", cfg->enable_physics);
    fprintf(file, "CODER_ENABLED=%d\n", cfg->enable_coder);
    fprintf(file, "KNOWLEDGE_ENABLED=%d\n", cfg->enable_knowledge);
    fprintf(file, "CHEMISTRY_ENABLED=%d\n", cfg->enable_chemistry);
    fprintf(file, "VIETNAMESE_ENABLED=%d\n", cfg->enable_vietnamese);
    fprintf(file, "BRIDGE_ENABLED=%d\n", cfg->enable_bridge);
    fprintf(file, "REASONING_ENABLED=%d\n", cfg->enable_reasoning);
    fprintf(file, "MARKOV_ENABLED=%d\n", cfg->enable_markov);
    fprintf(file, "CONTEXT_ENABLED=%d\n", cfg->enable_context);
    fprintf(file, "\n");
    
    fprintf(file, "# Paths\n");
    fprintf(file, "DATA_FILE=%s\n", cfg->data_file);
    fprintf(file, "LOG_FILE=%s\n", cfg->log_file);
    fprintf(file, "\n");
    
    fprintf(file, "# Limits\n");
    fprintf(file, "MAX_CONTEXT=%d\n", cfg->max_context);
    fprintf(file, "MAX_RESPONSE=%d\n", cfg->max_response);
    fprintf(file, "MAX_VOCAB=%d\n", cfg->max_vocab);
    
    fclose(file);
    printf("✅ Đã lưu cấu hình vào: %s\n", filename);
}

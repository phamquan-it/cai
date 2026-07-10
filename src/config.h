#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ============================================
//  CẤU HÌNH MẶC ĐỊNH
// ============================================

typedef struct {
    // Debug
    int debug_enabled;
    int debug_show_logs;
    
    // Các tính năng
    int enable_intent;
    int enable_attribute;
    int enable_physics;
    int enable_coder;
    int enable_knowledge;
    int enable_chemistry;
    int enable_vietnamese;
    int enable_bridge;
    int enable_reasoning;
    int enable_markov;
    int enable_context;
    
    // Đường dẫn
    char data_file[256];
    char log_file[256];
    
    // Giới hạn
    int max_context;
    int max_response;
    int max_vocab;
} Config;

// ============================================
//  HÀM API
// ============================================

// Khởi tạo cấu hình
Config* config_init();
void config_free(Config *cfg);

// Load từ file .env hoặc môi trường
int config_load_from_env(Config *cfg);
int config_load_from_file(Config *cfg, const char *filename);

// Lấy giá trị
int config_get_int(const char *key, int default_val);
const char* config_get_string(const char *key, const char *default_val);
int config_get_bool(const char *key, int default_val);

// Hiển thị
void config_print(Config *cfg);
void config_save(Config *cfg, const char *filename);

// ============================================
//  MACRO TIỆN ÍCH
// ============================================

#define CFG_DEBUG_ENABLED() (config_get_bool("DEBUG_ENABLED", 0))
#define CFG_INTENT_ENABLED() (config_get_bool("INTENT_ENABLED", 0))
#define CFG_REASONING_ENABLED() (config_get_bool("REASONING_ENABLED", 1))
#define CFG_PHYSICS_ENABLED() (config_get_bool("PHYSICS_ENABLED", 0))
#define CFG_CODER_ENABLED() (config_get_bool("CODER_ENABLED", 0))
#define CFG_KNOWLEDGE_ENABLED() (config_get_bool("KNOWLEDGE_ENABLED", 0))
#define CFG_CHEMISTRY_ENABLED() (config_get_bool("CHEMISTRY_ENABLED", 0))
#define CFG_VIETNAMESE_ENABLED() (config_get_bool("VIETNAMESE_ENABLED", 1))
#define CFG_BRIDGE_ENABLED() (config_get_bool("BRIDGE_ENABLED", 0))
#define CFG_MARKOV_ENABLED() (config_get_bool("MARKOV_ENABLED", 1))
#define CFG_CONTEXT_ENABLED() (config_get_bool("CONTEXT_ENABLED", 1))

#define CFG_DATA_FILE() (config_get_string("DATA_FILE", "data/knowledge.txt"))
#define CFG_LOG_FILE() (config_get_string("LOG_FILE", "logs/cai.log"))

#define CFG_MAX_CONTEXT() (config_get_int("MAX_CONTEXT", 10))
#define CFG_MAX_RESPONSE() (config_get_int("MAX_RESPONSE", 100))
#define CFG_MAX_VOCAB() (config_get_int("MAX_VOCAB", 1000))

#endif

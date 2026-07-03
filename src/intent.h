#ifndef INTENT_H
#define INTENT_H

#define MAX_INTENT_LEN 256

typedef enum {
    INTENT_UNKNOWN,
    INTENT_WANT,      // muốn cái gì
    INTENT_BUY,       // mua cái gì
    INTENT_ASK,       // hỏi về cái gì
    INTENT_LOOKING,   // tìm cái gì
    INTENT_NEED,      // cần cái gì
    INTENT_WANT_TO,   // muốn làm gì
    INTENT_CAN,       // có thể làm gì
    INTENT_KNOW,      // biết gì về
    INTENT_HELP,      // giúp tôi với
    INTENT_GO,        // đi đâu
    INTENT_EAT,       // ăn gì
    INTENT_DRINK,     // uống gì
    INTENT_LEARN,     // học gì
    INTENT_WATCH,     // xem gì
    INTENT_LISTEN,    // nghe gì
    INTENT_LOVE,      // thích gì
    INTENT_HATE,      // ghét gì
    INTENT_FEEL       // cảm thấy thế nào
} IntentType;

typedef struct {
    IntentType type;
    char object[MAX_INTENT_LEN];  // đối tượng (ví dụ: "món ăn", "xe máy")
    float confidence;
    char raw_query[MAX_INTENT_LEN];
} Intent;

// Hàm phân tích
Intent* analyze_intent(const char *input);
void free_intent(Intent *intent);

// Hàm sinh response theo intent
char* generate_response_by_intent(Intent *intent);

#endif

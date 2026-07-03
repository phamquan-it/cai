#include "intent.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>

// Cấu trúc pattern
typedef struct {
    const char *pattern;
    IntentType intent;
    const char *example_response;
} IntentPattern;

// Danh sách patterns - sắp xếp từ cụ thể đến tổng quát
static IntentPattern patterns[] = {
    // MUỐN
    {".*muốn mua.*", INTENT_BUY, "Bạn muốn mua gì? Tôi có thể tư vấn!"},
    {".*muốn có.*", INTENT_WANT, "Bạn muốn có cái gì? Kể tôi nghe với!"},
    {".*muốn làm.*", INTENT_WANT_TO, "Bạn muốn làm gì? Hãy chia sẻ!"},
    {".*muốn (học|tìm hiểu).*", INTENT_LEARN, "Học là điều tuyệt vời! Bạn muốn học gì?"},
    {".*muốn (xem|coi).*", INTENT_WATCH, "Bạn muốn xem gì? Tôi có thể gợi ý!"},
    {".*muốn (nghe|nghe nhạc).*", INTENT_LISTEN, "Bạn muốn nghe gì? Nhạc hay podcast?"},
    {".*muốn (ăn|uống).*", INTENT_EAT, "Bạn muốn ăn/uống gì? Tôi có thể gợi ý món!"},
    {".*muốn đi.*", INTENT_GO, "Bạn muốn đi đâu? Tôi có thể gợi ý địa điểm!"},
    {".*muốn (biết|hỏi).*", INTENT_ASK, "Bạn muốn hỏi gì? Tôi sẵn sàng giải đáp!"},
    {".*muốn.*", INTENT_WANT, "Vậy bạn muốn gì? Hãy nói cụ thể nhé!"},
    
    // MUA
    {".*mua cái.*", INTENT_BUY, "Bạn muốn mua cái gì? Tôi có thể tư vấn!"},
    {".*mua (điện thoại|laptop|xe).*", INTENT_BUY, "Mua công nghệ à? Hãy hỏi tôi!"},
    {".*mua (nhà|đất|chung cư).*", INTENT_BUY, "Mua nhà đất cần cân nhắc nhiều lắm!"},
    {".*mua (quần áo|giày dép|túi xách).*", INTENT_BUY, "Thời trang luôn là lựa chọn thú vị!"},
    {".*mua (đồ ăn|thực phẩm).*", INTENT_BUY, "Mua đồ ăn ở đâu? Tôi có thể gợi ý!"},
    {".*mua.*", INTENT_BUY, "Bạn muốn mua gì? Nói tôi nghe với!"},
    
    // HỎI
    {".*hỏi về.*", INTENT_ASK, "Bạn muốn hỏi về cái gì? Tôi sẵn sàng trả lời!"},
    {".*cho tôi (hỏi|hỏi về).*", INTENT_ASK, "Hãy hỏi đi! Tôi sẽ cố gắng giải đáp!"},
    {".*hỏi.*", INTENT_ASK, "Bạn hỏi gì thế? Tôi nghe đây!"},
    
    // CẦN
    {".*cần (giúp|giúp đỡ).*", INTENT_HELP, "Tôi sẵn sàng giúp bạn! Bạn cần gì?"},
    {".*cần (mua|mua sắm).*", INTENT_BUY, "Bạn cần mua gì? Tôi có thể tư vấn!"},
    {".*cần (tìm|tìm kiếm).*", INTENT_LOOKING, "Bạn cần tìm gì? Tôi sẽ giúp bạn!"},
    {".*cần.*", INTENT_NEED, "Bạn cần gì? Hãy nói cụ thể nhé!"},
    
    // TÌM
    {".*tìm (kiếm|thấy).*", INTENT_LOOKING, "Bạn đang tìm gì? Tôi có thể giúp!"},
    {".*tìm (chỗ|địa điểm|nơi).*", INTENT_GO, "Bạn muốn tìm địa điểm nào?"},
    {".*tìm.*", INTENT_LOOKING, "Bạn đang tìm cái gì vậy?"},
    
    // CÓ THỂ
    {".*có thể.*", INTENT_CAN, "Tôi có thể làm được nhiều thứ lắm! Bạn muốn gì?"},
    
    // BIẾT
    {".*(biết|hiểu) (về|gì).*", INTENT_KNOW, "Bạn muốn biết về cái gì? Tôi biết khá nhiều đó!"},
    {".*biết.*", INTENT_KNOW, "Bạn muốn biết điều gì?"},
    
    // CẢM XÚC
    {".*thích.*", INTENT_LOVE, "Bạn thích cái gì? Hãy chia sẻ với tôi!"},
    {".*(không thích|ghét).*", INTENT_HATE, "Không thích à? Tôi hiểu mà!"},
    {".*cảm thấy.*", INTENT_FEEL, "Bạn cảm thấy thế nào? Kể tôi nghe đi!"},
    
    // GIÚP
    {".*giúp (tôi|em|mình).*", INTENT_HELP, "Tôi sẵn sàng giúp bạn! Bạn cần gì?"},
    {".*giúp.*", INTENT_HELP, "Tôi có thể giúp gì cho bạn?"},
};

#define NUM_PATTERNS (sizeof(patterns) / sizeof(patterns[0]))

// Hàm trích xuất đối tượng
static void extract_object(const char *input, Intent *intent) {
    // Tìm từ sau các từ khóa: "muốn", "mua", "hỏi", "cần", "tìm"
    const char *keywords[] = {"muốn", "mua", "hỏi", "cần", "tìm", "biết", "thích"};
    char temp[1000];
    strcpy(temp, input);
    
    for (int i = 0; i < 7; i++) {
        char *pos = strstr(temp, keywords[i]);
        if (pos) {
            pos += strlen(keywords[i]);
            // Bỏ qua khoảng trắng
            while (*pos == ' ') pos++;
            
            // Lấy từ tiếp theo làm object
            int j = 0;
            while (pos[j] != ' ' && pos[j] != '.' && pos[j] != '?' && pos[j] != '\0' && j < MAX_INTENT_LEN - 1) {
                intent->object[j] = pos[j];
                j++;
            }
            intent->object[j] = '\0';
            
            if (strlen(intent->object) > 0) {
                break;
            }
        }
    }
    
    // Nếu không tìm thấy object, thử lấy từ cuối cùng
    if (strlen(intent->object) == 0) {
        char *last = strrchr(temp, ' ');
        if (last) {
            strcpy(intent->object, last + 1);
        } else {
            strcpy(intent->object, "điều đó");
        }
    }
}

// Hàm phân tích intent
Intent* analyze_intent(const char *input) {
    Intent *intent = (Intent*)malloc(sizeof(Intent));
    if (!intent) return NULL;
    
    intent->type = INTENT_UNKNOWN;
    intent->object[0] = '\0';
    intent->confidence = 0.0;
    strcpy(intent->raw_query, input);
    
    // Chuyển sang chữ thường để match
    char input_lower[1000];
    strcpy(input_lower, input);
    for (int i = 0; input_lower[i]; i++) {
        input_lower[i] = tolower(input_lower[i]);
    }
    
    // Regex match từng pattern
    for (int i = 0; i < NUM_PATTERNS; i++) {
        regex_t regex;
        int ret = regcomp(&regex, patterns[i].pattern, REG_EXTENDED | REG_ICASE | REG_NOSUB);
        if (ret == 0) {
            ret = regexec(&regex, input_lower, 0, NULL, 0);
            regfree(&regex);
            if (ret == 0) {
                intent->type = patterns[i].intent;
                intent->confidence = 0.9 - (i * 0.01); // độ ưu tiên giảm dần
                extract_object(input, intent);
                break;
            }
        }
    }
    
    // Nếu không match pattern nào
    if (intent->type == INTENT_UNKNOWN) {
        intent->confidence = 0.1;
    }
    
    return intent;
}

void free_intent(Intent *intent) {
    if (intent) free(intent);
}

// Sinh response theo intent
char* generate_response_by_intent(Intent *intent) {
    static char response[500];
    response[0] = '\0';
    
    if (intent->type == INTENT_UNKNOWN || intent->confidence < 0.5) {
        return NULL;  // để fallback về Markov
    }
    
    // Tìm pattern tương ứng để lấy response mẫu
    for (int i = 0; i < NUM_PATTERNS; i++) {
        regex_t regex;
        int ret = regcomp(&regex, patterns[i].pattern, REG_EXTENDED | REG_ICASE | REG_NOSUB);
        if (ret == 0) {
            char input_lower[1000];
            strcpy(input_lower, intent->raw_query);
            for (int j = 0; input_lower[j]; j++) {
                input_lower[j] = tolower(input_lower[j]);
            }
            ret = regexec(&regex, input_lower, 0, NULL, 0);
            regfree(&regex);
            if (ret == 0) {
                // Lấy response mẫu
                const char *example = patterns[i].example_response;
                
                // Tùy chỉnh theo intent
                switch (intent->type) {
                    case INTENT_WANT:
                        if (strlen(intent->object) > 0) {
                            snprintf(response, sizeof(response), 
                                     "Bạn muốn có %s phải không? Nói rõ hơn đi!", 
                                     intent->object);
                        } else {
                            strcpy(response, example);
                        }
                        break;
                        
                    case INTENT_BUY:
                        if (strlen(intent->object) > 0) {
                            snprintf(response, sizeof(response), 
                                     "Mua %s à? Tôi có thể tư vấn cho bạn!", 
                                     intent->object);
                        } else {
                            strcpy(response, example);
                        }
                        break;
                        
                    case INTENT_ASK:
                        if (strlen(intent->object) > 0) {
                            snprintf(response, sizeof(response), 
                                     "Về %s hả? Tôi sẽ cố gắng trả lời bạn!", 
                                     intent->object);
                        } else {
                            strcpy(response, example);
                        }
                        break;
                        
                    case INTENT_LOOKING:
                        if (strlen(intent->object) > 0) {
                            snprintf(response, sizeof(response), 
                                     "Tôi sẽ giúp bạn tìm %s!", 
                                     intent->object);
                        } else {
                            strcpy(response, example);
                        }
                        break;
                        
                    case INTENT_NEED:
                        if (strlen(intent->object) > 0) {
                            snprintf(response, sizeof(response), 
                                     "Bạn cần %s phải không? Tôi có thể giúp!", 
                                     intent->object);
                        } else {
                            strcpy(response, example);
                        }
                        break;
                        
                    case INTENT_HELP:
                        strcpy(response, "Tôi sẵn sàng giúp bạn! Hãy nói cụ thể bạn cần gì nhé!");
                        break;
                        
                    case INTENT_LEARN:
                        if (strlen(intent->object) > 0) {
                            snprintf(response, sizeof(response), 
                                     "Học %s rất hay đấy! Bắt đầu thôi!", 
                                     intent->object);
                        } else {
                            strcpy(response, "Học thật tuyệt vời! Bạn muốn học gì nào?");
                        }
                        break;
                        
                    case INTENT_EAT:
                        if (strlen(intent->object) > 0) {
                            snprintf(response, sizeof(response), 
                                     "Món %s nghe hấp dẫn đấy!", 
                                     intent->object);
                        } else {
                            strcpy(response, "Bạn thích ăn gì? Tôi biết nhiều món ngon lắm!");
                        }
                        break;
                        
                    case INTENT_GO:
                        if (strlen(intent->object) > 0) {
                            snprintf(response, sizeof(response), 
                                     "Đi %s à? Tôi có thể gợi ý vài chỗ!", 
                                     intent->object);
                        } else {
                            strcpy(response, "Bạn muốn đi đâu? Tôi có thể tư vấn!");
                        }
                        break;
                        
                    case INTENT_LOVE:
                        if (strlen(intent->object) > 0) {
                            snprintf(response, sizeof(response), 
                                     "Thích %s hả? Tôi cũng thích đó!", 
                                     intent->object);
                        } else {
                            strcpy(response, "Bạn thích điều gì? Kể tôi nghe với!");
                        }
                        break;
                        
                    default:
                        strcpy(response, example);
                        break;
                }
                break;
            }
        }
    }
    
    return response;
}

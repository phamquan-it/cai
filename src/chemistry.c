#include "chemistry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CHEMISTRY 100

static ChemistryFormula chemistry[MAX_CHEMISTRY];
static int chemistry_count = 0;

// ============================================
//  KHO KIẾN THỨC HÓA HỌC
// ============================================

void add_chemistry(const char *name, const char *formula, 
                   const char *desc, const char *example) {
    if (chemistry_count >= MAX_CHEMISTRY) return;
    strcpy(chemistry[chemistry_count].name, name);
    strcpy(chemistry[chemistry_count].formula, formula);
    strcpy(chemistry[chemistry_count].description, desc);
    strcpy(chemistry[chemistry_count].example, example ? example : "");
    chemistry_count++;
}

static void init_chemistry() {
    if (chemistry_count > 0) return;
    
    // ========================================
    //  NGUYÊN TỐ
    // ========================================
    add_chemistry("Hydrogen", "H", "Nguyên tố nhẹ nhất, chiếm 75% vũ trụ", "Nước H₂O chứa H");
    add_chemistry("H", "H", "Ký hiệu của Hydrogen", "H₂O, HCl, H₂SO₄");
    add_chemistry("Oxygen", "O", "Nguyên tố cần thiết cho sự sống, chiếm 21% không khí", "H₂O, CO₂, O₂");
    add_chemistry("O", "O", "Ký hiệu của Oxygen", "O₂, CO₂, H₂O");
    add_chemistry("Carbon", "C", "Nguyên tố nền tảng của sự sống, có trong mọi hợp chất hữu cơ", "CO₂, CH₄, C₆H₁₂O₆");
    add_chemistry("C", "C", "Ký hiệu của Carbon", "CO₂, CH₄, C₂H₅OH");
    add_chemistry("Nitrogen", "N", "Nguyên tố chiếm 78% không khí", "N₂, NH₃, HNO₃");
    add_chemistry("N", "N", "Ký hiệu của Nitrogen", "N₂, NH₃, HNO₃");
    add_chemistry("Sodium", "Na", "Kim loại kiềm, phản ứng mạnh với nước", "NaCl, NaOH, NaHCO₃");
    add_chemistry("Na", "Na", "Ký hiệu của Sodium (Natri)", "NaCl (muối ăn), NaOH");
    add_chemistry("Chlorine", "Cl", "Khí halogen màu vàng lục, độc", "NaCl, HCl, Cl₂");
    add_chemistry("Cl", "Cl", "Ký hiệu của Chlorine", "NaCl, HCl");
    add_chemistry("Iron", "Fe", "Kim loại phổ biến nhất, màu xám trắng", "Fe₂O₃, FeCl₃");
    add_chemistry("Fe", "Fe", "Ký hiệu của Iron (Sắt)", "Fe₂O₃ (gỉ sắt)");
    add_chemistry("Gold", "Au", "Kim loại quý, không bị oxy hóa", "Au, hợp kim trang sức");
    add_chemistry("Au", "Au", "Ký hiệu của Gold (Vàng)", "Au (vàng nguyên chất)");
    add_chemistry("Silver", "Ag", "Kim loại quý, dẫn điện tốt nhất", "Ag, AgNO₃");
    add_chemistry("Ag", "Ag", "Ký hiệu của Silver (Bạc)", "AgNO₃ (bạc nitrat)");
    add_chemistry("Helium", "He", "Khí trơ, nhẹ thứ hai, dùng trong bóng bay", "He");
    add_chemistry("He", "He", "Ký hiệu của Helium", "He (bóng bay)");
    
    // ========================================
    //  HỢP CHẤT
    // ========================================
    add_chemistry("Nước", "H₂O", "Hợp chất quan trọng nhất cho sự sống", "H₂O là dung môi phổ biến");
    add_chemistry("H₂O", "H₂O", "Công thức của nước", "Nước, băng, hơi nước");
    add_chemistry("Muối ăn", "NaCl", "Hợp chất ion tạo bởi Na và Cl", "NaCl dùng trong nấu ăn");
    add_chemistry("NaCl", "NaCl", "Công thức của muối ăn (Sodium Chloride)", "NaCl là muối ăn");
    add_chemistry("Axit clohidric", "HCl", "Axit mạnh, có trong dạ dày", "HCl giúp tiêu hóa");
    add_chemistry("HCl", "HCl", "Công thức của axit clohidric", "HCl trong dạ dày");
    add_chemistry("Axit sulfuric", "H₂SO₄", "Axit mạnh, dùng trong công nghiệp", "H₂SO₄ là 'vua của các axit'");
    add_chemistry("H₂SO₄", "H₂SO₄", "Công thức của axit sulfuric", "H₂SO₄ dùng trong pin");
    add_chemistry("Axit nitric", "HNO₃", "Axit mạnh, ăn mòn", "HNO₃ dùng trong thuốc nổ");
    add_chemistry("HNO₃", "HNO₃", "Công thức của axit nitric", "HNO₃ (nước cường toan)");
    add_chemistry("Axit cacbonic", "H₂CO₃", "Axit yếu, có trong nước có gas", "H₂CO₃ trong Coca Cola");
    add_chemistry("H₂CO₃", "H₂CO₃", "Công thức của axit cacbonic", "H₂CO₃ tạo CO₂");
    add_chemistry("Glucose", "C₆H₁₂O₆", "Đường đơn, nguồn năng lượng chính", "C₆H₁₂O₆ trong trái cây");
    add_chemistry("C₆H₁₂O₆", "C₆H₁₂O₆", "Công thức của Glucose", "Glucose cung cấp năng lượng");
    add_chemistry("Ethanol", "C₂H₅OH", "Cồn, chất hữu cơ dễ cháy", "C₂H₅OH trong rượu bia");
    add_chemistry("C₂H₅OH", "C₂H₅OH", "Công thức của ethanol (cồn)", "C₂H₅OH (cồn y tế)");
    add_chemistry("Metan", "CH₄", "Khí gas, dễ cháy", "CH₄ trong bếp gas");
    add_chemistry("CH₄", "CH₄", "Công thức của metan", "CH₄ (khí ga)");
    add_chemistry("Amoniac", "NH₃", "Khí mùi khai, tan trong nước", "NH₃ dùng trong phân bón");
    add_chemistry("NH₃", "NH₃", "Công thức của amoniac", "NH₃ (nước lau kính)");
    add_chemistry("CO₂", "CO₂", "Khí carbon dioxide, gây hiệu ứng nhà kính", "CO₂ trong nước có gas");
    add_chemistry("Carbon dioxide", "CO₂", "Khí thải từ hô hấp và đốt cháy", "CO₂ gây biến đổi khí hậu");
    
    // ========================================
    //  KHÁI NIỆM
    // ========================================
    add_chemistry("pH", "pH = -log[H⁺]", "Độ pH đo tính axit-bazơ của dung dịch", "pH 7 là trung tính");
    add_chemistry("axit", "Axit", "Chất có pH < 7, có vị chua", "HCl, H₂SO₄ là axit mạnh");
    add_chemistry("bazơ", "Bazơ", "Chất có pH > 7, có vị đắng", "NaOH, KOH là bazơ mạnh");
    add_chemistry("muối", "Muối", "Sản phẩm của phản ứng axit + bazơ", "NaCl là muối ăn");
    add_chemistry("cân bằng hóa học", "Kc", "Trạng thái phản ứng thuận-nghịch", "H₂ + I₂ ⇄ 2HI");
    add_chemistry("phản ứng oxy hóa khử", "redox", "Phản ứng trao đổi electron", "Fe + CuSO₄ → FeSO₄ + Cu");
}

void load_chemistry_knowledge() {
    init_chemistry();
}

// ============================================
//  PHÁT HIỆN CÂU HỎI HÓA HỌC
// ============================================

int is_chemistry_question(const char *input) {
    char lower[1000];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    const char *keywords[] = {
        "hóa học", "hóa", "nguyên tố", "hợp chất", "công thức",
        "phản ứng", "axit", "bazơ", "muối", "ph", "dung dịch",
        "kim loại", "phi kim", "khí", "nước"
    };
    
    for (int i = 0; i < 15; i++) {
        if (strstr(lower, keywords[i]) != NULL) {
            return 1;
        }
    }
    
    // Ký hiệu hóa học
    const char *symbols[] = {"h", "o", "c", "n", "na", "cl", "fe", "au", "ag", "he",
                             "h2o", "nacl", "hcl", "h2so4", "hno3", "co2", "nh3", "ch4"};
    for (int i = 0; i < 18; i++) {
        if (strstr(lower, symbols[i]) != NULL) {
            return 1;
        }
    }
    
    return 0;
}

// ============================================
//  TRẢ LỜI CÂU HỎI HÓA HỌC
// ============================================

char* answer_chemistry_question(const char *input) {
    static char answer[4096];
    answer[0] = '\0';
    
    if (!is_chemistry_question(input)) {
        return NULL;
    }
    
    char keyword[128];
    keyword[0] = '\0';
    
    char lower[1000];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    // Tìm từ khóa
    const char *patterns[] = {"công thức", "nguyên tố", "hợp chất", "hóa học"};
    for (int i = 0; i < 4; i++) {
        char *pos = strstr(lower, patterns[i]);
        if (pos) {
            pos += strlen(patterns[i]);
            while (*pos == ' ') pos++;
            int j = 0;
            while (pos[j] != ' ' && pos[j] != '\0' && pos[j] != '.' && pos[j] != '?' && j < 127) {
                keyword[j] = pos[j];
                j++;
            }
            keyword[j] = '\0';
            if (strlen(keyword) > 0) break;
        }
    }
    
    // Nếu không tìm thấy, lấy từ cuối
    if (strlen(keyword) == 0) {
        char *last = strrchr(lower, ' ');
        if (last) {
            strcpy(keyword, last + 1);
        } else {
            strcpy(keyword, lower);
        }
    }
    
    // Tìm kiến thức
    char *info = get_chemistry_info(keyword);
    if (info) {
        return info;
    }
    
    // Nếu không tìm thấy
    snprintf(answer, sizeof(answer),
             "🧪 Tôi chưa có kiến thức hóa học về '%s'!\n"
             "💡 Nhưng tôi biết:\n"
             "   - Nguyên tố: H, O, C, N, Na, Fe, Au, Ag\n"
             "   - Hợp chất: H₂O, NaCl, HCl, H₂SO₄, CO₂, NH₃\n"
             "   - Khái niệm: axit, bazơ, muối, pH\n"
             "🔍 Hãy hỏi cụ thể: 'công thức nước'",
             keyword);
    return answer;
}

char* get_chemistry_info(const char *keyword) {
    static char result[4096];
    result[0] = '\0';
    
    if (strlen(keyword) == 0) return NULL;
    
    for (int i = 0; i < chemistry_count; i++) {
        if (strcasestr(chemistry[i].name, keyword) != NULL || 
            strcasestr(keyword, chemistry[i].name) != NULL ||
            strcasestr(chemistry[i].formula, keyword) != NULL) {
            
            snprintf(result, sizeof(result),
                     "╔══════════════════════════════════════════════════╗\n"
                     "║  🧪 %-43s ║\n"
                     "╠══════════════════════════════════════════════════╣\n"
                     "║  🔢 %-43s ║\n"
                     "║  📖 %-43s ║\n"
                     "║  💡 %-43s ║\n"
                     "╚══════════════════════════════════════════════════╝",
                     chemistry[i].name,
                     chemistry[i].formula,
                     chemistry[i].description,
                     chemistry[i].example);
            return result;
        }
    }
    
    return NULL;
}

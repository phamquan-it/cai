#include "physics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_FORMULAS 50

static PhysicsFormula formulas[MAX_FORMULAS];
static int formula_count = 0;

// ============================================
//  KHO CÔNG THỨC VẬT LÝ (cơ bản)
// ============================================

void add_physics_formula(const char *name, const char *formula, 
                         const char *desc, const char *example) {
    if (formula_count >= MAX_FORMULAS) return;
    strcpy(formulas[formula_count].name, name);
    strcpy(formulas[formula_count].formula, formula);
    strcpy(formulas[formula_count].description, desc);
    strcpy(formulas[formula_count].example, example ? example : "Không có ví dụ");
    formula_count++;
}

static void init_physics() {
    if (formula_count > 0) return;
    
    // Cơ học
    add_physics_formula(
        "định luật 2 newton",
        "F = m × a",
        "Lực bằng khối lượng nhân với gia tốc",
        "Một vật 2kg tăng tốc 3m/s² cần lực 6N"
    );
    
    add_physics_formula(
        "công",
        "A = F × s × cos(α)",
        "Công bằng lực nhân quãng đường nhân cos góc",
        "Kéo vật 10m với lực 5N, góc 0° → A = 50J"
    );
    
    add_physics_formula(
        "động năng",
        "Wđ = ½ × m × v²",
        "Động năng bằng một nửa khối lượng nhân bình phương vận tốc",
        "Xe 1000kg chạy 20m/s có Wđ = 200,000J"
    );
    
    add_physics_formula(
        "thế năng",
        "Wt = m × g × h",
        "Thế năng bằng khối lượng nhân gia tốc trọng trường nhân độ cao",
        "Vật 5kg ở độ cao 10m có Wt = 500J (g=10m/s²)"
    );
    
    add_physics_formula(
        "bảo toàn cơ năng",
        "W = Wđ + Wt = const",
        "Cơ năng bảo toàn khi không có ma sát",
        "Con lắc đơn: thế năng chuyển thành động năng"
    );
    
    add_physics_formula(
        "vận tốc",
        "v = s / t",
        "Vận tốc bằng quãng đường chia thời gian",
        "Đi 100m trong 10s → v = 10m/s"
    );
    
    add_physics_formula(
        "gia tốc",
        "a = Δv / Δt",
        "Gia tốc bằng độ thay đổi vận tốc chia thời gian",
        "Tăng tốc từ 0 lên 20m/s trong 5s → a = 4m/s²"
    );
    
    add_physics_formula(
        "định luật vạn vật hấp dẫn",
        "F = G × (m₁ × m₂) / r²",
        "Lực hấp dẫn tỉ lệ thuận với tích khối lượng, tỉ lệ nghịch với bình phương khoảng cách",
        "G = 6.674×10⁻¹¹ N⋅m²/kg²"
    );
    
    // Nhiệt học
    add_physics_formula(
        "nhiệt lượng",
        "Q = m × c × Δt",
        "Nhiệt lượng bằng khối lượng nhân nhiệt dung riêng nhân độ tăng nhiệt độ",
        "Đun 2kg nước tăng 10°C, c=4200J/kg·K → Q=84,000J"
    );
    
    add_physics_formula(
        "cân bằng nhiệt",
        "Q_tỏa = Q_thu",
        "Nhiệt lượng tỏa ra bằng nhiệt lượng thu vào",
        "Thả đồng nóng vào nước lạnh"
    );
    
    // Điện học
    add_physics_formula(
        "định luật ohm",
        "I = U / R",
        "Cường độ dòng điện bằng hiệu điện thế chia điện trở",
        "U=12V, R=4Ω → I=3A"
    );
    
    add_physics_formula(
        "công suất điện",
        "P = U × I",
        "Công suất bằng hiệu điện thế nhân cường độ dòng điện",
        "Bóng đèn 220V, 0.5A → P=110W"
    );
    
    add_physics_formula(
        "điện năng",
        "A = P × t",
        "Điện năng bằng công suất nhân thời gian",
        "Bóng đèn 100W dùng 5h → A=0.5kWh"
    );
    
    add_physics_formula(
        "điện trở",
        "R = ρ × l / S",
        "Điện trở bằng điện trở suất nhân chiều dài chia tiết diện",
        "Dây đồng dài 10m, S=1mm² → R≈0.17Ω"
    );
    
    // Quang học
    add_physics_formula(
        "định luật khúc xạ",
        "n₁ × sin(i) = n₂ × sin(r)",
        "Tia sáng bị gãy khi qua mặt phân cách hai môi trường",
        "Ánh sáng từ không khí (n=1) vào nước (n=1.33)"
    );
    
    add_physics_formula(
        "công thức thấu kính",
        "1/f = 1/d + 1/d'",
        "Tiêu cự liên hệ với khoảng cách vật và ảnh",
        "Thấu kính hội tụ f=10cm, vật d=20cm → d'=20cm"
    );
    
    add_physics_formula(
        "số phóng đại",
        "k = -d'/d",
        "Độ phóng đại bằng tỉ số khoảng cách ảnh và vật",
        "d'=20cm, d=10cm → k=2 (ảnh lớn gấp 2)"
    );
    
    // Sóng
    add_physics_formula(
        "vận tốc sóng",
        "v = λ × f",
        "Vận tốc sóng bằng bước sóng nhân tần số",
        "Sóng có λ=2m, f=50Hz → v=100m/s"
    );
    
    // Lượng tử
    add_physics_formula(
        "năng lượng photon",
        "E = h × f",
        "Năng lượng photon bằng hằng số Planck nhân tần số",
        "h = 6.626×10⁻³⁴ J·s"
    );
    
    // Tương đối
    add_physics_formula(
        "e=mc²",
        "E = m × c²",
        "Năng lượng bằng khối lượng nhân bình phương tốc độ ánh sáng",
        "c = 3×10⁸ m/s"
    );
    
    add_physics_formula(
        "thuyết tương đối hẹp",
        "Δt' = Δt / √(1 - v²/c²)",
        "Thời gian chậm lại khi chuyển động nhanh",
        "Vận tốc càng gần c, thời gian càng chậm"
    );
}

void load_physics_formulas() {
    init_physics();
}

// ============================================
//  PHÁT HIỆN CÂU HỎI VẬT LÝ
// ============================================

int is_physics_question(const char *input) {
    char lower[1000];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    const char *physics_keywords[] = {
        "vật lý", "công thức", "định luật", "lực", "vận tốc", 
        "gia tốc", "khối lượng", "năng lượng", "động năng", 
        "thế năng", "nhiệt", "điện", "ohm", "sóng", "photon",
        "tương đối", "hấp dẫn", "cơ năng", "công"
    };
    
    for (int i = 0; i < 20; i++) {
        if (strstr(lower, physics_keywords[i]) != NULL) {
            return 1;
        }
    }
    
    if (strstr(lower, "công thức") != NULL || 
        strstr(lower, "định luật") != NULL) {
        return 1;
    }
    
    return 0;
}

// ============================================
//  TÌM CÔNG THỨC
// ============================================

char* get_formula(const char *keyword) {
    static char result[1024];
    result[0] = '\0';
    
    if (strlen(keyword) == 0) return NULL;
    
    for (int i = 0; i < formula_count; i++) {
        if (strcasestr(formulas[i].name, keyword) != NULL) {
            snprintf(result, sizeof(result),
                     "╔═══════════════════════════════════════════╗\n"
                     "║  📐 %-33s ║\n"
                     "╠═══════════════════════════════════════════╣\n"
                     "║  🔢 %-34s ║\n"
                     "║  📖 %-34s ║\n"
                     "║  💡 %-34s ║\n"
                     "╚═══════════════════════════════════════════╝",
                     formulas[i].name,
                     formulas[i].formula,
                     formulas[i].description,
                     formulas[i].example);
            return result;
        }
    }
    
    for (int i = 0; i < formula_count; i++) {
        if (strcasestr(formulas[i].formula, keyword) != NULL) {
            snprintf(result, sizeof(result),
                     "╔═══════════════════════════════════════════╗\n"
                     "║  📐 %-33s ║\n"
                     "╠═══════════════════════════════════════════╣\n"
                     "║  🔢 %-34s ║\n"
                     "║  📖 %-34s ║\n"
                     "║  💡 %-34s ║\n"
                     "╚═══════════════════════════════════════════╝",
                     formulas[i].name,
                     formulas[i].formula,
                     formulas[i].description,
                     formulas[i].example);
            return result;
        }
    }
    
    return NULL;
}

// ============================================
//  TRẢ LỜI CÂU HỎI VẬT LÝ
// ============================================

char* answer_physics_question(const char *input) {
    static char answer[2048];
    answer[0] = '\0';
    
    if (!is_physics_question(input)) {
        return NULL;
    }
    
    char keyword[128];
    keyword[0] = '\0';
    
    char lower[1000];
    strcpy(lower, input);
    for (int i = 0; lower[i]; i++) {
        lower[i] = tolower(lower[i]);
    }
    
    const char *patterns[] = {"công thức", "định luật", "vật lý"};
    for (int i = 0; i < 3; i++) {
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
    
    if (strlen(keyword) == 0) {
        char *last = strrchr(lower, ' ');
        if (last) {
            strcpy(keyword, last + 1);
        } else {
            strcpy(keyword, lower);
        }
    }
    
    char *formula = get_formula(keyword);
    if (formula) {
        return formula;
    }
    
    snprintf(answer, sizeof(answer),
             "🧪 Khoa học vẫn chưa có công thức cho '%s'!\n"
             "📚 Nhưng tôi có những công thức này:\n"
             "   - Định luật 2 Newton: F = m × a\n"
             "   - Động năng: Wđ = ½ × m × v²\n"
             "   - Định luật Ohm: I = U / R\n"
             "   - E = mc² (thuyết tương đối)\n"
             "   - Nhiệt lượng: Q = m × c × Δt\n"
             "🔍 Hãy hỏi 'công thức [tên]' để biết chi tiết!",
             keyword);
    return answer;
}

================================================================================
                    DICTIONARY CLASS - CRUD + SEARCH
                    ================================
================================================================================

MỤC LỤC
--------
1. Giới thiệu
2. Cài đặt
3. Các phương thức
4. Ví dụ sử dụng
5. Yêu cầu hệ thống
6. License


================================================================================
1. GIỚI THIỆU
================================================================================

Một class C++ đơn giản để quản lý từ điển với đầy đủ chức năng CRUD 
(Create, Read, Update, Delete) và tìm kiếm linh hoạt.

Tính năng:
  ✅ Thêm mới entry
  ✅ Đọc/ tra cứu theo key
  ✅ Cập nhật value
  ✅ Xóa entry
  ✅ Tìm kiếm theo key
  ✅ Tìm kiếm theo value
  ✅ Tìm kiếm cả key và value
  ✅ In toàn bộ từ điển
  ✅ Kiểm tra tồn tại


================================================================================
2. CÀI ĐẶT
================================================================================

2.1. Tạo các file cần thiết:
--------------------------------
Tạo 3 file: Dictionary.hpp, Dictionary.cpp, main.cpp

2.2. Biên dịch:
----------------
g++ -std=c++11 -o dictionary main.cpp Dictionary.cpp

2.3. Chạy chương trình:
------------------------
./dictionary


================================================================================
3. CÁC PHƯƠNG THỨC
================================================================================

3.1. CREATE - Thêm dữ liệu
--------------------------------
bool add(const string& key, const string& value);
  - Thêm mới 1 entry
  - Trả về false nếu key đã tồn tại

bool addOrUpdate(const string& key, const string& value);
  - Thêm mới nếu key chưa tồn tại
  - Cập nhật nếu key đã tồn tại


3.2. READ - Đọc dữ liệu
---------------------------
string get(const string& key) const;
  - Lấy value theo key
  - Ném exception nếu key không tồn tại

bool exists(const string& key) const;
  - Kiểm tra key có tồn tại không

int size() const;
  - Trả về số lượng entries

bool isEmpty() const;
  - Kiểm tra từ điển có rỗng không

vector<Entry> getAll() const;
  - Lấy tất cả entries


3.3. UPDATE - Cập nhật
-------------------------
bool update(const string& key, const string& newValue);
  - Cập nhật value cho key
  - Trả về false nếu key không tồn tại


3.4. DELETE - Xóa dữ liệu
----------------------------
bool remove(const string& key);
  - Xóa entry theo key
  - Trả về false nếu key không tồn tại

void clear();
  - Xóa tất cả entries


3.5. SEARCH - Tìm kiếm
-------------------------
vector<string> searchByKey(const string& keyword) const;
  - Tìm các key chứa keyword (không phân biệt hoa thường)

vector<string> searchByValue(const string& keyword) const;
  - Tìm các value chứa keyword (không phân biệt hoa thường)

vector<Entry> search(const string& keyword) const;
  - Tìm trong cả key và value
  - Trả về vector<Entry> đầy đủ


3.6. UTILITY - Tiện ích
--------------------------
void printAll() const;
  - In toàn bộ từ điển ra console với định dạng đẹp

vector<Entry> getEntries() const;
  - Lấy tất cả entries (tương tự getAll())


================================================================================
4. VÍ DỤ SỬ DỤNG
================================================================================

4.1. Ví dụ đơn giản:
---------------------
#include "Dictionary.hpp"
#include <iostream>

int main() {
    Dictionary dict;
    
    // Thêm dữ liệu
    dict.add("apple", "A red or green fruit");
    dict.add("book", "A set of written pages");
    dict.add("computer", "An electronic device");
    
    // Đọc dữ liệu
    cout << dict.get("apple") << endl;  // A red or green fruit
    cout << "Size: " << dict.size() << endl;  // 3
    
    // Cập nhật
    dict.update("apple", "A sweet fruit");
    
    // Tìm kiếm
    auto results = dict.search("fruit");
    for (const auto& entry : results) {
        cout << entry.key << ": " << entry.value << endl;
    }
    
    // Xóa
    dict.remove("computer");
    dict.printAll();
    
    return 0;
}


4.2. Ví dụ đầy đủ:
--------------------
Xem file main.cpp để biết demo chi tiết với tất cả chức năng.


================================================================================
5. YÊU CẦU HỆ THỐNG
================================================================================

  - Compiler: C++11 trở lên
  - Standard Library: 
    * <string>
    * <vector>
    * <unordered_map>
    * <algorithm>
    * <stdexcept>
    * <iostream>
    * <iomanip>

Hỗ trợ trên tất cả các hệ điều hành: Linux, macOS, Windows (MinGW/Cygwin)


================================================================================
6. CẤU TRÚC DỮ LIỆU
================================================================================

  Entry:     struct { string key; string value; }
  Storage:   vector<Entry> entries          (lưu theo thứ tự)
  Index:     unordered_map<string, int>     (key -> vị trí trong vector)

Ưu điểm:
  ✅ Duy trì thứ tự thêm vào
  ✅ Tra cứu nhanh O(1) bằng hash map
  ✅ Dễ dàng tìm kiếm và duyệt


================================================================================
7. CÁC TRƯỜNG HỢP SỬ DỤNG
================================================================================

  | Tình huống              | Phương thức dùng      |
  |-------------------------|-----------------------|
  | Thêm từ mới             | add()                 |
  | Thêm hoặc sửa           | addOrUpdate()         |
  | Tra cứu nghĩa           | get()                 |
  | Kiểm tra từ tồn tại     | exists()              |
  | Sửa nghĩa               | update()              |
  | Xóa từ                  | remove()              |
  | Tìm từ theo key         | searchByKey()         |
  | Tìm từ theo nghĩa       | searchByValue()       |
  | Tìm tất cả              | search()              |
  | In ra console           | printAll()            |
  | Xóa toàn bộ             | clear()               |


================================================================================
8. VÍ DỤ OUTPUT
================================================================================

========== DICTIONARY ==========
KEY                  VALUE                          
---------------------------------
apple                A red or green fruit           
book                 A set of written pages         
computer             An electronic device           
=================================
Total: 3 entries


================================================================================
9. LICENSE
================================================================================

MIT License - Free to use and modify!


================================================================================
10. CONTRIBUTING
================================================================================

Pull requests welcome! Các tính năng có thể thêm:
  - Save/Load to file
  - Sort entries by key or value
  - Auto-complete suggestions
  - Frequency counter
  - Export to CSV/JSON


================================================================================
11. LIÊN HỆ
================================================================================

Báo lỗi hoặc góp ý: GitHub Issues
Star ⭐ nếu thấy hữu ích!

================================================================================
                    CẢM ƠN BẠN ĐÃ SỬ DỤNG!
================================================================================

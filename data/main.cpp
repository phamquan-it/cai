#include <iostream>
#include <map>
#include <string>

using namespace std;

int main() {
    map<string, int> diem;  // Không cần std::
    string ten = "An";
    diem[ten] = 9;
    cout << "Điểm: " << diem[ten] << endl;
    return 0;
}

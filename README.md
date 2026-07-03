🧠 CAI - Cognitive Artificial Intelligence

[![GitHub repo](https://img.shields.io/badge/GitHub-phamquan--it/cai-blue?logo=github)](https://github.com/phamquan-it/cai)
[![Language: C](https://img.shields.io/badge/Language-C-00599C?logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](http://makeapullrequest.com)

CAI (Cognitive Artificial Intelligence) là một chatbot AI đơn giản, được viết hoàn toàn bằng ngôn ngữ C thuần, sử dụng thuật toán Markov Chain để học hỏi và sinh câu trả lời. Dự án này là một ví dụ về cách xây dựng một mô hình AI nhẹ, không phụ thuộc vào bất kỳ thư viện bên ngoài nào.

✨ Tính năng nổi bật
--------------------
  ✅ 100% C thuần: Không sử dụng thư viện ngoài, chỉ dùng thư viện chuẩn của C.
  🧠 Học từ hội thoại: AI có thể ghi nhớ và học hỏi từ chính câu nói của người dùng.
  💬 Sinh câu trả lời: Dựa trên chuỗi Markov để tạo ra câu trả lời từ một từ khóa.
  ⚡ Hiệu năng cao: Nhẹ và chạy nhanh nhờ được viết bằng C.
  📚 Dễ dàng mở rộng: Bạn có thể thêm dữ liệu huấn luyện dễ dàng qua file data/knowledge.txt.

🚀 Bắt đầu nhanh
-----------------
  1. Clone dự án
     git clone https://github.com/phamquan-it/cai.git
     cd cai

  2. Biên dịch (yêu cầu có gcc và make)
     make

  3. Chạy chương trình
     ./bin/cai

📖 Cách sử dụng
---------------
Sau khi khởi chạy, bạn có thể bắt đầu trò chuyện với AI. Chương trình hỗ trợ các lệnh đặc biệt:

  /help     Hiển thị danh sách các lệnh.
  /stats    Xem thống kê của "bộ não" AI.
  /clear    Xóa sạch màn hình terminal.
  /exit     Thoát chương trình.

Ví dụ hội thoại:
  You: hello
  🤖 CAI: hello i am cai an artificial intelligence.

  You: machine learning
  🤖 CAI: machine learning is fun and powerful.

📂 Cấu trúc dự án và danh sách file
------------------------------------
Dưới đây là cấu trúc thư mục và danh sách các file chính, kèm đường dẫn trực tiếp đến mã nguồn trên GitHub để bạn dễ dàng xem xét.

src/ - Thư mục chứa mã nguồn chính
  main.c      - Điểm bắt đầu chương trình. Xử lý giao diện dòng lệnh (CLI) và vòng lặp chính.
                https://github.com/phamquan-it/cai/blob/main/src/main.c

  brain.c     - Lớp bọc cho AI. Khởi tạo, huấn luyện và quản lý trạng thái của chatbot.
  brain.h     - Header file cho brain.c
                https://github.com/phamquan-it/cai/blob/main/src/brain.c
                https://github.com/phamquan-it/cai/blob/main/src/brain.h

  markov.c    - Lõi thuật toán Markov Chain. Xử lý việc học (xây dựng bảng từ) và sinh câu.
  markov.h    - Header file cho markov.c
                https://github.com/phamquan-it/cai/blob/main/src/markov.c
                https://github.com/phamquan-it/cai/blob/main/src/markov.h

  utils.c     - Các hàm tiện ích dùng chung như xử lý chuỗi, xóa màn hình,...
  utils.h     - Header file cho utils.c
                https://github.com/phamquan-it/cai/blob/main/src/utils.c
                https://github.com/phamquan-it/cai/blob/main/src/utils.h

data/ - Thư mục chứa dữ liệu
  knowledge.txt - File chứa các câu mẫu để huấn luyện AI ban đầu. Bạn có thể thêm/sửa nội dung.
                https://github.com/phamquan-it/cai/blob/main/data/knowledge.txt

Các file khác
  Makefile    - File dùng để biên dịch dự án với lệnh make.
                https://github.com/phamquan-it/cai/blob/main/Makefile

  .gitignore  - Liệt kê các file và thư mục không cần đẩy lên Git.
                https://github.com/phamquan-it/cai/blob/main/.gitignore

  README.md   - File giới thiệu chính mà bạn đang đọc.
                https://github.com/phamquan-it/cai/blob/main/README.md

🛠️ Các lệnh Makefile
---------------------
  make         - Biên dịch và tạo thực thi bin/cai
  make clean   - Xóa các file đã biên dịch (trong build/ và bin/)
  make run     - Biên dịch và chạy chương trình
  make debug   - Biên dịch với cờ gỡ lỗi -g và chạy gdb

📊 Cách thức hoạt động
----------------------
  1. Huấn luyện (Training):
     Chương trình đọc các câu trong file data/knowledge.txt. Mỗi câu được tách thành các từ và tạo thành một bản đồ Markov (Markov Chain), ghi nhận mối quan hệ giữa các từ đứng sau nó.

  2. Học hỏi (Learning):
     Khi bạn nhập một câu dài, AI sẽ tự động học từ câu đó, bổ sung vào bản đồ Markov hiện tại.

  3. Sinh câu trả lời (Generation):
     Khi bạn hỏi một câu, AI sẽ lấy từ đầu tiên làm "từ khóa". Nó sẽ đi ngẫu nhiên trong bản đồ Markov, bắt đầu từ từ khóa đó, để tạo thành một câu trả lời mới.

🤝 Đóng góp
-----------
Dự án được xây dựng với mục đích học tập. Mọi ý kiến đóng góp, báo lỗi hay pull request đều được chào đón!

  1. Fork dự án
  2. Tạo nhánh tính năng của bạn (git checkout -b feature/AmazingFeature)
  3. Commit thay đổi (git commit -m 'Add some AmazingFeature')
  4. Push lên nhánh (git push origin feature/AmazingFeature)
  5. Mở một Pull Request

📝 Giấy phép
------------
Dự án này được phân phối dưới giấy phép MIT. Xem file LICENSE để biết thêm chi tiết.

---
Made with ❤️ and pure C

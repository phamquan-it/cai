# 🧠 CAI - Trí Tuệ Nhân Tạo bằng C

> AI chatbot viết bằng C thuần, 100% tiếng Việt

## Tính năng
- ✅ 100% C thuần, không thư viện ngoài
- ✅ Học từ hội thoại
- ✅ Nhớ ngữ cảnh 10 câu gần nhất
- ✅ Sinh câu trả lời bằng Markov Chain
- ✅ 100% tiếng Việt

## Chạy nhanh
```bash
make
./bin/cai
```

Lệnh

· /help - Trợ giúp
· /stats - Thống kê
· /context - Xem ngữ cảnh
· /reset - Xóa ngữ cảnh
· /clear - Xóa màn hình
· /exit - Thoát

Cấu trúc

```
cai/
├── src/
│   ├── main.c
│   ├── brain.c/h
│   ├── markov.c/h
│   ├── context.c/h
│   └── utils.c/h
├── data/
│   └── knowledge.txt
├── Makefile
└── README.md
```


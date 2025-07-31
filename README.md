# 🧠 Trình Tạo Bài Quiz (C++ ➞ HTML)

Đây là một chương trình **C++** giúc chuyển đổi file văn bản chứa câu hỏi trắc nghiệm thành một **biểu mẫu HTML tự chấm điểm**.

---

## ✨ Tính năng

- ✅ Hỗ trợ tiếng Việt và mã hóa UTF-8  
- ✅ Tự động phát hiện đáp án đúng nếu bị lặp lại  
- ✅ Trộn thứ tự các lựa chọn trả lời  
- ✅ Xuất file HTML đẹp, có chấm điểm bằng JavaScript  
- ✅ Hiển thị đúng/sai ngay sau khi nộp bài  
- ✅ Tính điểm theo thang 10

---

## 📄 Cấu trúc file đầu vào (`quiz_input.txt`)

File nhập phải được lưu ở định dạng **UTF-8**, với định dạng như sau:

```
Câu hỏi: Đây là câu hỏi thứ nhất?
A. Đáp án A
B. Đáp án B
C. Đáp án C
B. Đáp án B

Câu hỏi: Câu hỏi thứ hai là gì?
A. Một lựa chọn
B. Một lựa chọn khác
C. Một lựa chọn đúng
C. Một lựa chọn đúng
```

> ⚠️ Lưu ý: Những đáp án nào bị **lặp lại** sẽ được xem là **đáp án đúng**.

---

## 🧪 Kết quả đầu ra

- Tạo file `quiz.html`.
- Mở file bằng trình duyệt để làm bài trắc nghiệm.
- Sau khi bấm nút **"Nộp bài"**, bạn sẽ:
  - Nhận kết quả đúng/sai cho từng câu hỏi
  - Nhận tổng điểm hiển thị bằng cửa sổ thông báo

---

## 🛠️ Cách hoạt động

1. **Đọc và xử lý dòng** từ file văn bản (`quiz_input.txt`)  
2. **Xác định câu hỏi** bắt đầu bằng `Câu hỏi:`  
3. **Phân tích đáp án** bắt đầu bằng `A.`, `B.`,...  
4. **Tìm đáp án đúng** dựa vào các dòng bị lặp  
5. **Trộn lựa chọn** để tránh học vẹt  
6. **Xuất HTML** có JavaScript để chấm điểm trực tiếp

---

## 🧹 Công nghệ sử dụng

- **C++17**: Viết logic xử lý chính  
- **Windows API (`windows.h`)**: Chuyển mã UTF-8 ↔ `std::wstring`  
- **HTML + JavaScript**: Tạo giao diện và xử lý chấm điểm

---

## 🚀 Cách chạy chương trình

1. Cài đặt trình biên dịch C++ hỗ trợ chuẩn C++17 (g++ hoặc MSVC)  
2. Tạo file `quiz_input.txt` với các câu hỏi như hướng dẫn  
3. Biên dịch và chạy chương trình:

```bash
g++ -std=c++17 quiz_generator.cpp -o quiz.exe
./quiz.exe
```

4. Mở file `quiz.html` vừa tạo bằng trình duyệt bất kỳ

---

## 📌 Ghi chú

- Mã nguồn hiện chỉ hoạt động trên **Windows** do dùng `MultiByteToWideChar` và `WideCharToMultiByte`.
- Muốn chạy trên Linux/Mac cần thay thế bằng giải pháp khác (như `iconv` hoặc thư viện `codecvt`)

---

## 📚 Giấy phép

Dự án này là mã nguồn mở. Bạn có thể chỉnh sửa và sử dụng thoái mái cho mục đích học tập hoặc cá nhân.

# BÁO CÁO BÀI TẬP 03: PHÂN TÍCH TRADE-OFF BIỂU THỨC LOGIC GỌP VS BIẾN CỜ PHÂN RÃ TRẠNG THÁI SHOPEEFOOD

> 👤 **Học viên:** Hoàng Công Dũng | **Mã SV:** PTIT-HCM-009
> 🏫 **Môn học:** it102-rl-hoangcongdung

---

## 📊 Sơ đồ thiết kế hệ thống (Flowchart)

> 💡 *Sơ đồ dưới đây được render tự động trực tiếp trên GitHub bằng Mermaid. Bạn cũng có thể tải file **`bt3.drawio`** trong repository này để mở và chỉnh sửa trực tiếp trên [Draw.io (diagrams.net)](https://app.diagrams.net).* 

```mermaid
graph TD
  Start([Bắt đầu]) --> Input[Nhập order_id, subtotal, distance_km, booking_hour, is_store_open, inventory_qty]
  Input --> Guard{Kiểm tra tính hợp lệ đầu vào &
  is_store_open == 1 && inventory_qty > 0 &&
  distance_km > 0.0 && distance_km <= 15.0 &&
  booking_hour >= 0 && booking_hour <= 23 && subtotal >= 0}
  Guard -- Không hợp lệ --> SetZero[final_delivery_fee = 0
total_payment = 0
is_valid_order = 0] --> Print[In kết quả đơn hàng không hợp lệ]
  Guard -- Hợp lệ --> SetValid[is_valid_order = 1
base_fee = 15000]
  SetValid --> PeakCheck[is_peak_hour = (booking_hour >= 11 && booking_hour <= 13) || (booking_hour >= 18 && booking_hour <= 20)]
  PeakCheck --> SurchargeCalc[surcharge = is_peak_hour * 10000]
  SurchargeCalc --> DiscCheck[is_freeship_eligible = (subtotal >= 100000)]
  DiscCheck --> DiscCalc[discount = is_freeship_eligible * 15000]
  DiscCalc --> FeeCalc[final_delivery_fee = base_fee + surcharge - discount]
  FeeCalc --> TotalCalc[total_payment = subtotal + final_delivery_fee]
  TotalCalc --> Print
  Print --> End([Kết thúc])
```

---

## Nhiệm vụ 1: Phân tích Bài toán và Yêu cầu Kỹ thuật

Bài toán yêu cầu xây dựng chương trình C xử lý cước phí và xác thực đơn hàng cho nền tảng ShopeeFood mà tuyệt đối không được sử dụng các cấu trúc điều khiển rẽ nhánh quen thuộc như if, else, switch-case, toán tử ba ngôi hoặc vòng lặp. Toàn bộ logic phải được biểu diễn thông qua đại số mệnh đề, kết hợp khéo léo giữa toán tử quan hệ, toán tử logic (&&, ||, !) và phép nhân số học với các biến cờ (flag).

Hệ thống cần tiếp nhận 6 tham số đầu vào từ terminal gồm mã đơn hàng, giá trị tiền món ăn, khoảng cách giao hàng, giờ đặt hàng, trạng thái mở cửa của quán và số lượng tồn kho. Việc xử lý không dùng lệnh rẽ nhánh bắt buộc lập trình viên phải tư duy theo hướng biến đổi biểu thức toán học, tận dụng tính chất trả về giá trị 1 hoặc 0 của các phép so sánh trong ngôn ngữ C.

- Cước phí cơ bản (base_fee) cố định ở mức 15,000 VNĐ cho các đơn hàng hợp lệ.
- Phụ phí cao điểm (surcharge) cộng thêm 10,000 VNĐ nếu giờ đặt nằm trong khung 11h-13h hoặc 18h-20h.
- Ưu đãi giảm giá (discount) trừ trực tiếp 15,000 VNĐ vào phí ship khi giá trị món ăn từ 100,000 VNĐ trở lên.
- Ràng buộc phòng thủ dữ liệu: Chặn đứng các trường hợp giờ âm, giờ lớn hơn 23, khoảng cách âm hoặc bằng 0, và tiền món ăn âm.

## Nhiệm vụ 2: Đề xuất và So sánh Giải pháp Kỹ thuật (Trade-off Analysis)

Để giải quyết bài toán không dùng rẽ nhánh, tôi đề xuất 2 giải pháp kỹ thuật khác nhau và tiến hành phân tích ưu nhược điểm để tìm ra phương án tối ưu nhất cho hệ thống Core Backend của ShopeeFood.

Giải pháp A (Tách biệt cờ hiệu - Flag-based Arithmetic): Khai báo tường minh các biến trung gian mang giá trị 0 hoặc 1 cho từng quy tắc nghiệp vụ như is_peak_hour, is_freeship_eligible, is_valid_input. Sau đó, áp dụng phép nhân đại số để tính toán chi phí. Giải pháp này giúp code cực kỳ minh bạch, dễ debug từng bước nhỏ.

Giải pháp B (Gộp biểu thức đại số Logic đơn - Single-Expression Inline): Gom toàn bộ điều kiện logic phức tạp vào thẳng dòng lệnh tính toán và in kết quả. Cách này gọn code nhưng làm giảm khả năng đọc hiểu và cực kỳ khó khăn khi cần bảo trì hoặc mở rộng nghiệp vụ.

| Tiêu chí đánh giá | Giải pháp A: Tách biệt cờ hiệu (Flag-based) | Giải pháp B: Gộp biểu thức đại số (Inline) |
| --- | --- | --- |
| Độ đọc hiểu (Readability) | Rất cao nhờ đặt tên biến cờ rõ nghĩa, dễ theo dõi luồng tính toán. | Thấp, biểu thức quá dài gây rối mắt và khó kiểm tra lỗi logic. |
| Khả năng bảo trì (Maintainability) | Dễ dàng thêm bớt quy tắc nghiệp vụ mới bằng cách bổ sung biến cờ. | Khó khăn, dễ làm sai lệch toàn bộ biểu thức nếu sửa nhầm một dấu ngoặc. |
| Khả năng mắc lỗi (Bug-proneness) | Thấp hơn nhờ tách nhỏ các điều kiện kiểm tra biên và kiểu dữ liệu. | Cao do dễ nhầm lẫn độ ưu tiên của toán tử logic và toán tử số học. |
| Dung lượng bộ nhớ | Tốn thêm một vài byte khai báo biến cờ phụ trợ (không đáng kể). | Tối ưu số lượng biến tối đa trên stack. |

## Nhiệm vụ 3: Lựa chọn Giải pháp và Thiết kế Mã nguồn

Sau khi cân nhắc kỹ lưỡng các tiêu chí trong bảng trade-off, tôi quyết định lựa chọn Giải pháp A (Tách biệt cờ hiệu - Flag-based Arithmetic) để triển khai mã nguồn C. Trong môi trường sản xuất của hệ thống lớn như ShopeeFood, tính dễ bảo trì, khả năng đọc hiểu và độ an toàn khi kiểm tra lỗi dữ liệu (Defensive Programming) được đặt lên hàng đầu thay vì cố gắng gom code thành một dòng ngắn gọn nhưng khó kiểm soát.

Để xử lý triệt để lỗi trôi bộ đệm stdin khi người dùng nhập lẫn lộn giữa số nguyên (int) và số thực (float), tôi sử dụng kỹ thuật đọc chuỗi hoặc thêm khoảng trắng trong định dạng scanf('%f ', &distance_km) kết hợp kiểm tra mã trả về của hàm scanf. Các bẫy dữ liệu như khoảng cách âm, giờ vượt quá 24h cũng được bọc chặt trong biểu thức điều kiện logic.

- Sử dụng kiểu dữ liệu chuẩn int cho các biến cờ hiệu, giờ, trạng thái và float cho khoảng cách giao hàng.
- Kiểm tra tính hợp lệ toàn cục thông qua biến is_valid_order trước khi thực hiện mọi phép tính tài chính.
- Đảm bảo tuân thủ tuyệt đối quy tắc CẤM mọi câu lệnh rẽ nhánh if/else hay vòng lặp.

## Nhiệm vụ 4: Hướng dẫn Triển khai và Kiểm tra Biên

Mã nguồn được viết hoàn chỉnh trong file main.c, tuân thủ tiêu chuẩn C11 và biên dịch thành công thông qua trình dịch gcc với các cờ cảnh báo nghiêm ngặt nhằm đảm bảo không có cảnh báo hay lỗi tiềm ẩn.

Phiếu biên lai đơn hàng (Order Receipt) được định dạng rõ ràng trên console, hiển thị tường minh mã đơn hàng, trạng thái hợp lệ, chi tiết cước phí cơ bản, phụ phí giờ cao điểm, mức giảm giá và tổng tiền thanh toán cuối cùng.

---

## 📁 Danh sách tệp tin nộp bài trong Repository
- 📝 `bt3.docx`: Báo cáo tài liệu phân tích nghiệp vụ hoàn chỉnh.
- 🎨 `bt3.drawio`: File thiết kế sơ đồ chuẩn theo quy định đề bài (mở trực tiếp bằng [Draw.io](https://app.diagrams.net) hoặc Lucidchart).
- 💻 `main.c`: Mã nguồn chương trình.

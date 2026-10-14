#include <stdio.h>

int main() {
    int order_id;
    int subtotal;
    float distance_km;
    int booking_hour;
    int is_store_open;
    int inventory_qty;

    printf('=== CHUONG TRINH XAC THUC & TINH PHI SHOPEEFOOD ===\n');
    printf('Nhap ma don hang (order_id): ');
    if (scanf('%d', &order_id) != 1) return 0;

    printf('Nhap gia tri tien mon an (subtotal - VND): ');
    if (scanf('%d', &subtotal) != 1) return 0;

    printf('Nhap khoang cach giao hang (distance_km - km): ');
    if (scanf('%f', &distance_km) != 1) return 0;

    printf('Nhap gio dat hang trong ngay (booking_hour từ 0 den 23): ');
    if (scanf('%d', &booking_hour) != 1) return 0;

    printf('Nhap trang thai quan mo cua (is_store_open: 1 la Mo, 0 la Dong): ');
    if (scanf('%d', &is_store_open) != 1) return 0;

    printf('Nhap so luong ton kho cua mon (inventory_qty): ');
    if (scanf('%d', &inventory_qty) != 1) return 0;

    int is_valid_input = (is_store_open == 1) && 
                         (inventory_qty > 0) && 
                         (distance_km > 0.0f) && 
                         (distance_km <= 15.0f) && 
                         (booking_hour >= 0) && 
                         (booking_hour <= 23) && 
                         (subtotal >= 0);

    int is_valid_order = is_valid_input;

    int base_fee = is_valid_order * 15000;

    int is_peak_hour = ((booking_hour >= 11) && (booking_hour <= 13)) || 
                       ((booking_hour >= 18) && (booking_hour <= 20));
    int surcharge = is_valid_order * is_peak_hour * 10000;

    int is_freeship_eligible = (subtotal >= 100000);
    int discount = is_valid_order * is_freeship_eligible * 15000;

    int final_delivery_fee = base_fee + surcharge - discount;
    int total_payment = subtotal + final_delivery_fee;

    final_delivery_fee = final_delivery_fee * is_valid_order;
    total_payment = total_payment * is_valid_order;

    printf('\n========================================\n');
    printf('          PHIEU BIEN LAI SHOPEEFOOD     \n');
    printf('========================================\n');
    printf('Ma don hang          : %d\n', order_id);
    printf('Trang thai hop le    : %s\n', is_valid_order ? 'HOP LE (CHAP NHAN)' : 'KHONG HOP LE (TU CHOI)');
    printf('Tien mon an (subtotal): %d VND\n', subtotal);
    printf('Khoang cach giao     : %.1f km\n', distance_km);
    printf('Gio dat hang         : %d h\n', booking_hour);
    printf('Phi ship co bản      : %d VND\n', is_valid_order * 15000);
    printf('Phu phí cao điểm     : %d VND\n', surcharge);
    printf('Uu đãi giảm giá ship : %d VND\n', discount);
    printf('Phi giao hang thuc te: %d VND\n', final_delivery_fee);
    printf('Tong tien thanh toan : %d VND\n', total_payment);
    printf('========================================\n');

    return 0;
}
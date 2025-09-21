# STM32F103 – SPI Master giao tiếp module SPI và UART hiển thị dữ liệu

## 📌 Mô tả
Chương trình sử dụng **STM32F103C8T6** làm **Master SPI** để giao tiếp với module **MAX7219**.  
- STM32 cấu hình SPI1, gửi dữ liệu điều khiển đến MAX7219 để bật/tắt LED ma trận.  
- Dữ liệu trạng thái ("LED ON", "LED OFF") sẽ được gửi về terminal thông qua UART.  
- Chương trình minh họa nguyên tắc: gửi dữ liệu qua SPI và quan sát phản hồi bằng UART.  

---

## 🛠️ Yêu cầu phần cứng
- **Board**: STM32F103C8T6 (Blue Pill).  
- **Module SPI**: MAX7219 LED driver (có thể thay bằng module SPI khác: cảm biến, màn hình, SD-Card…).  
- **USB-TTL** để kết nối UART với máy tính.  
- **Kết nối chân**:  
  - SPI1 (STM32 ↔ MAX7219):  
    - PA5 (SCK)  → CLK  
    - PA7 (MOSI) → DIN  
    - PA6 (MISO) → (nếu có, MAX7219 không dùng)  
    - PA4 (CS)   → CS/LOAD  
  - UART1 (STM32 ↔ PC qua USB-TTL):  
    - PA9 (TX)  → RX USB-TTL  
    - PA10 (RX) → TX USB-TTL  
    - GND       ↔ GND  

---

## ⚙️ Cấu hình

### 1. SPI1 (Master)
- Pin: PA5 (SCK), PA7 (MOSI), PA6 (MISO), PA4 (CS).  
- Mode: SPI Master.  
- Data size: 8 bit.  
- CPOL = 0, CPHA = 0 (Mode 0).  
- Baudrate prescaler: 8.  
- First bit: MSB.  

### 2. MAX7219
- Giao tiếp SPI (chỉ cần MOSI + SCK + CS).  
- Thanh ghi cấu hình:  
  - `0x09` → Decode mode (0x00 = no decode).  
  - `0x0A` → Intensity (0x00–0x0F).  
  - `0x0B` → Scan limit (số digit dùng).  
  - `0x0C` → Shutdown (0x01 = normal).  
  - `0x0F` → Display test (0x00 = off).  

### 3. UART1
- Baudrate: 9600.  
- Word length: 8 bit.  
- Stop bit: 1.  
- Parity: None.  
- Mode: TX.  

---

## 🖥️ Terminal
Mở phần mềm terminal (PuTTY, TeraTerm, RealTerm, …) với cấu hình:  
- Baudrate: 9600  
- Data bits: 8  
- Stop bits: 1  
- Parity: None  

**Kết quả mong đợi**:  
- Terminal hiển thị:  


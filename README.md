# esp32-project-task2
# 🧠 Task 2 – Multitasking ESP32-S3 (FreeRTOS)

## 🎯 Tujuan
Membuat program multitasking untuk masing-masing peripheral pada **ESP32-S3**, di mana setiap perangkat (LED, tombol, buzzer, stepper, OLED, dll.) dijalankan dalam **task terpisah** menggunakan **FreeRTOS**.

---

## 🧩 Komponen yang Digunakan
- ESP32-S3 (WROOM)
- 3x LED
- 2x Push Button
- 1x Buzzer (aktif)
- 1x OLED 0.96" (I2C)
- 1x Rotary Encoder
- 1x Stepper Motor
- (Opsional) Servo

---

## ⚙️ Wiring Diagram (Wokwi)
Semua koneksi sudah didefinisikan di file `diagram.json`.

| Komponen | Pin ESP32-S3 | Keterangan |
|-----------|---------------|------------|
| LED1 | GPIO2 | TaskLED1 |
| LED2 | GPIO4 | TaskLED2 |
| LED3 | GPIO5 | TaskLED3 |
| Tombol 1 | GPIO12 | Start/Stop Buzzer |
| Tombol 2 | GPIO13 | Ganti mode LED |
| Buzzer | GPIO14 | TaskBuzzer |
| OLED SDA | GPIO21 | I2C SDA |
| OLED SCL | GPIO22 | I2C SCL |
| Stepper IN1 | GPIO25 | TaskStepper |
| Stepper IN2 | GPIO26 |  |
| Stepper IN3 | GPIO27 |  |
| Stepper IN4 | GPIO32 |  |

---

## 🧠 Struktur Task

| Task | Fungsi |
|------|---------|
| **TaskLED1** | Menyalakan LED1 secara berkedip setiap 500ms |
| **TaskLED2** | Menyalakan LED2 secara bergantian dengan LED3 |
| **TaskButton** | Membaca input dari tombol dan mengubah mode operasi |
| **TaskBuzzer** | Mengaktifkan buzzer saat tombol ditekan |
| **TaskOLED** | Menampilkan status sistem (mode LED, status buzzer, dsb) |
| **TaskStepper** | Menggerakkan stepper sesuai mode |

---

## 🧰 Struktur File Proyek

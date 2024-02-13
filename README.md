# Tugas Kecil 1 - IF 2211 Strategi Algoritma 2023/2024 - Cyberpunk 2077 Breach Protocol Minigame Solver


## Deskripsi Singkat
Program Cyberpunk 2077 Breach Protocol Minigame Solver bertujuan untuk memberikan solusi dari permasalahan yang serupa dengan minigame Breach Protocol pada video game Cyberpunk 2077
Pencarian solusi dilakukan dengan algoritma Brute Force dan diimplementasikan dalam bahasa C.
Program ini memiliki Graphical User Interface yang dibuat menggunakan library raylib dan diuji pada platform Windows Desktop.

## Requirements
1. raylib untuk kompilasi program (untuk run tidak diperlukan)

## Compilation
Program dapat dicompile dari directory src dengan menggunakan command yang terdapat pada file .txt di folder tersebut
> GUI memiliki keterbatasan dalam ukuran matriks (10 x 10 masih dapat dilihat dengan wajar) dan jumlah sekuens (max 8) yang dapat ditampilkan

## Cara Menggunakan Program
Program dapat digunakan dalam mode GUI dengan menjalankan main.exe pada folder bin dengan current working directory berada pada folder bin,


### Cara menggunakan GUI
1. Pilih metode input (dengan txt atau random input)
2. Untuk input dengan txt file, program akan meminta memasukkan nama file .txt di dalam folder tests
3. Setelah nama file telah diinput klik tombol submit
4. Untuk input random bisa menekan tombol generate random dan input kolom sesuai label
5. Program akan menampilkan matriks dan sekuens beserta bobotnya
6. Klik tombol show solution
7. Program akan menunjukkan alur jalannya solusi dimulai dari token pertama
8. Klik tombol di kanan atas untuk mengeksport hasil ke file txt
9. Input nama file .txt yang diinginkan dan klik submit
10. File .txt hasil dapat dibuka di folder results

Program juga dapat dijalankan dalam mode CLI untuk ukuran matriks atau jumlah sekuens yang tidak wajar untuk ditampilkan dengan menjalankan winCli.exe (untuk windows) atau unixCli (untuk OS unix)
### Cara menggunakan CLI
1. Untuk input dengan txt file langsung dimasukkan dalam run program ex : `winCli.exe 'test2.txt'`
2. Untuk input random dapat dilakukan dengan run program tanpa argumen pada pemanggilannya `winCli.exe`

Abdul Rafi Radityo Hutomo
13522089

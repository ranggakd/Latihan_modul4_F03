# Latihan_modul4_F03
Untuk keperluan latihan dan asistensi daring Sistem Operasi 2019

<center>

![image](cover.gif "FUSE UP")

</center>

---

## NO 1

Buat sebuah file sistem yang kamu buat ke /home/[user]/Downloads, lalu ubah nama file yang ada pada folder tersebut
menjadi [nama file].[ekstensi].bak . File .bak tersebut hanya dapat dibaca.

**Jawaban**

1. Buat program dibawah ini dengan nama file lat1.c di folder yang ditentukan.

```c

```
Penjelasan:
```c

```
Fungsi untuk mencari bilangan factorial dari argumen tipe data arv yang akan di type cast menjadi pointer integer dengan konsep pemrograman dinamis.
```c

```
invalid handling argumen command line yang membutuhkan minimal satu argumen
```c

```
Array integer flag guna menandai bilangan apa saja yang telah dipassing sebagai argumen ke program. While loop berguna untuk melakukan threading program menghitung nilai factorial suatu argumen kemudian dilanjutkan dengan menunggu threading itu mati.
```c

```
While loop untuk menampilkan hasil output sesuai soal berdasarkan tanda array flag.

2. Lalu compile file tadi dengan
```c
gcc -Wall `pkg-config fuse --cflags` lat1.c -o l1 `pkg-config fuse --libs`
```
dan jalankan di terminal

3. Lakukan simulasi test. Hasil akhir bisa dicek di terminal.

---

## NO 2

Buat sebuah file system yang mengarah ke /home/[user]/Documents. Pada saat membuka file dengan ekstensi .pdf,
.doc, .txt pada direktori Documents akan muncul pesan error “Terjadi kesalahan! File berisi konten berbahaya.” dan tidak
dapat membaca file tersebut. Setelah memunculkan pesan error, file tersebut diganti namanya menjadi <namafile>.
<ekstensi>.ditandai . Setelah memunculkan pesan error dan mengganti nama file tadi, file tersebut otomatis
dipindahkan ke direktori rahasia . Jika folder rahasia belum ada, maka secara otomatis akan membuat direktori
“rahasia” sebelum dipindahkan dan file tidak bisa di read write execute.

**Jawaban**

1. Buat program dibawah ini dengan nama file lat2.c di folder yang ditentukan.

```c

```
Penjelasan:
```c

```
Fungsi untuk mencari bilangan factorial dari argumen tipe data arv yang akan di type cast menjadi pointer integer dengan konsep pemrograman dinamis.
```c

```
invalid handling argumen command line yang membutuhkan minimal satu argumen
```c

```
Array integer flag guna menandai bilangan apa saja yang telah dipassing sebagai argumen ke program. While loop berguna untuk melakukan threading program menghitung nilai factorial suatu argumen kemudian dilanjutkan dengan menunggu threading itu mati.
```c

```
While loop untuk menampilkan hasil output sesuai soal berdasarkan tanda array flag.

2. Lalu compile file tadi dengan
```c
gcc -Wall `pkg-config fuse --cflags` lat2.c -o l2 `pkg-config fuse --libs`
```
dan jalankan di terminal

3. Lakukan simulasi test. Hasil akhir bisa dicek di terminal.

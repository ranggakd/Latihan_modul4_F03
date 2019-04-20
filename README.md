# Latihan_modul4_F03
Untuk keperluan latihan dan asistensi daring Sistem Operasi 2019

<center>

![image](cover.gif "FUSE UP")

</center>

---

## NO 1

Buatlah program C yang bisa menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan  
Contoh:  
	./faktorial 5 3 4  
	3! = 6  
	4! = 24  
	5! = 120

**Jawaban**

1. Buat program dibawah ini dengan nama file soal1.c di folder yang ditentukan.

```c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#define llu unsigned long long

pthread_t tid[50];
llu result[50] = {0};

void* fact_dp(void* arv);

int main(int argc, char* argv[])
{
    //invalid cmd argument
    if (argc <2)
    {
        printf("minimal satu argumen\n");
        exit(1);
    }
    //processing factorial by threading
    int flag[50] = {0};
    int c=0;
    int* arg=malloc(sizeof(int*));
    while(argc>1)
    {
        *arg=(atoi(argv[c+1]));
        pthread_create(&tid[c],NULL,fact_dp,(void*)arg);
        flag[*arg]=1;
        argc--;
        c++;
        pthread_join(tid[c-1],NULL);
        //join until any thread eliminate
    }
    int l=0;
    //display the output from array sort asc
    while(l<50)
    {
        if (flag[l] != 0)
            printf("%d! = %llu\n",l,result[l]);
        l++;
    }
    return 0;
}

void* fact_dp(void* arv)
{
    int *n = (int*)arv;
    // printf("n=%d\n",*n);
    if (*n >= 0) 
    {
        if (result[*n] != 0)
            return NULL;
        else
        {
            result[0] = 1;
            for (int i = 1; i <= *n; ++i) 
                result[i] = i * result[i - 1];
            return NULL;
        }
    }
}
```
Penjelasan:
```c
void* fact_dp(void* arv)
{
    int *n = (int*)arv;
    // printf("n=%d\n",*n);
    if (*n >= 0) 
    {
        if (result[*n] != 0)
            return NULL;
        else
        {
            result[0] = 1;
            for (int i = 1; i <= *n; ++i) 
                result[i] = i * result[i - 1];
            return NULL;
        }
    }
}
```
Fungsi untuk mencari bilangan factorial dari argumen tipe data arv yang akan di type cast menjadi pointer integer dengan konsep pemrograman dinamis.
```c
//invalid cmd argument
if (argc <2)
{
    printf("minimal satu argumen\n");
    exit(1);
}
```
invalid handling argumen command line yang membutuhkan minimal satu argumen
```c
//processing factorial by threading
int flag[50] = {0};
int c=0;
int* arg=malloc(sizeof(int*));
while(argc>1)
{
    *arg=(atoi(argv[c+1]));
    pthread_create(&tid[c],NULL,fact_dp,(void*)arg);
    flag[*arg]=1;
    argc--;
    c++;
    pthread_join(tid[c-1],NULL);
    //join until any thread eliminate
}
```
Array integer flag guna menandai bilangan apa saja yang telah dipassing sebagai argumen ke program. While loop berguna untuk melakukan threading program menghitung nilai factorial suatu argumen kemudian dilanjutkan dengan menunggu threading itu mati.
```c
int l=0;
//display the output from array sort asc
while(l<50)
{
    if (flag[l] != 0)
        printf("%d! = %llu\n",l,result[l]);
    l++;
}
return 0;
```
While loop untuk menampilkan hasil output sesuai soal berdasarkan tanda array flag.

2. Lalu compile file tadi dengan `-pthread` dan jalankan di terminal

3. Lakukan simulasi test. Hasil akhir bisa dicek di terminal.

---

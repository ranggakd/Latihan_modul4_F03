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
#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static const char *dirpath = "/home/rak/Downloads";

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
    res = lstat(fpath, stbuf);
    if (res == -1)
        return -errno;
    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);
    int res = 0;
    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;
    dp = opendir(fpath);
    if (dp == NULL)
        return -errno;
    while ((de = readdir(dp)) != NULL)
    {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        res = (filler(buf, de->d_name, &st, 0));
        if(res!=0) break;
    }
    closedir(dp);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
struct fuse_file_info *fi)
{
    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);
    int res = 0;
    int fd = 0 ;
    (void) fi;
    fd = open(fpath, O_RDONLY);
    if (fd == -1)
        return -errno;
    res = pread(fd, buf, size, offset);
    if (res == -1)
        res = -errno;
    close(fd);
    return res;
}

static int xmp_rename(const char *src, const char *dst)
{
    char fpath[1000];
    char fdest[1000];
    if(strcmp(src,"/") == 0)
    {
        src=dirpath;
        sprintf(fpath,"%s",src);
	    sprintf(fdest,"%s",dst);
    }
    else
    {
        sprintf(fpath, "%s%s",dirpath,src);
        sprintf(fdest, "%s%s",dirpath,dst);
    }
    int res = rename(fpath,fdest);
    if (res == -1)
        return -errno;
    return 0;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .rename = xmp_rename,
};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}

```
Penjelasan:
```c
static const char *dirpath = "/home/rak/Downloads";
```
Direktori yang diingin soal.
```c
static int xmp_rename(const char *src, const char *dst)
{
    char fpath[1000];
    char fdest[1000];
    if(strcmp(src,"/") == 0)
    {
        src=dirpath;
        sprintf(fpath,"%s",src);
	    sprintf(fdest,"%s",dst);
    }
    else
    {
        sprintf(fpath, "%s%s",dirpath,src);
        sprintf(fdest, "%s%s",dirpath,dst);
    }
    int res = rename(fpath,fdest);
    if (res == -1)
        return -errno;
    return 0;
}
```
Fungsi yang didefinisikan untuk diassign ke fungsi built-in rename.
```c
static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .rename = xmp_rename,
};
```
Penamabahan rename pada fuse operation. Selain itu, merupakan template dari modul.

2. Lalu compile file tadi dengan
```c
gcc -Wall `pkg-config fuse --cflags` lat1.c -o l1 `pkg-config fuse --libs`
```
dan jalankan di terminal

3. Lakukan simulasi test dengan terlebih dahulu ada file apapun di direktori `/home/rak/Downloads` , lalu pindah direktori ke direktori yang dimount melalui GUI.

4. Lakukan rename file apapun di direktori yang dimount dengan menambahi .bak, maka file tersebut hanya dapat di read.

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
#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>

static const char *dirpath = "/home/rak/Documents";

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];
    sprintf(fpath,"%s%s",dirpath,path);
    res = lstat(fpath, stbuf);
    if (res == -1)
        return -errno;
    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);
    int res = 0;
    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;
    dp = opendir(fpath);
    if (dp == NULL)
        return -errno;
    while ((de = readdir(dp)) != NULL)
    {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        res = (filler(buf, de->d_name, &st, 0));
        if(res!=0) break;
    }
    closedir(dp);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
struct fuse_file_info *fi)
{
    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);
    int res = 0;
    int fd = 0 ;
    (void) fi;
    //aplikasi open
    fd = open(fpath, O_RDONLY);
    if (fd == -1)
        return -errno;
    res = pread(fd, buf, size, offset);
    if (res == -1)
        res = -errno;
    close(fd);
    return res;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    //cek yang diopen sesuai soal tidak
    char *point;
    int flag = 0;
    if((point = strrchr(path,'.')) != NULL )
    {
        if(strcmp(point,".pdf") == 0 || strcmp(point,".doc") == 0 || strcmp(point,".txt") == 0)
        {
            system("zenity --error --text='Terjadi kesalahan\! File berisi konten berbahaya\.' --title='Pesan Error'");
            flag = 1;
        }
    }
    char fpath[1000];
    char fmark[2000];
    if(strcmp(path,"/") == 0)
    {
        sprintf(fmark,"/home/rak/Documents/rahasia%s.ditandai",path);
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else
    {
        sprintf(fmark,"/home/rak/Documents/rahasia%s.ditandai",path);
        sprintf(fpath, "%s%s",dirpath,path);
    }
    (void) fi;
    int res;
    if (flag == 1)
    {
        //cek direktori ada apa nggak
        DIR *dp;
        dp = opendir("/home/rak/Documents/rahasia");
        if (dp == NULL)
            //buat direktori otomatis   v
            mkdir("/home/rak/Documents/rahasia", 0777);
        //lakukan rename    v
        rename(fpath,fmark);
        //lakukan chmod v
        chmod(fmark, 0000);
        return 0;
    }
    res = open(fpath, O_RDONLY);
    if(res == -1)
        return -errno;
    close(res);
    return 0;
}

static int xmp_rename(const char *src, const char *dst)
{
    char fpath[1000];
    char fdest[1000];
    if(strcmp(src,"/") == 0)
    {
        src=dirpath;
        sprintf(fpath,"%s",src);
	    sprintf(fdest,"%s",dst);
    }
    else
    {
        sprintf(fpath, "%s%s",dirpath,src);
        sprintf(fdest, "%s%s",dirpath,dst);
    }
    int res = rename(fpath,fdest);
    if (res == -1)
        return -errno;
    return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);
    int res;
    res = mkdir(fpath, mode);
    if(res == -1)
        return -errno;
    return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);
    int res;
    res = chmod(fpath, mode);
    if(res == -1)
        return -errno;
    return 0;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    //yg dibutuhkan soal
    .open = xmp_open,
    .rename = xmp_rename,
    .mkdir = xmp_mkdir,
    .chmod = xmp_chmod,
};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
```
Penjelasan:
Masih belum sesuai dengan kehendak soal. Fungsi open selalu terpanggil sebelum aksi open di GUI dilakukan.

2. Lalu compile file tadi dengan
```c
gcc -Wall `pkg-config fuse --cflags` lat2.c -o l2 `pkg-config fuse --libs`
```
dan jalankan di terminal

3. Lakukan simulasi test dengan membuka salah satu file berekstensi pdf, doc atau txt. Maka akan muncul pop up error dan hasil terakhir file dipindahkan ke folder rahasia dan tidak dapat di execute, read ataupun write.
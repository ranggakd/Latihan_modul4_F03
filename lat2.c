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
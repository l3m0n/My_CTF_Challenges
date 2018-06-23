#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <dirent.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <zlib.h>
//#include "../php_encrypt_php.h"
#include "config.h"
#include "../aes.c"
#include "../aes_crypt.c"
#include "../md5.h"

void xor_enc(char *s){
    char *p;
    for (p=s; *p; p++){
        *p = *p ^ 0x9a;
    }
    return s;
}

void errMsg(char *str,char *str2) {
  printf("\033[40;31m%s%s\033[0m\n",str,str2);
}

void alertMsg(char *str,char *str2) {
  printf("\033[1;32m%s%s\033[0m\n",str,str2);
}

void screw_encrypt(char *file);
void screw_decrypt(char *file);
void scanRoot(char *path);
void screw_work(char *file);
int isPHP(char *filename);
int encode = 1;
uint8_t enTag[16];
uint8_t key[64];
void main(int argc, char**argv)
{
    DIR *hP;
    FILE *fp;
    char path[300];
    char suf[1];
    if (argc < 2) {
        errMsg("please input a valid path"," ");
        exit(1);
    }
    if(argc >2 && strncmp(argv[2],"-d",2) == 0)
      encode = 0;
    strcpy(path,argv[1]);
    if(!isPHP(argv[1]) && path[strlen(path)-1]!='/')
        strcat(path,"/");
    hP=opendir(path);
    if(hP == NULL) {
        fp = fopen(path, "r");
        if(fp == NULL) {
            errMsg(path," is not a valid path ");
            exit(1);
        }else
            screw_work(path);
        
    }
    scanRoot(path);
}

void scanRoot(char *path)
{
    struct dirent* dir = NULL;
    DIR *hP;
    FILE *fp;
    int i;
    int l = 0;
    hP=opendir(path);
    if(hP == NULL)
        return;
    while (NULL != (dir=readdir(hP)))
    {
        if(strncmp(dir->d_name,".",1) == 0 || strncmp(dir->d_name,"..",2) == 0)
            continue;
        l = strlen(path) + strlen(dir->d_name);
        char curPath[l+1];
        memset(curPath,0,sizeof(curPath));
        strcat(curPath,path);
        strcat(curPath,dir->d_name);
        if(dir->d_type == 4){
            if(curPath[l-1] != '/')
                strcat(curPath,"/");
            scanRoot(curPath);
        }else
        if(isPHP(dir->d_name) && dir->d_type == 8){
            screw_work(curPath);
        } 
    }
}

int isPHP(char *filename) {
    int i;
    char tmpS[5];
    memset(tmpS,0,sizeof(tmpS));
    for(i = 0;i<strlen(filename);i++)
        if(i >= strlen(filename)-4)
            tmpS[i - strlen(filename)+4] = filename[i];
    tmpS[4] = '\0';
    return strncmp(tmpS,".php",4)==0?1:0;
}

void screw_decrypt(char *file) {
    FILE  *fp;
    struct  stat  stat_buf;
    char  *datap;
    uLong flen, ulen;
    char  lenBuf[16];
    int i,datalen;
    uint8_t enTag[16];
    uint8_t key[64];
    fp = fopen(file, "rb+");
    if (fp == NULL) {
        errMsg("File not found(%s)", file);
        exit(0);
    }

    memset(key, 0, sizeof(key));
    memcpy(key, md5(CAKEY), 32);
    memcpy(enTag, key, 16);
    memset(lenBuf, 0, 16);

    fread(&ulen, sizeof(uLong), 1, fp); // src len
    fread(&flen, sizeof(uLong), 1, fp); // compress len

    // fstat(fileno(fp), &stat_buf);
    // datalen = stat_buf.st_size;

    datap = (char*)malloc(maxBytes);
    memset(datap, 0, sizeof(datap));

    fread(datap, sizeof(char), flen, fp);
    fclose(fp);

    xor_enc(datap);

    char* o2stream = (char *)malloc(maxBytes);
    memset(o2stream, 0, sizeof(o2stream));
    uncompress(o2stream, &ulen, datap, flen);

    aes_crypt(0,o2stream,ulen,key,&ulen);



    fp = fopen(file, "w+");
    fwrite(o2stream, sizeof(char), ulen, fp);
    free(datap);
    free(o2stream);
    fclose(fp);
    alertMsg("Success Decrypting - ", file);
}

void screw_work(char *file) {
  if(encode)
    screw_encrypt(file);
  else
    screw_decrypt(file);
}

void screw_encrypt(char *file) {
    FILE    *fp;
    struct  stat    stat_buf;
    char    *datap, *ostream;
    uLong datalen;
    char  oldfilename[256];
    char *prepare;
    char lenBuf[16];
    int i;

    memset(lenBuf, 0, 16);
    memset(key, 0, sizeof(key));
    memcpy(key, md5(CAKEY), 32);
    memcpy(enTag, key, 16);

    fp = fopen(file, "rb");
    if (fp == NULL) {
        fprintf(stderr, "File not found(%s)", file);
        exit(0);
    }

    // fstat(fileno(fp), &stat_buf);
    // datalen = stat_buf.st_size;
    
    fseek(fp, 0L, SEEK_END);    /* 跳到文件末尾 */  
    datalen = ftell(fp);        /* 获取文件长度 */  
    fseek(fp, 0L, SEEK_SET);  

    datap = (char*)malloc(maxBytes);
    memset(datap, 0, sizeof(datap));
    fread(datap, datalen, 1, fp);
    fclose(fp);

    sprintf(lenBuf,"%d",datalen);
    if (memcmp(datap, enTag, 16) == 0) {
        errMsg(file ," Already Crypted");
        return ;
    }else if(datalen <1) {
        errMsg(file ," will not be crypted");
        return ;
    }
    aes_crypt(1,datap,datalen,key,&datalen);
    
    // compress
    uLong destLen = compressBound(datalen);
    ostream = (char*)malloc(maxBytes);
    memset(ostream, 0, sizeof(ostream));
    compress(ostream, &destLen, (const unsigned char *)datap, datalen); 

    // xor
    xor_enc(ostream);

    // write file
    fp = fopen(file, "wb");
    if (fp == NULL) {
        errMsg("Can not create crypt file(%s)", oldfilename);
        exit(0);
    }

    // write some information
    fwrite(&datalen, sizeof(uLong), 1, fp); // src len
    fwrite(&destLen, sizeof(uLong), 1, fp); // compress len
    fwrite(ostream, sizeof(char), destLen, fp);

    fclose(fp);
    alertMsg("Success Crypting - ", file);

    free(datap);
    free(ostream);
}

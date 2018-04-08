#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
//#define SIZE 21646

//获取文件大小
int file_size(char *filename)
{
    struct stat statbuf;
    stat(filename,&statbuf);
    int size=statbuf.st_size; 
    return size;
}
//加密
int kaisa_enc(const char *str1,char *str2,const int len)
{
    if(str1 == NULL || str2 == NULL || len <= 0)
	{
        return -1;
    }
    int m = strlen(str1);
    if(m <= 0)
	{
        return -1;
    }
	int i;
    for(i=0;i<m;i++)
    {
        if(str1[i]>='a'&&str1[i]<='z')
            str2[i]='a'+(str1[i]-'a'+len)%26;
		else if(str1[i]>='A'&&str1[i]<='Z')
            str2[i]='A'+(str1[i]-'A'+len)%26;
        else
            str2[i]=str1[i];                                                                                 
    }
    return 0;
}

int main()
{
	int nefd,SIZE;
	SIZE=file_size("Bitcoin--A Peer-to-Peer Electronic Cash System");
	if((nefd=open("Bitcoin--A Peer-to-Peer Electronic Cash System",O_RDONLY))==-1)
	{
		perror("open");
		exit(1);
	}
	char necon[SIZE],econ[SIZE];
	ssize_t nerd;
	nerd=read(nefd,necon,SIZE);
	//printf("%s\n",necon);
	if(nerd==-1)
	{
		printf("read content error!\n");
		exit(1);
	}
	close(nefd);
	//计算加密时间
	struct timeval start;
	struct timeval end;
	unsigned long timer;
	gettimeofday(&start,NULL);
	kaisa_enc(necon,econ,3);
	gettimeofday(&end,NULL);
	timer = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
	printf("Encryption time:%ld us\n",timer);
	
	int efd;
	if((efd=open("Bitcoin--A Peer-to-Peer Electronic Cash System ciphertext",O_WRONLY|O_CREAT|O_TRUNC,600))==-1)
	{
		perror("open");
		exit(1);
	}
	ssize_t wfd;
	if((wfd=write(efd,econ,SIZE))==-1)
	{
		perror("write");
		exit(1);
	}
	close(efd);
	//将加密时间转换成字符串类型
	char etime[4];
	sprintf(etime,"%d",timer);
	int apfd;
	if((apfd=open("Bitcoin--A Peer-to-Peer Electronic Cash System ciphertext",O_WRONLY|O_APPEND))==-1)
	{
		perror("open");
		exit(1);
	}
	ssize_t awfd;
	if((awfd=write(apfd,etime,sizeof(etime)))==-1)
	{
		perror("write");
		exit(1);
	}
	close(apfd);
	printf("Encrypt file success!\n");
	return 0;
}
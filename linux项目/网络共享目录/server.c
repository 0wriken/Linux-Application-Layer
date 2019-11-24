#include <arpa/inet.h>
#include "stdio.h"
#include <sys/types.h>
#include <dirent.h>
#include "stdlib.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "string.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define MAX 100
short PORT=8888;
char *IP="192.168.18.149";
char s_buf[1024];
char time_buf[100];
int sockfd;
int fd;
int fds[100];//存放各个客户
int cfd;
int i;
int j;
sem_t sem;
off_t check;
typedef struct meg
{
    int cfd1;
    char file[100];
}AD;
void Ser_init();
void Ser_fd(int *fd,char *file);
void Ser_all(char *file);
void *Ser_thread(void *p);

int main(int argc,char *argv[])
{
    AD ad;//存放客户端与目录
    pthread_t id;
    Ser_init();
    if(argc<2)
    {
        printf("请添加目录\n");
        return 0;
    }
    while(1)
    {
        struct sockaddr s_addr;
        int len=sizeof(s_addr);
        memset(&s_addr,0,sizeof(struct sockaddr));
         if((cfd=accept(sockfd,&s_addr,&len))==-1)
         {
            perror("accept");
            exit(0);
         }
         
         printf("cfd%d\n",cfd);
         fds[i]=cfd;
         ad.cfd1=cfd;
         strcpy(ad.file,argv[1]);
         i++;
         printf("ok1\n");
         pthread_create(&id,0,Ser_thread,&ad);//把目录放入线程判断
    }
    return 0;
}
void *Ser_thread(void *p)  
{
    AD *ad1=(struct meg*)malloc(sizeof(struct meg));
    ad1=(struct meg*)p;  
    char buf[1024]={0};
    char read_buf[1024]={0};
    DIR *dir;
    sem_init(&sem,0,0);
    struct dirent *prt=((struct dirent*)malloc(sizeof(1024)));
    printf("%s\n",ad1->file);
    printf("%d\n",ad1->cfd1);
        while(1)
    {
        printf("完成第一轮\n");
       if((dir=opendir(ad1->file))==NULL)
        {
            perror("opendir");
            exit(0);
        }
 
        while((prt=readdir(dir))!=NULL)
        {
            struct stat sb;
            struct stat sb1;
            if(strcmp(prt->d_name,".")!=0&&strcmp(prt->d_name,"..")!=0)
            {
                sprintf(s_buf,"%s/%s",ad1->file,prt->d_name);
                fd= open(s_buf,O_RDONLY|O_CREAT,0666);
                if(fd<0)
                {
                    perror("open");
                    exit(0);
                }
                if(lstat(s_buf,&sb)==-1)
                {
                    perror("stat");
                    exit(0);
                }
                //存放文件大小
                check=sb.st_size;
                
    printf("ok2\n");
                while(read(fd,buf,1024)!=0)
                {
                   recv(ad1->cfd1,read_buf,1024,0);
                   printf("recv_buf%s\n",read_buf); 
                   //比较文件,不同发送更新信息
                   if(strcmp(buf,read_buf)!=0)//////////////////////发送过去为updatetest
                    {
                        char p[102]={0};
                        strcpy(p,"update");
                        printf("send_buf%s",p);
                     if(send(ad1->cfd1,"up",2,0)==-1)
                    {
                        perror("send");
                        close(ad1->cfd1);
                        pthread_exit((void *)(&ad1));
                        exit(0);
                    }
                     memset(read_buf,0,strlen(read_buf));
                     //更新文件
                     Ser_fd(&ad1->cfd1 ,s_buf);
                    }
                   else
                   send(ad1->cfd1,"right",8,0);
                }
                            //检测服务器文件有无更新
             if(lstat(s_buf,&sb1)==-1)
                {
                    perror("stat1");
                    exit(0);
                }
             if(sb1.st_size!=check)
                 Ser_all(s_buf);//全部更新
                memset(s_buf,0,strlen(s_buf));
                memset(&sb,0,sizeof(struct stat));
                memset(&sb1,0,sizeof(struct stat));
        }
            }
            
    }
}

void Ser_init()
{
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1)
    {
        perror("sockfd");
        exit(0);
    }
    struct sockaddr_in  addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=0;
    if(bind(sockfd,(struct sockaddr *)&addr,sizeof(struct sockaddr))==-1)
    {
        perror("bind");
        exit(0);
    }
    if(listen(sockfd,MAX)==-1)
    {
        perror("listen");
        exit(0);
    }
    printf("服务器初始化完成\n");

}
void Ser_fd(int *fd,char *file)
{
    printf("进入ser_fd\n");
    printf("file:%s\n",file);
    int sfd=0;
    char send_buf[1024]={0};
    char sd_buf[1024]={0};
    sfd= open(file,O_RDONLY|O_CREAT);
    if(sfd<0)
    {
        perror("open");
        exit(0);
    }

    while(read(sfd,send_buf,1024)!=0)
    {
        if(recv(*fd,sd_buf,1024,0)!=0)
        {
            if(strcmp(sd_buf,"ok")==0)
            {
        ////////////////////
            printf("send_buf%s\n",send_buf);
            if(send(*fd,send_buf,strlen(send_buf),0)<0)
            {
                perror("sfd send");
                exit(0);
            }
            memset(send_buf,0,strlen(send_buf));
            }
        }
    }
    printf("服务器发送新文件完成\n");

}
void Ser_all(char *file)
{
    for(int a=0;a<i;a++)
    {
        int sfd=0;
        char send_buf[1024]={0};
        sfd= open(file,O_RDONLY|O_CREAT);
        if(sfd<0)
        {
            perror("open");
            exit(0);
        }
        if(send(fds[a],"update",strlen("update"),0)==-1)
        {
            perror("send");
            exit(0);
        }

        while(read(sfd,send_buf,1024)!=0)
        {
            if(send(fds[a],send_buf,strlen(send_buf),0)<0)
            {
                perror("sfd send");
                exit(0);
            }
            memset(send_buf,0,strlen(send_buf));
        }
        printf("服务器发送新文件完成\n");
    }
}


























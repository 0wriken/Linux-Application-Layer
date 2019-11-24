#include <arpa/inet.h>
#include "stdio.h"
#include <sys/types.h>
#include <dirent.h>
#include "stdlib.h"
#include "string.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
short PORT=8888;
char *IP="192.168.18.149";
int fd;
int sockfd;
sem_t sem;
char s_buf[1024]={0};
void *recv_thread(void *p);
void cli_init(void);
void cli_init()
{
    struct sockaddr_in addr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("sockfd");
        exit(0);
    }
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr(IP);
    addr.sin_port=htons(PORT);
    if(connect(sockfd,(struct sockaddr *)&addr,sizeof(struct sockaddr))==-1)
    {
        perror("connect");
        exit(0);
    }
    printf("客户连接成功\n");

}
void *recv_thread(void *p)
{
    char buf[1024]={0};
    memset(buf,0,1024);
    char read_buf[1024]={0};
    printf("ok1\n");
    while(1)
    {
        memset(read_buf,0,strlen(read_buf));   //第一次2接受
        printf("清空后read_buf%s\n",read_buf);
        if(recv(*(int *)p,read_buf,1024,0)==-1)
        {
            perror("recv");
            exit(0);
        }
        printf("read_buf:%s\n",read_buf);////////接受有问题！！
        if(strcmp(read_buf,"up")==0)
        {
            fd=open(s_buf,O_WRONLY|O_CREAT|O_TRUNC,0666);
            if(fd<0)
            {
                perror("open fd");
                exit(0);
            }
            memset(buf,0,strlen(buf));
            //收到文件的次数
            ///////
            printf("进入update\n");
            send(*(int *)p,"ok",2,0);//确认更新
            if(recv(*(int *)p,buf,1024,0)!=0)
            { 
                printf("recv:%s\n",buf);
               write(fd,buf,strlen(buf));//更新时写入数据
               memset(buf,0,strlen(buf));
            }
            printf("更新完成\n");
            sem_post(&sem);
        }
        else
        {
            printf("当前文件正确\n");
            sem_post(&sem);
        }
    }

}
int main(int argc,char *argv[])
{
    pthread_t id;
    DIR *dir;
    char send_buf[1024]={0};
    int fd1;
    int i=0;
    struct dirent *prt=(struct dirent*)malloc(sizeof(1024));
    cli_init();
    sem_init(&sem,0,0);//设置无名信号量
    pthread_create(&id,NULL,recv_thread,(void*)&sockfd); 

    while(1)
    {
        i=0;
        dir=opendir(argv[1]);
        memset(s_buf,0,strlen(s_buf));
        if(dir==NULL)
        {
            perror("opendir");
            exit(0);
        }
       while(( prt=readdir(dir))!=NULL)
       {
           if(strcmp(prt->d_name,".")!=0&&strcmp(prt->d_name,"..")!=0)
           {
               
                sprintf(s_buf,"%s/%s",argv[1],prt->d_name);
                printf("name %s\n",s_buf);
                fd1=open(s_buf,O_RDONLY|O_CREAT,0666);
                if(fd1==-1)
                {
                    perror("open");
                    exit(0);
                }
               while( read(fd1,send_buf,1024)!=0)
               {
                   //发送文件信息给服务器
                   if(send(sockfd,send_buf,strlen(send_buf),0)==-1)
                   {
                       perror("send");
                       exit(0);
                   }
////////////////////////
                   printf("send_buf%s\n",send_buf);
                   memset(send_buf,0,1024);
               }
               i++;
               printf("第%d文件传输完成\n",i);
               sem_wait(&sem);//等待服务器确认
           }
       }

        printf("校验结束\n");
        printf("等待20秒后重新\n");
        sleep(5);//每20秒校验一次
    }
    return 0;
}






















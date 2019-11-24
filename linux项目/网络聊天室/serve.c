#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define MAX_SIZE 256
short PORT=8888;
char * IP="0";
char name_buf[100];
char name[100][100];
int fds[MAX_SIZE];
int i;
int j;
int a;
int sockfd;
int  test;
void Ser_init(void);
void Sendall(char *p);
void Server();
void flush(int a);
void Sendfd(int *fd,char *buf);
void *Serv_thread(void *p);
int main(int argc,char *argv[])
{
    Ser_init();
    Server();
    return 0;
}
void Ser_init(void)
{
    struct sockaddr_in addr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1)
    {
        perror("sockfd");
        exit(0);
    }
    int reuse = 1;
    test = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));


    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=0;
    if(bind(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr))==-1)
    {
        perror("bind");
        exit(0);
    }
   if( listen(sockfd,MAX_SIZE)==-1)
   {
       perror("listen");
       exit(0);
   }
   printf("服务器初始化完成\n");
     
}
void Server(void)
{
    while(1)
    {
        struct sockaddr s_addr;
        int len=sizeof(s_addr);
        memset(&s_addr,0,sizeof(s_addr));/////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        int fd= accept(sockfd,&s_addr,&len);
        if(fd<0)
        {
            perror("accept");
            exit(0);
        }
        char rcv_buf[100]={0};
        char sbuf[100]={0};
        int test=0;
        if(fd==-1)
        {
            perror("accept");
            exit(0);
        }
        fds[i]=fd; 
        i++;//存放群聊fd
        test=recv(fd,rcv_buf,100,0);
        if(test==-1)
        {
            perror("recv");
            exit(0);
        }
        strcpy(*(name+j),rcv_buf);//拷贝名字
        j++;
        printf("ok\n");
        sprintf(sbuf,"%s用户加入聊天室!",rcv_buf);
        memset(rcv_buf,0,strlen(rcv_buf));
        Sendall(sbuf);
        pthread_t id;
        int *newfd=malloc(sizeof(int));
        *newfd=fd;
        pthread_create(&id,0,Serv_thread,newfd);
        if(i==MAX_SIZE)
        {
           char *bb="该群聊已达上限";
           test=send(fd,bb,strlen(bb),0);
           if(test==-1)
           {
               perror("send bb");
               exit(0);
           }
           close(fd);
        }
    }

}
//结构体存放客户端信息;
void *Serv_thread(void *p)
{
     char r_buf[1024]={0};
     memset(r_buf,0,1024);

str:
    while(1)
    {
               test=recv(*(int*)p,r_buf,sizeof(r_buf),0);
        //线程退出要释放空间
        if(test==0)
        {
            printf("ptread_exit%ld\n",pthread_self());
            close(*(int *)p);
            pthread_exit(NULL);
        }
        if(test==-1)
        {
            perror("serv thread");
            pthread_exit((void*)&i);
            exit(0);
        }
        if(strcmp(r_buf,"person")==0)
            goto leap;
        Sendall(r_buf);
        memset(r_buf,0,strlen(r_buf));
    }
leap:
        printf("当前对话已保密请输入:\n");
        memset(r_buf,0,strlen(r_buf));
        test=recv(*(int *)p,r_buf,sizeof(r_buf),0);
        if(test==-1)
        {
            perror("私人recv\n");
            exit(0);
        }
        printf("r_buf:%s\n",r_buf);
        //判断昵称所在的fd
        for(a=0;a<=j;a++)
        {
            printf("name:%s\n",*(name+a));
            if((strcmp(r_buf,*(name+a))==0))
                break;
            if(a==j)
            {
                printf("未找到该昵称成员\n");
                goto str;
            }
        }

        int fd=fds[a];
        printf("找到用户：%s\n",*(name+a));
        flush(a);
        printf("ok\n");
        memset(r_buf,0,strlen(r_buf));
        while(1)
        {
            test=recv(*(int *)p,r_buf,sizeof(r_buf),0);
            if(test==-1)
            {
                perror("私人recv\n");
                pthread_exit((void*)&i);
                exit(0);
            }

            Sendfd(&fd,r_buf);
            if(strcmp(r_buf,"quit")==0)
            {
                fds[i]=fd;
                fd=0;
                i++;
                goto str;
            }

            memset(r_buf,0,strlen(r_buf));
        }
}



void Sendall(char *p)
{
    int test=0;
    for(int a=0;a<i;a++)
    {
        printf("name:%s\n",*(name+a));
        test=send(fds[a],p,strlen(p),0);
        if(test==-1)
        {
            perror("sendall");
            exit(0);
        }
    }
    printf("群发完毕\n");
}
void Sendfd(int *fd,char *buf)
{
    int test=0;
    test=send(*fd,buf,strlen(buf),0);
    if(test==-1)
    {
        perror("sendfd");
        exit(0);
    }
}
void flush(int a)
{
    for(a;a<i;a++)
    {
        fds[a]=fds[a+1];
    }
    i--;
}

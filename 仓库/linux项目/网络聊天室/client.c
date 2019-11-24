#include <arpa/inet.h>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
short  PORT = 8888; 
char * IP= "192.168.18.149";
int sockfd;
void cli_init(void);
void *recv_thread(void *p);
void cli_init()
{
    struct sockaddr_in addr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1)
    {
        perror("sockfd");
        exit(0);
    } 
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=inet_addr(IP);
    if(connect(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr))==-1)
    {
        perror("connect");
        exit(0);
}
}
void *recv_thread(void *p)
{
    char buf[1024]={0};
    memset(buf,0,strlen(buf));
    while(1)
    {
    int test=0;
    test=recv(sockfd,buf,100,0);
    if(test==-1)
    {
        perror("recv thread");
        exit(0);
    }
    if(test!=0)
    {
    printf("%s\n",buf);
    memset(buf,0,strlen(buf));
    }
}
}
int main(int argc,char *argv[])
{
    char buf[100];
    int test;
    char name_buf[1024];
    char s_buf[1024];
    pthread_t id;
    cli_init();
    printf("请输入的昵称:\n");
    scanf("%s",buf);
    strcpy(name_buf,buf);
    if( send(sockfd,buf,strlen(buf),0)==-1)
  {
      perror("send name");
      exit(0);
  }
   pthread_create(&id,NULL,recv_thread,(void*)&sockfd);
   while(1)
   {
       printf("please input:\n");
       scanf("%s",buf);
       if(strcmp(buf,"person")==0)
       {
           send(sockfd,buf,strlen(buf),0);
           goto leep;
       }
       if(strcmp(buf,"quit")==0)
       {
           goto leap;
       }
       sprintf(s_buf,"%s说:%s",name_buf,buf);
       test=send(sockfd,s_buf,strlen(s_buf),0);
       if(test==-1)
       {
           perror("send");
           exit(0);
       }
       if(strcmp(buf,"person")==0)
       {
leep:      
           memset(buf,0,strlen(buf));
           printf("请输入要私聊的对象昵称\n");

           scanf("%s",buf);
leap:           test=send(sockfd,buf,strlen(buf),0);
           if(test==-1)
           {
               perror("私聊昵称\n");
               exit(0);
           }
       }
       memset(buf,0,strlen(buf));
       memset(s_buf,0,strlen(s_buf));
       
   }
   return 0;
}
















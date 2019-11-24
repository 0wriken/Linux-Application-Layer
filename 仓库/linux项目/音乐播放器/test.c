#include "mp3.h"

int name[100];
int all=0;
int i;
int com=0;
int flag=1;
int n=0;
int a;
static int m=48;
void flush(void);
void findmusic(void)
{
    system("./find.sh");//脚本查询
}
//从路径下读取mp3文件名
void getname(void)
{
    int len=0;
    static char *buf;
    DIR *dir;
    struct dirent *prt;
    dir=opendir("/mnt/hgfs/linux--share/音乐文件夹");
    while((prt=readdir(dir))!=NULL)
    {
        len=strlen(prt->d_name);
        
        len=len-4;
        if(strcmp(prt->d_name+len,".mp3")==0)
        {
            buf=(char*)malloc(sizeof(char)*100);
            memset(buf,0,strlen(buf));
            strcpy(buf,prt->d_name);
            song[all++]=buf;
        }
    }
    for(int i=0;i<all;i++)
    printf("%d:%s\n",i,song[i]);

}
//列表循环播放
void play_listcircle_music(int b)
{
    flag=0;
    printf("进入列表循环\n");
    while(1)
    {

        printf("m:%d\n",m);
text: 
        text();
        if(m!=48)  //音乐在播放

        {
            sleep(3);//等待播放结束
            goto text;
        }
        savename();
        if(com>all-1)
        {
            com=0;
            break;
        }
        printf("com:%d\n",com);
        sprintf(song_buf,"madplay /mnt/hgfs/linux--share/音乐文件夹/%s &",song[com]);
        printf("music:%s\n",song[com]);
        printf("song_buf:%s\n",song_buf);
        system(song_buf);
        flush(); 
        com++;
    }
}
//单曲循环
void play_singcircle_music(int a)
{
    flag=1;
    while(1)
    {
text:   text();
        if(m!=48)
        {
            sleep(3);
            goto text;
        }
              savename();
             sprintf(song_buf,"madplay /mnt/hgfs/linux--share/音乐文件夹/%s &",song[com]);
            system(song_buf);
            flush();
    }

}
//随机播放
void play_random_music(int c)
{
    int r;
    flag=2;
    while(1)
    {
text:  text();
       if(m!=48)
       {
           sleep(3);
           goto text;
       }
repeat:
       srand((int)time(NULL));
       r=rand()%(all);//产生随机数
       if(r>all-1)
           goto repeat;
       com=r;
       savename();
       sprintf(song_buf,"madplay /mnt/hgfs/linux--share/音乐文件夹/%s &",song[com]);
       system(song_buf);
       flush();    
    }
}
void last_music(int a )
{
   sprintf(song_buf,"killall -9 madplay");//停止播放
   system(song_buf);
   flush();
   if(n<0)//savename存放播放历史
   {
         printf("当前播放第一首\n");
         n=0;
   }
   for(int a=0;a<n;a++)
   {
   printf("name:%d\n",name[a]);
   }
   com=name[n-1];//播放上一首
   n--;
    sprintf(song_buf,"madplay /mnt/hgfs/linux--share/音乐文件夹/%s &",song[com]);
    system(song_buf);
    flush();
    //当前播放方式选择
    if(flag==0)
        raise(4); //发送信号给进程自身
    if(flag==1)
        raise(5);
    if(flag==2)
        raise(6);

}
//播放下一首
void next_music(int b)
{
   sprintf(song_buf,"killall -9 madplay");
   system(song_buf);
   flush();
   switch(flag)
   {
    case 1:
       savename();
       text();
       if(m!=48)
       { sprintf(song_buf,"killall -9 madplay");
           system(song_buf);
           flush();}
       printf("当前单曲循环\t n=%d",n);
       sprintf(song_buf,"madplay /mnt/hgfs/linux--share/音乐文件夹/%s &",song[com]);
      system(song_buf);
      flush();
      raise(5);
      break;
    case 0:
       text();
       if(m!=48)
       { sprintf(song_buf,"killall -9 madplay");
           system(song_buf);
           flush();}
      printf("com==%d,all==%d\n",com,all);
       if(--com==all-1) //最后一首
       {
           printf("最后一首\n");
           com=-1;
       }
       com++;
       savename();
       text();
       if(m!=48)
       {
           sprintf(song_buf,"killall -9 madplay");
           system(song_buf);
           flush();
       }
       printf("当前列表循环\n");
       sprintf(song_buf,"madplay %s &",song[com]);
       system(song_buf);
       flush();
       printf("song com=%d\n",com);
       raise(4);
       break;
    case 2:
reap:   
      srand((int)time(NULL));
      int r=rand()%(all);
      if(r>all-1)
          goto reap;
      com=r;
      savename();
      text();
      if(m!=48)
        {  sprintf(song_buf,"killall -9 madplay");
           system(song_buf);
           flush();
        }
      printf("当前随机播放\n");
     sprintf(song_buf,"madplay %s &",song[com]);
     system(song_buf);
     flush();
     raise(6);
    break;
   }
}
//判断madplay是否在运行，运行为1，非运行为0;
void text(void )
{

   FILE   *stream;   
   
   char   buf[1024];   
       
    memset( buf, '\0', sizeof(buf) );//初始化buf,以免后面写如乱码到文件中  
    stream = popen( "ps -ef | grep -w madplay | grep -v grep | wc -l", "r" ); 
  
    fread( buf, sizeof(char), sizeof(buf), stream); //将刚刚FILE* stream的数据流读取到buf中  
    m=(int)buf[0];
    
}
void savename(void)
{
    if(n<100)
    {
        name[n++]=com; //存放播放过的音乐
    }
    else
        Name(); 
}

void Name(void)
{
    for(int i=0;i<99;i++)
    {
        name[i]=name[i+1];
    }
    n--;
}
void play_music_com(int a)
{
    getname();
    printf("请点歌\n");
    int ch=0;
    scanf("%d",&ch);
    com=ch;
    sprintf(song_buf,"madplay /mnt/hgfs/linux--share/音乐文件夹/%s &",song[com]);
    savename();
    system(song_buf);
    flush();
    printf("选择播放方式");
    int flag1=0;
    scanf("%d",&flag1);
    if(flag1==0)
    raise(4);
    if(flag1==1)
        raise(5);
    if(flag==2)
        raise(6);
}
void flush(void)
{
    char buf[100];
    sprintf(buf,"killall -SIGSTOP madplay");
    system(buf);
    memset(buf,0,100);
    sprintf(buf,"killall -SIGCONT madplay");
    system(buf);

}

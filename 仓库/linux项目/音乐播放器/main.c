#include "mp3.h"
int main(int argc,char *argv[])
{
    system("killall -9 madplay");
    int ch;
    getname();
    pid_t pid=fork();
    if(pid<0)
    {
        printf("创建进程失败\n");
        return 0;
    }
    else if(pid==0)
    {
        //signal(20,&fun);//信号与之执行函数
        signal(4,&play_listcircle_music);
        signal(5,& play_singcircle_music);
        signal(6,&play_random_music);
        signal(7,&last_music);
        signal(8,&next_music);
        signal(10,&play_music_com);
        while(1);
    }
    else
    {
        while(1)
        {
        printf("欢迎来到我的音乐\n");
        printf("*******************************************0:暂停音乐播放********************************\n");
        printf("*******************************************1:继续播放音乐********************************\n");
        printf("*******************************************2:列表循环播放********************************\n");
        printf("*******************************************3:音乐单曲循环********************************\n");
        printf("*******************************************4:音乐随机播放********************************\n");
        printf("*******************************************5:切换上一首歌********************************\n");
        printf("*******************************************6:切换下一首歌********************************\n");
        printf("*******************************************7:点歌****************************************\n");
        printf("******************************************8:关闭音乐播放器*******************************\n");
        printf("请输入按键操作\n");
        scanf("%d",&ch);
        switch(ch)
        {
            case 0:
                printf("暂停\n");
                sprintf(cmd_buf,"killall -SIGSTOP madplay");
                system(cmd_buf);
                break;
            case 1:
                printf("继续播放文件\n");
                sprintf(cmd_buf,"killall -SIGCONT madplay");
                system(cmd_buf);
                break;
            case 2:
                printf("列表循环播放\n");
                kill(pid,4);
                break;
            case 3:
                printf("单曲循环\n");
                kill(pid,5);//向pid发送信号
                break;
            case 4:
                printf("随机播放\n");
                kill(pid,6);
                break;
            case 5:
                printf("上一首\n");
                kill(pid,7);
                break;
            case 6:
                printf("下一首\n");
                kill(pid,8);
                break;
            case 7:
                kill(pid,10);
                break;
            case 8:
                printf("关闭音乐程序\n");
                sprintf(cmd_buf,"killall -9 madplay");
                system(cmd_buf);
                system("killall -9 mp3");
                raise(9);
                break;
            default: break;

        }
    }
    }
}

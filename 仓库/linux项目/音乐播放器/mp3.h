#ifndef MP3_H_
#define MP3_H_
#include "stdio.h"
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/ptrace.h>
#include "string.h"
extern void findmusic(void);
extern void getname(void);
extern void play_singcircle_music(int );
extern void play_listcircle_music(int );
extern void play_random_music(int );
extern void last_music(int a );
extern void next_music(int b);
extern void text(void );
extern void savename(void);
extern void Name(void);
extern void play_music_com(int);

char *song[100];
char song_buf[100];
char cmd_buf[100];







#endif

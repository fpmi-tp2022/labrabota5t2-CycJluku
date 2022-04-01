#ifndef SHAREFILE_INCLUDED
#define SHAREFILE_INCLUDED
#ifdef  MAIN_FILE
extern short access; 
extern short isCommander;
extern char current_buff[100];
extern char current_ID[10];
#else
short access; 
short isCommander;
char current_buff[100];
char current_ID[10];
#endif
#endif

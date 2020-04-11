#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/wait.h>
char *shellpath[100]; // 환경변수 저장 변수
char cwd[1024]; // 현재의 주소
int getenvown(char argv[100]){ // getenv함수를 실행하기 위한 함수
    char slash[10] = "/";
    int i,j = 0;
    char *env, *str;
    char *tok[100];
    if (argv == "init"){ // 입력값이 init일 경우 path으로 getenv 실행
        env = getenv("PATH");
    }else // 아닐경우 입력값으로 실행
        env = getenv(argv);
    printf("%s=%s\n",argv,env);
    for(j=0,str=env; ;str=NULL,j++){
        tok[j] = strtok(str,":");
        if(tok[j] == NULL) break;
        if(argv == "init"){
            shellpath[j] = tok[j];
        }
        printf("\t%s\n",tok[j]);

        tok[j] = NULL; // 토크의 값을 NULL로 초기화
    }

    return 0;
}
int forkstart(char temp1[100], char *tok1[100]){ // 프로세서를 실행하여 프로그램을 실행하는 함수
    int isfork;
    pid_t pid;
    pid = fork();
    switch(pid){ // 포크가 생성되었는지 여부를 판단함
        case -1:
            isfork = -1;
            printf("fork failed");
            break;
        case 0:
            execv(temp1,tok1); // 프로그램 실행
            printf("exec failed");
            break;
        default:
            wait((int*)0); // 생성된 프로세서가 끝나는 것을 기다림
            isfork=0;
    }
    return isfork;
}
int statcheck(char in[100]){ // 프로그램이 실행가능 한지 여부를 판단하고 실행이 가능하다면 forkstart함수를 실행
    pid_t pid;
    struct stat fstat_buf;
    int i=0, count=0, j=0;
    int ret;
    char inconvert[100] = "/";
    char temp[100] = "";
    char *tok[100];
    char *str = strtok(in," ");
    count = 0;
    while(tok[i]!=NULL){ // 실행 전 남아있는 tok의 값을 비움
        tok[i] = NULL;
        i++;}
    i=0;
    while(str != NULL){ // 입력된 문자열을 띄어쓰기를 기준으로 자름
        tok[i] = str;
        str = strtok(NULL," ");
        i++;
    }
    strcat(inconvert,tok[0]); // make char -> /char;
    if ( strcmp(tok[0],"cd") == 0 ){ // cd명령어가 입력이 되었다면 경로를 움직이고 함수를 빠져나옴
        if(chdir(tok[1]) == 0){ 
            getcwd(cwd,sizeof(cwd));}
        else{
            fprintf(stderr,"path error\n");}
        return 0;
    }

    while(shellpath[j] != NULL) { // 저장된 환경변수의 값들을 통해 명령어를 검색함
        temp[0] = '\0';
        strcat(temp,shellpath[j]);
        strcat(temp,inconvert);
        if (stat(temp,&fstat_buf)==0){
            ret=stat(temp,&fstat_buf);
            if (forkstart(temp,tok)==0);
            else
                printf("fork is failed\n");
            return ret;
        }
        j++;
    }
    ret = stat(tok[0],&fstat_buf);
    if(ret==-1){
        perror("stat");
    }
    return ret;
}



int main()
{
    int compar; // 문자열을 비교하기 위한 변수
    char input[100]=""; // 입력값을 받기 위한 변수
    char quit[100]="quit"; // 문자열 quit을 저장
    getenvown("init");
    getcwd(cwd,sizeof(cwd));
    do{
        printf("%s (if input = quit program shutdown) > ",cwd);
        fgets(input,sizeof(input),stdin); // 문자열 입력을 받음
        input[strlen(input)-1] = '\0';
        compar = strcmp(input,quit); // 입력된 문자열과 quit을 아스키값으로 비교
        if(compar == 0){ // 같다면 반복문을 빠져나옴
            break;
        }
        else {
            if (input == "pwd") // pwd가 입력되면 프로그램이 동작하고 있는 위치를 알려줌
                printf("%s\n",cwd);
            else if (statcheck(input) == 0 );
            else {
                getenvown(input); // getenv의 명령어가 입력된다면 이것을 통해 실행
            }

        }

    }while(1); // 프로그램은 quit을 입력받기 전까지는 종료되지 않음

    return 0;
}


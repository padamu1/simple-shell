Simple_Shell by C
================
# Composed by
### Python code file(.c)

# Preview
#### int statcheck(char in[100]){ // Determine whether the program is executable and if it is possible to execute the forkstart function
    pid_t pid;
    struct stat fstat_buf;
    int i=0, count=0, j=0;
    int ret;
    char inconvert[100] = "/";
    char temp[100] = "";
    char *tok[100];
    char *str = strtok(in," ");
    count = 0;
    while(tok[i]!=NULL){ // before start clear tok
        tok[i] = NULL;
        i++;}
    i=0;
    while(str != NULL){ // cut input string by spacebar
        tok[i] = str;
        str = strtok(NULL," ");
        i++;
    }
    strcat(inconvert,tok[0]); // make char -> /char;
    if ( strcmp(tok[0],"cd") == 0 ){ // if input is "cd" move cosor and exit function
        if(chdir(tok[1]) == 0){ 
            getcwd(cwd,sizeof(cwd));}
        else{
            fprintf(stderr,"path error\n");}
        return 0;
    }

    while(shellpath[j] != NULL) { // use stored path and start fork using forkstart function
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
* * *
# Tools
### elementary os 5.1.2
### (Linux 5.3.0 - 42)
	
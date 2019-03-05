#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXCHAR 2000
#define MAXARGS 100

char** pipeParse(char* input, int* pipeFound){
  char** pipeBreak = malloc(MAXARGS * sizeof(char*));
  char* input_backup = strdup(input);
  char* found;
  int length = 0;

  if(strsep(&input_backup, "|") != NULL){
    while((found = strsep(&input, "|")) != NULL){
      pipeBreak[length] = malloc((strlen(found) + 1) * sizeof(char));
      pipeBreak[length] = found;
      length++;
      }
//    printf("pipeBreak[0] : %spipeBreak[1] : %s\n", pipeBreak[0], pipeBreak[1]);
    }

    //printf("length : %d \n", length);
    if(length == 2){
      *pipeFound = 1;
    }
    else{
      *pipeFound = 0;
  }
  return pipeBreak;
}


char** spaceParse(char* input){
  int length = 0;
  char** output = malloc(MAXARGS * sizeof(char*));
  //char* input_backup = malloc(strlen(input) * sizeof(char));
  //strcpy(input_backup, input);
  char* found;
  //pass 1 ===> get num of args
  while((found = strsep(&input, " ")) != NULL){
    if((strcmp(found, "") != 0)) {
      output[length] = malloc((strlen(found) + 1) * sizeof(char));
      output[length] = found;
      char* pos;
      if((pos = strchr(output[length], '\n')) != NULL) {
        *pos = '\0';
      }
       //printf("%s \n", output[(length)]);
      length++;
      }
  }

  printf("spaceParse found %d strings \n", length);
  return output;
}
int execArgs(char** args, int execMode){
  pid_t pid;
  int status;

  //for parent: fork a child
  if((pid = fork()) < 0){
    printf("*#*#*#*# AND ERROR HAS BEEN ENCOUNTERED: UNABLE TO FORK A CHILD #*#*#*#*");
    exit(1);
  }
  else if(pid == 0){
    if(execvp(args[0], args)){
      printf("*#*#*#*# AND ERROR HAS BEEN ENCOUNTERED WITH THE PROGRAM FILE EXECUTION #*#*#*#*");
    }
  }
  else{
    pid_t c = wait(&status);
    printf("Parent: Child %ld exited with status = %d\n",
(long) c, status);

  }
  return status;
}


int processInput(char* input){
  //printf(" input inside processInput function: %s \n",input);
  int pipeFlag = 0;
  char **pipeParsed = pipeParse(input, &pipeFlag);

  if(pipeParsed != NULL){
    //printf("whole expression: %s \n", pipeParsed[0]);
  }
  printf("pipeStatus: %d \n", pipeFlag);
  if(pipeFlag == 1){
    //printf("before pipe: %s \n after pipe: %s \n", pipeParsed[0], pipeParsed[1]);
    char** beforePipe = spaceParse(pipeParsed[0]);
    char** afterPipe = spaceParse(pipeParsed[1]);
    int overallCount = 0;
    int count1 = 0;
    while(beforePipe[count1] != NULL){
      //printf("%s ", beforePipe[count1]);
      count1++;
      overallCount++;
    }
    printf("\n\n");
    int count2 = 0;
    while(afterPipe[count2] != NULL){
      //printf("%s ", afterPipe[count2]);
      count2++;
      overallCount++;
    }

    char** argsArray = malloc(overallCount * sizeof(char*));
    for (int i = 0; i < count1; i++){
      argsArray[i] = strdup(beforePipe[i]);
      printf("%s---%d\n", argsArray[i], (int)strlen(argsArray[i]));
    }

    for(int i = 0; i < count2; i++){
      argsArray[(overallCount - count2) + i] = strdup(afterPipe[i]);
      printf("%s---%d \n", argsArray[(overallCount - count2) + i], (int)strlen(argsArray[(overallCount - count2) + i]));
    }
    argsArray[overallCount] = NULL;
  }
  else{
    char**argsArray = spaceParse(input);
    execArgs(argsArray, 1);
  }
  return 0;
}



int main(){
  char input[MAXCHAR];
  char* user = getenv("USER");
  //int numOfArgs = 0;
  int pipeFound = 0;
  while(1){
    printf("%s-$: ", user);
    fgets(input, MAXCHAR, stdin);
    if(strcmp(input, "exit\n")==0){
      exit(0);
    }

    printf("%s\n", input);
    int result = processInput(input);

    //printf("%d \n", pipeFound);
  }
  return 0;
}

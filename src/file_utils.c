#include <file_utils.h>

FILE* fd;
char **content;
int allocated = 0;
int lines;

int count_lines(){
  int lines = 0;
  while (EOF != (fscanf(fd, "%*[^\n]"), fscanf(fd, "%*c"))) 
    ++lines;
  return lines;
}

char *getLine(int i){
  return content[i];
}

int getLinesCount(){
  return lines;
}

void setLine(int ind, char* buf){
  free(content[ind]);
  content[ind] = buf;
}

void initializeFileAccess(char *path){
  fd = fopen(path, "r+");

  if (fd == NULL){
    printf("Cannot open file.");
    exit(0);
  }

  lines = count_lines();

  content = calloc(lines, sizeof(char*));

  //load file content
  char buf[256];
  bzero(buf, 256);
  int i=0;
  rewind(fd);
  while(1){
    if (fscanf(fd, "%[^\n]", buf) == EOF)
      break;
    int l = strlen(buf);
    content[i] = calloc(l+1, sizeof(char));
    strcpy(content[i],buf);
    i++;
    fscanf(fd, "%*c");
    bzero(buf, l+1);
  }
}

void updateFile(){
  rewind(fd);
  int size = 0;
  for (int i=0; i<lines; i++){
    fwrite(content[i], strlen(content[i]), 1, fd);
    fwrite("\n", 1, 1, fd);
    size += 1 + strlen(content[i]);
  }
  ftruncate(fileno(fd), size);
}

void saveCloseFile(){
  updateFile();
  fclose(fd);

  for (int i=0; i < lines; i++)
    free(content[i]);
  free(content);
}
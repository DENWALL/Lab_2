#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

void help() {
/*printf(" program \"\" -f file # for write output to file\n" );
printf("\n");
printf("the program for the count the number of hidden objects in directory\n\n");
printf("Usage:\n");
printf(" program scanning_dir\n");
printf(" program \"\" # for current directory\n" );
printf("\n");
printf("Result example:\n");
printf(" all objects: 7\n");
printf(" hidden objects: 1\n");
printf("\n");*/
exit(0);
}
int main (int argc, char *argv[]) {
// help:
if ((argc < 2) || (strcmp(argv[1], "-h") == 0)) {
help();
}
// create dir path:
char *inputdir = argv[1];
char *dirpath = malloc(2048);
dirpath[0] = 0;
if (strlen(inputdir) == 0) {
getcwd(dirpath, 2048);
} else {
strcpy(dirpath, inputdir);
}
// open dir stream:
DIR *dir = opendir(dirpath);
if (dir == NULL) {
fprintf(stderr, "error: cannot open \"%s\" dierctory (errno code %x)\n", dirpath, errno);
free(dirpath);
exit(-1);
}
//free(dirpath);
// scan objects in dir stream:
struct dirent *obj = readdir(dir);
int allobjs = 0;
int hidobjs = 0;
int folder = 0;
char *out = malloc(128);
while (obj != NULL) {
// check object name:
if ((strcmp(obj->d_name, ".") != 0) && (strcmp(obj->d_name, "..") != 0)) {
allobjs++;
if (obj->d_name[0] == '.') {
hidobjs++;
}
}
if(opendir(obj->d_name)!=NULL){
sprintf(out,"folder:");
sprintf(out,"%s%s%s\n",dirpath,"/",obj->d_name);
//sprintf(out);
}
if(opendir(obj->d_name)==NULL){
sprintf(out,"file:");
sprintf(out,"%s%s%s\n",dirpath,"/",obj->d_name);
//sprintf(out);
}
obj = readdir(dir);
printf("%s",out);
}
free(dirpath);
// send result to stdout or file:
//char *out = malloc(128);
sprintf (out, "all objects: %d\nhidden objects: %d\n", allobjs, hidobjs);
if (argc < 4) {
printf(out); // send to stdout
} else {
// write to file:
if (strcmp(argv[2], "-f") != 0) {
free(out);
help();
}
char *outfilename = argv[3];
int fd = open(outfilename, O_WRONLY | O_CREAT, 0777);
if (fd == -1) {
fprintf(stderr, "error: cannot open \"%s\" file (errno code %x)\n", outfilename, errno);
free(out);
exit(-1);
}
int n = write(fd, out, strlen(out));
if (n == -1) {
close(fd);
fprintf(stderr, "error: cannot write data to \"%s\" file (errno code %x)\n", outfilename, errno);
free(out);
exit(-1);
}
close(fd);
}
return 0;
}

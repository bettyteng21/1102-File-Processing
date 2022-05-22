#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

int hit_count=0;

char* find_file(char* dir, char* file_name){
	DIR *start_dir;
    struct dirent *dirp;
    char temp[10000];

    errno=0;
	if ((start_dir = opendir(dir)) == NULL) {
         switch (errno) {
            // case EACCES: printf("Permission denied\n"); break;
            case ENOENT: printf("Directory does not exist\n"); break;
            case ENOTDIR: printf("'%s' is not a directory\n", dir); break;
        }
        return NULL;
    }

    while ((dirp = readdir(start_dir)) != NULL){ 
        memset(temp,'\0', sizeof(temp));

		if(strcmp(dirp->d_name, ".")!=0 && strcmp(dirp->d_name, "..")!=0){ //跳過.和..
            /* 如果目前是讀到一個folder，就加上目前路徑，然後繼續遞迴往下尋找 */
			if(dirp->d_type==DT_DIR){ 
				if(strcmp(dir, "/")==0){
					sprintf(temp, "%s%s", dir, dirp->d_name);
				}
		    	else sprintf(temp, "%s/%s", dir, dirp->d_name);
			    
				find_file(temp, file_name);
			}
            /* 如果目前是讀到一個file，就判斷檔名是否符合，若符合就印出來 */
			else if(dirp->d_type==DT_REG){
				if(strcmp(dirp->d_name, file_name)==0){
					sprintf(temp, "%s/%s", dir, dirp->d_name);
					printf("%s\n", temp);
					hit_count++;
					// return NULL;
				}
				
			}
			
		}
		
    }

    if (closedir(start_dir) == -1)
        perror("closedir");

    return NULL;
}

int main(int argc, char *argv[])
{
    DIR *start_dir;
    struct dirent *dirp;
    char dir[550], file_name[550];
    memset(dir,'\0', sizeof(dir));
    memset(file_name,'\0', sizeof(file_name));

    /* 處理傳入的參數 */
    if (argc <= 1) {
        fprintf(stderr, "Usage: ./program directory_name\n");
        exit(EXIT_FAILURE);
    }

    if(!argv[2]){ // 沒輸入路徑代表從目前路徑開始找
    	strcpy(dir, ".");
    	strcpy(file_name, argv[1]);
    }
    else{
    	strcpy(dir, argv[1]);
    	strcpy(file_name, argv[2]);
    }
    printf("from path: %s\n", dir);
    printf("search file: %s\n", file_name);

 	find_file(dir, file_name);

 	if(hit_count==0) printf("Cannot find the corresponding file\n");
    
    return 0;
}
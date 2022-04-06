#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/time.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

int f1;
long int size = (200 * 1024 * 1024)-1;
char buffer_init[200*1024*1024];
char buffer_seq_read[200*1024*1024], buffer_seq_write[200*1024*1024], buffer_rand_read[4096];
char buffer_rand_write1[2048], buffer_rand_write2[2048];

void seq_read(){
	f1= open("1-3.txt", O_RDONLY);
	struct stat st;
	stat("1-3.txt", &st);
	char *map_f1= mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, f1, 0);
	if(map_f1 == MAP_FAILED){
		printf("mmap error\n");
		return;
	}

	struct timeval start;
	struct timeval end;
	float diff;

	gettimeofday(&start,NULL);

	int read_count= ((200*1024*1024)/4096);
	for(int i=0; i<read_count ; i++){
		memcpy(buffer_seq_read, map_f1, 4096);
		map_f1 += 4096; //sequential read 4KB
	}
	
	gettimeofday(&end,NULL);

	diff = end.tv_sec-start.tv_sec + ((float)(end.tv_usec - start.tv_usec)/1000000);
	printf("Sequential Read: %f(s) \n",diff);

	map_f1 -= (read_count*4096);
	if(munmap(map_f1, st.st_size)==-1) printf("unmap error\n");
	close(f1);

}

void seq_write(){
	f1= open("1-3.txt", O_RDWR);
	struct stat st;
	stat("1-3.txt", &st);
	char *map_f1= mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, f1, 0);
	if(map_f1 == MAP_FAILED){
		printf("mmap error\n");
		return;
	}

	struct timeval start;
	struct timeval end;
	float diff;
	for (int i=0; i < (200*1024*1024); i++){
		buffer_seq_write[i]='a';	
	}

	gettimeofday(&start,NULL);

	int write_count= ((200*1024*1024)/2048);
	for(int i=0; i<write_count; i++){
		memcpy(map_f1, buffer_seq_write, 2048);
		map_f1 += 2048; //sequential write 2KB
		fsync(f1);
	}
	fsync(f1);

	gettimeofday(&end,NULL);

	diff = end.tv_sec-start.tv_sec + ((float)(end.tv_usec - start.tv_usec)/1000000);
	printf("Sequential Write: %f(s) \n",diff);

	map_f1 -= (write_count*2048);
	if(munmap(map_f1, st.st_size)==-1) printf("unmap error\n");
	close(f1);
}

void rand_read(){
	f1= open("1-3.txt", O_RDONLY);
	struct stat st;
	stat("1-3.txt", &st);
	char *map_f1= mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, f1, 0);
	if(map_f1 == MAP_FAILED){
		printf("mmap error\n");
		return;
	}

	struct timeval start;
	struct timeval end;
	float diff;
	int rand_num=0;
	time_t t;

	gettimeofday(&start,NULL);

	srand((unsigned) time(&t));
	for (int i=0; i<50000; i++){
		rand_num= (rand() % (51200));  //共切成51200個格子(rand隨機取0~51199的一個數)
		map_f1 += (rand_num*4096);
		memcpy(buffer_seq_read, map_f1, 3072);
		map_f1 -= (rand_num*4096); 
	}

	gettimeofday(&end,NULL);

	diff = end.tv_sec-start.tv_sec + ((float)(end.tv_usec - start.tv_usec)/1000000);
	printf("Random Read: %f(s) \n",diff);

	if(munmap(map_f1, st.st_size)==-1) printf("unmap error\n");
	close(f1);
}

void rand_write1(){
	f1= open("1-3.txt", O_RDWR);
	struct stat st;
	stat("1-3.txt", &st);
	char *map_f1= mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, f1, 0);
	if(map_f1 == MAP_FAILED){
		printf("mmap error\n");
		return;
	}

	struct timeval start;
	struct timeval end;
	float diff;
	int rand_num=0;
	time_t t;
	for (int i=0; i < 4096; i++){
		buffer_rand_write1[i]='b';	
	}

	gettimeofday(&start,NULL);

	srand((unsigned) time(&t));
	for (int i=0; i<50000; i++){
		rand_num= (rand() % (51200));  //共切成51200個格子(rand取0~51199)
		map_f1 += (rand_num*4096);
		memcpy(map_f1, buffer_rand_write1, 4096);
		map_f1 -= (rand_num*4096);
	}

	gettimeofday(&end,NULL);

	diff = end.tv_sec-start.tv_sec + ((float)(end.tv_usec - start.tv_usec)/1000000);
	printf("Random Write1: %f(s) \n",diff);

	if(munmap(map_f1, st.st_size)==-1) printf("unmap error\n");
	close(f1);
}

void rand_write2(){
	f1= open("1-3.txt", O_RDWR);
	struct stat st;
	stat("1-3.txt", &st);
	char *map_f1= mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, f1, 0);
	if(map_f1 == MAP_FAILED){
		printf("mmap error\n");
		return;
	}

	struct timeval start;
	struct timeval end;
	float diff;
	int rand_num=0;
	time_t t;
	for (int i=0; i < 3072; i++){
		buffer_rand_write2[i]='c';	
	}

	gettimeofday(&start,NULL);

	srand((unsigned) time(&t));
	for (int i=0; i<50000; i++){
		rand_num= (rand() % (51200));  //共切成51200個格子(rand取0~51199)
		map_f1 += (rand_num*4096);
		memcpy(map_f1, buffer_rand_write2, 3072);
		map_f1 -= (rand_num*4096);
		fsync(f1);
	}

	gettimeofday(&end,NULL);

	diff = end.tv_sec-start.tv_sec + ((float)(end.tv_usec - start.tv_usec)/1000000);
	printf("Random Write2: %f(s) \n",diff);

	if(munmap(map_f1, st.st_size)==-1) printf("unmap error\n");
	close(f1);
}

int main(int argc, char const *argv[])
{
	for (int i=0; i<(200*1024*1024); i++){
		buffer_init[i]='0';
	}
	f1 = open("1-3.txt", O_WRONLY | O_CREAT, 0777); //create file
	write(f1, buffer_init, 200*1024*1024);
	close(f1);

	seq_read();
	seq_write();
	rand_read();
	rand_write1();
	rand_write2();

	return 0;
}

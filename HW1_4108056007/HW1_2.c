#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/time.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int f1;
long int size = (200 * 1024 * 1024)-1;
char buffer_init[200*1024*1024];
char buffer_seq_read[200*1024*1024], buffer_seq_write[200*1024*1024], buffer_rand_read[4096];
char buffer_rand_write1[2048], buffer_rand_write2[2048];

void seq_read(){	
	f1= open("1-2.txt", O_RDONLY);
	struct timeval start;
	struct timeval end;
	float diff;

	gettimeofday(&start,NULL);

	for(int i=200*1024*1024; i>0 ; i-=4096){
		if(read(f1, buffer_seq_read, 4096) < 0){
			printf("sequential read: read error!\n");
			return;
		}
	}
	
	gettimeofday(&end,NULL);

	
	diff = end.tv_sec-start.tv_sec + ((float)(end.tv_usec - start.tv_usec)/1000000);
	printf("Sequential Read: %f(s) \n",diff);
	close(f1);
} 

void seq_write(){
	f1= open("1-2.txt", O_RDWR);
	struct timeval start;
	struct timeval end;
	float diff;
	int write_num= ((200*1024*1024)/2048);
	lseek(f1, 0, SEEK_SET);

	for (int i=0; i < (200*1024*1024); i++){
		buffer_seq_write[i]='a';	
	}

	gettimeofday(&start,NULL);
	
	for(int i=0; i<write_num ; i++){
		// if(write(f1, buffer_seq_write, 2048) == -1){
		// 	printf("sequential write: write error!\n");
		// 	return;
		// }
		write(f1, buffer_seq_write, 2048);
		fsync(f1);
	}
	
	gettimeofday(&end,NULL);

	diff = end.tv_sec-start.tv_sec + ((float)(end.tv_usec - start.tv_usec)/1000000);
	printf("Sequential Write: %f(s) \n",diff);
	close(f1);
}

void rand_read(){
	f1= open("1-2.txt", O_RDONLY);
	struct timeval start;
	struct timeval end;
	float diff;
	int rand_num=0;
	time_t t;
	lseek(f1, 0, SEEK_SET);

	gettimeofday(&start,NULL);

	srand((unsigned) time(&t));
	for (int i=0; i<50000; i++){
		rand_num= (rand() % (51200));  //共切成51200個格子(rand隨機取0~51199的一個數)
		lseek(f1, (rand_num*4096), SEEK_SET); 
		if(read(f1, buffer_rand_read, 3072) < 0){
			printf("random read: read error!\n");
			return;
		} 
	}

	gettimeofday(&end,NULL);

	diff = end.tv_sec-start.tv_sec + ((float)(end.tv_usec - start.tv_usec)/1000000);
	printf("Random Read: %f(s) \n",diff);
	close(f1);
}

void rand_write1(){
	f1= open("1-2.txt", O_RDWR);
	struct timeval start;
	struct timeval end;
	float diff;
	int rand_num=0;
	time_t t;
	lseek(f1, 0, SEEK_SET);
	for (int i=0; i < 3072; i++){
		buffer_rand_write1[i]='b';	
	}

	gettimeofday(&start,NULL);

	srand((unsigned) time(&t));
	for (int i=0; i<50000; i++){
		rand_num= (rand() % (51200));  //共切成51200個格子(rand取0~51199)
		lseek(f1, (rand_num*4096), SEEK_SET);
		if(write(f1, buffer_rand_write1, 3072) == -1){
			printf("random write1: write error!\n");
			return;
		}
	}

	gettimeofday(&end,NULL);

	diff = end.tv_sec-start.tv_sec + ((float)(end.tv_usec - start.tv_usec)/1000000);
	printf("Random Write1: %f(s) \n",diff);
	close(f1);
}

void rand_write2(){
	f1= open("1-2.txt", O_RDWR);
	struct timeval start;
	struct timeval end;
	float diff;
	int rand_num=0;
	time_t t;
	lseek(f1, 0, SEEK_SET);
	for (int i=0; i < 3072; i++){
		buffer_rand_write2[i]='c';	
	}

	gettimeofday(&start,NULL);

	srand((unsigned) time(&t));
	for (int i=0; i<50000; i++){
		rand_num= (rand() % (51200));  //共切成51200個格子(rand取0~51199)
		lseek(f1, (rand_num*4096), SEEK_SET);
		if(write(f1, buffer_rand_write2, 3072) == -1){
			printf("random write2: write error!\n");
			return;
		}
		fsync(f1);
	}

	gettimeofday(&end,NULL);

	diff = end.tv_sec-start.tv_sec + ((float)(end.tv_usec - start.tv_usec)/1000000);
	printf("Random Write2: %f(s) \n",diff);
	close(f1);
}

int main(int argc, char const *argv[])
{
	
	for (int i=0; i<(200*1024*1024); i++){
		buffer_init[i]='0';
	}

	f1 = open("1-2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	write(f1, buffer_init, 200*1024*1024);
	close(f1);

	seq_read();
	seq_write();
	rand_read();
	rand_write1();
	rand_write2();

	return 0;
}
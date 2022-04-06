#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/time.h>
#include <string.h>
#include <time.h>

FILE* f1;
long int size = (200 * 1024 * 1024)-1;
char buffer_init[200*1024*1024];
char buffer_seq_read[200*1024*1024], buffer_seq_write[200*1024*1024], buffer_rand_read[4096];
char buffer_rand_write1[2048], buffer_rand_write2[2048];

void seq_read(){	
	f1= fopen("1-1.txt", "r");
	struct timeval start;
	struct timeval end;
	float diff;
	int check=0, read_count=((200 * 1024 * 1024)/4096);

	gettimeofday(&start,NULL);

	for(int i=0; i<read_count; i++){
		check= fread(buffer_seq_read, 4096, 1, f1);
	}

	gettimeofday(&end,NULL);

	if(check!=1){
		printf("sequential read: fread error!\n");
		return;
	}

	diff = end.tv_sec-start.tv_sec + ((float)(end.tv_usec - start.tv_usec)/1000000);
	printf("Sequential Read: %f(s) \n",diff);

	fclose(f1);
} 

void seq_write(){
	f1= fopen("1-1.txt", "w");

	struct timeval start;
	struct timeval end;
	float diff;
	fseek(f1, 0, SEEK_SET);
	int check=0, write_count=((200 * 1024 * 1024)/2048);
	for (int i=0; i < (200*1024*1024); i++){
		buffer_seq_write[i]='a';	
	}

	gettimeofday(&start,NULL);
	
	for(int i=0; i<write_count; i++){
		check= fwrite(buffer_seq_write, 2048, 1, f1);
		fsync(fileno(f1));
	}
	
	gettimeofday(&end,NULL);

	if(check!=1){
		printf("sequential write: fwrite error!\n");
		return;
	}

	diff = end.tv_sec-start.tv_sec + ((float)(end.tv_usec - start.tv_usec)/1000000);
	printf("Sequential Write: %f(s) \n",diff);
	fclose(f1);
}

void rand_read(){
	f1= fopen("1-1.txt", "r");
	struct timeval start;
	struct timeval end;
	float diff;
	int rand_num=0;
	time_t t;
	fseek(f1, 0, SEEK_SET);

	gettimeofday(&start,NULL);

	srand((unsigned) time(&t));
	for (int i=0; i<50000; i++){
		rand_num= (rand() % (51200));  //共切成51200個格子(rand取0~51199)
		fseek(f1, (rand_num*4096), SEEK_SET); 
		if(fread(buffer_rand_read, 4096, 1, f1) != 1){
			printf("random read: fread error!\n");
			return;
		} 
	}

	gettimeofday(&end,NULL);

	diff = end.tv_sec-start.tv_sec + ((float)(end.tv_usec - start.tv_usec)/1000000);
	printf("Random Read: %f(s) \n",diff);
	fclose(f1);
}

void rand_write1(){
	f1= fopen("1-1.txt", "r+");
	fseek (f1, 0, SEEK_SET);
	struct timeval start;
	struct timeval end;
	float diff;
	int rand_num=0;
	time_t t;
	fseek(f1, 0, SEEK_SET);
	for (int i=0; i < 2048; i++){
		buffer_rand_write1[i]='b';	
	}

	gettimeofday(&start,NULL);

	srand((unsigned) time(&t));
	for (int i=0; i<50000; i++){
		rand_num= (rand() % (51200));  //共切成51200個格子(rand取0~51199)
		fseek(f1, (rand_num*4096), SEEK_SET);
		if(fwrite(buffer_rand_write1, 2048, 1, f1) != 1){
			printf("random write1: fwrite error!\n");
			return;
		}
	}

	gettimeofday(&end,NULL);

	diff = end.tv_sec-start.tv_sec + ((float)(end.tv_usec - start.tv_usec)/1000000);
	printf("Random Write1: %f(s) \n",diff);
	fclose(f1);
}

void rand_write2(){
	f1= fopen("1-1.txt", "r+");
	fseek (f1, 0, SEEK_SET);
	struct timeval start;
	struct timeval end;
	float diff;
	int rand_num=0;
	time_t t;
	fseek(f1, 0, SEEK_SET);
	for (int i=0; i < 2048; i++){
		buffer_rand_write2[i]='c';	
	}

	gettimeofday(&start,NULL);

	srand((unsigned) time(&t));
	for (int i=0; i<50000; i++){
		rand_num= (rand() % (51200));  //共切成51200個格子(rand取0~51199)
		fseek(f1, (rand_num*4096), SEEK_SET);
		if(fwrite(buffer_rand_write2, 2048, 1, f1) != 1){
			printf("random write1: fwrite error!\n");
			return;
		}
		fsync(fileno(f1));
	}

	gettimeofday(&end,NULL);

	diff = end.tv_sec-start.tv_sec + ((float)(end.tv_usec - start.tv_usec)/1000000);
	printf("Random Write2: %f(s) \n",diff);
	fclose(f1);
}

int main(int argc, char const *argv[])
{
	
	for (int i=0; i<(200*1024*1024); i++){
		buffer_init[i]='0';
	}

	f1 = fopen("1-1.txt", "w");
	fwrite(buffer_init, strlen(buffer_init), 1, f1);
	fclose(f1);

	seq_read();
	seq_write();
	rand_read();
	rand_write1();
	rand_write2();

	return 0;
}
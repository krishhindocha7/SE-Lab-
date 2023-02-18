#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <time.h>


#define ARRAY_SIZE 40000000

void merge_sort(float *array, float *tmp, float size);
void merge_r(float *src, float *tmp, float start, float mid, float end);
void ins_sort(float *src, int start, int end);

int main() {
  int i;
  float *src, *tmp;
  struct timeval tv1, tv2;
	struct timezone tz;
	double elapsed=0; 
   src=(float *)malloc(ARRAY_SIZE * sizeof(int));
   tmp=(float *)malloc(ARRAY_SIZE * sizeof(int));
float numTh[9]={1,2,4,6,8,10,12,14,16};
  srand(time(NULL));
  
  // generate random numbers for float type array
  for (int i = 0; i < ARRAY_SIZE; i++) {
    src[i] = rand();
  }

for(int c=0;c<9;c++){
	omp_set_num_threads(numTh[c]);
	//to check if the omp_set_num_threads(this part is correct or not)
	//printf("%d",numTh[c]);
	for(float avg=0;avg<5;avg++){
		gettimeofday(&tv1, &tz);
		#pragma omp parallel 
		 merge_sort(src, tmp, ARRAY_SIZE);
	gettimeofday(&tv2, &tz);
	elapsed += (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6;
	//remove the comment if you need to check if the part is correct
	//printf("elapsed time = %4.2lf seconds for the thread %d.\n", elapsed,numTh[c]);
}
	printf(" average elapsed time = %4.2lf seconds for the thread %f.\n", elapsed/5,numTh[c]);
	//fprintf(fp,"%d\t %lf\n",numTh[c],elapsed/5);
	elapsed=0;
}

 

/*printf("after sorting\n");
for (i = 0; i < ARRAY_SIZE; i++) {
    printf("%d \t",src[i]);
	
  }
printf("\n");*/
 
  free(src);
  free(tmp);

  return 0;
}

void merge_sort(float *src, float *tmp, float size) {
  float i, stride=100;
    for(i=0;i+stride<size;i=i+stride){
		
	    ins_sort(src, i, i+stride-1);
    }
    if(i<size){ ins_sort(src, i, size-1); }

   while(stride<size){	
    for(i=0;i+2*stride<size;i=i+2*stride){
	    merge_r(src, tmp, i, i+stride, i+2*stride-1);
    }
    if(i+stride<size){ merge_r(src,tmp, i, i+stride, size-1); } 
    stride=2*stride;
   }
}
void ins_sort(float *src, int start, int end){
  #pragma omp parallel for
	for( int pos=start+1; pos<=end; pos++){
		int temp=src[pos];
		
		while(pos>start && src[pos-1]>temp){ src[pos]=src[pos-1]; pos=pos-1; }
		if(pos>=start){ src[pos]=temp; }
	}	
}

void merge_r(float *src, float *tmp, float start, float mid, float end) {
int i=start, j=mid, k=start;
	while (i <=mid-1 && j <=end) {
		if (src[i] <= src[j]) { tmp[k++] = src[i++]; }
	       	else { tmp[k++] = src[j++]; }
	}
	while (i<=mid-1) { tmp[k++] = src[i++]; }	
	while (j<=end) { tmp[k++] = src[j++];}
	i=start;
	j=end;
	while(i<=j){ src[i]=tmp[i]; i++;}
}

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

#define SHM_NAME "/my_shared_memory"
#define SEM_NAME "/my_semaphore"

int main(){
	int shm_fd;
	void *ptr;
	sem_t *sem;

	// 세마포어 생성 (초기값:1)
	sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
	if(sem == SEM_FAILED){
		perror("sem_open failed");
		return 1;
	}

	// 공유 메모리 생성
	shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
	if(shm_fd == -1){
		perror("shm_open failed");
		return 1;
	}

	// 공유 메모리 크기 설정
	ftruncate(shm_fd, 1024);

	// 공유 메모리를 매핑
	ptr = mmap(0, 1024, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED){
		perror("mmap failed");
		return 1;
	}

	// 세마포어 잠금
	sem_wait(sem);

	// 공유 메모리에 데이터 쓰기
	char message[] = "Hello from shared memory with semaphore!";
	sprintf(ptr, "%s", message);
	printf("[Writer] Date written : %s\n", message);

	// 세마포어 해제
	sem_post(sem);

	return 0;
}	

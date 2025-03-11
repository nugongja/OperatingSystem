#include <stdatomic.h>
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

	// 세마포어 열기
	sem = sem_open(SEM_NAME, 0);
	if (sem == SEM_FAILED){
		perror("sem_open failed");
		return 1;
	}
	

	// 공유 메모리 열기
	shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
	if (shm_fd == -1){
		perror("shm_open failed");
		return 1;
	}	


	// 공유 메모리 매핑
	ptr = mmap(0, 1024, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED){
		perror("mmap failed");
		return 1;
	}


	// 세마포어 잠금
	sem_wait(sem);
	
	// 공유 메모리에서 데이터 읽기
	printf("[Reader] Date read: %s\n", (char*)ptr);

	// 세마포어 해제
	sem_post(sem);
	
	return 0;
}

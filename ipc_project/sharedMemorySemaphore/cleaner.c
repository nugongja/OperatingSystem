#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>

#define SHM_NAME "/my_shared_memory"
#define SEM_NAME "/my_semaphore"

int main(){
	// 공유 메모리 삭제
	if (shm_unlink(SHM_NAME) == -1){
		perror("shm_unlink failed");
		return 1;
	}

	// 세마포어 삭제
	if (sem_unlink(SEM_NAME) == -1){
		perror("sem_unlink failed");
		return 1;
	}


	printf("[Cleaner] Shared memory and semaphore deleted successfully.\n");
	return 0;
}

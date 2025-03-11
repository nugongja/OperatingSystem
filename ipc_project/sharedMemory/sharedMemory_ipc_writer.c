#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define SHM_NAME "/my_shared_memory"

int main() {
	int shm_fd;
	void *ptr;

	// 공유 메모리 생성 (읽기/쓰기)
	shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
	if (shm_fd == -1) {
		perror("shm_open faild");
		return 1;
	}

	// 공유 메모리 크기 설정 (1024바이트)
	ftruncate(shm_fd, 1024);

	// 공유 메모리를 프로세스 주소 공간에 매핑
	ptr = mmap(0, 1024, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if(ptr == MAP_FAILED) {
		perror("mmap faild");
		return 1;
	}

	char message[] = "Hello from shared memory!";
	sprintf(ptr, "%s", message);
	printf("[Writer] Date written: %s\n", message);

	return 0;
}

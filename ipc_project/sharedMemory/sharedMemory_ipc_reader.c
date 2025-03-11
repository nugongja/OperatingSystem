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

	// 공유 메모리 열기 (읽기 전용)
	shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
	if (shm_fd == -1){
		perror("shm_open faild");
		return 1;
	}

	// 공유 메모리를 프로세스 주소 공간에 매핑
	ptr = mmap(0, 1024, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		perror("mmap faild");
		return 1;
	}

	// 공유 메모리에서 데이터 읽기
	printf("[Reader] Date read: %s\n", (char *)ptr);

	// 공유 메모리 해제 (프로세스 종료 후에도 데이터 유지됨)
	return 0;
}

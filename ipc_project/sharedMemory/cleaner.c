#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define SHM_NAME "/my_shared_memory"

int main() {
	if (shm_unlink(SHM_NAME) == -1) {
		perror("shm_unlink faild");
		return 1;
	}

	printf("[Cleaner] Shared memory deleted successfully.\n");
	return 0;
}

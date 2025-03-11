#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
	int fd[2];  // fd[0] = 읽기, fd[1] = 쓰기
	char message[] = "Hello from parent!";
	char buffer[100];

	if (pipe(fd) == -1){
		perror("Pipe faild");
		return 1;
	}

	pid_t pid = fork();
	if (pid < 0) {
		perror("Fork faild");
		return 1;
	}

	if (pid > 0) { // 부모 프로세스
		close(fd[0]);  // 읽기 끝 닫기
		write(fd[1], message, strlen(message) + 1);
		close(fd[1]);  // 쓰기 끝 닫기
	} else { // 자식 프로세스
		close(fd[1]);
		read(fd[0], buffer, sizeof(buffer));
		printf("Child received: %s\n", buffer);
		close(fd[0]);
	}

	return 0;
}

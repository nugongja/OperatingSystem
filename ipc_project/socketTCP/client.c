#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define PORT 8080

int main(void){
	int sock = 0;
	struct sockaddr_in serv_addr;
	char buffer[1024] = {0};

	// 1. 소켓 생성
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}

	// 2. 서버 주소 설정
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// 3. 서버 IP주소 변환
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) < 0) {
		perror("Invalid address");
		exit(EXIT_FAILURE);
	}
	
	// 4. 서버에 연결 요청
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
		perror("Connection failed");
		exit(EXIT_FAILURE);
	}

	// 5. 서버에 메시지 전송
	char *message = "Hello from client!";
	send(sock, message, strlen(message), 0);
	printf("[Client] Message sent: %s\n", message);

	// 6. 서버로부터 응답 수신
	read(sock, buffer, sizeof(buffer));
	printf("[Client] Received: %s\n", buffer);

	// 7. 소켓 종료
	close(sock);
	return 0;
	
}

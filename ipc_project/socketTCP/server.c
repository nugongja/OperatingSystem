#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main(void){
	int server_fd, new_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};

	// 1. 소켓 생성
	// AF_INET : 주소 체계로서 IPv4를 사용하는 인터넷망에 접속
	// SOCK_STREAM : TCP 소켓
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}

	// 2. 주소 설정
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	

	// 3. 소켓과 주소 바인딩
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}


	// 4. 연결 대기
	if (listen(server_fd, 3) < 0) {
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}	

	printf("[Server] Wating for connections...\n");
	
	// 5. 클라이언트 연결 수락
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
		perror("Accept failed");
		exit(EXIT_FAILURE);
	}
	
	
	
	// 6. 클라이언트로부터 데이터 수신
	read(new_socket, buffer, sizeof(buffer));
	printf("[Server] Received: %s\n", buffer);	


	// 7. 클라이언트에게 응답 전송
	char *response = "Hello from server!";
	send(new_socket, response, strlen(response), 0);
	printf("[Server] Response sent.\n");

	// 8. 소켓 종료
	close(new_socket);
	close(server_fd);
	return 0;
}

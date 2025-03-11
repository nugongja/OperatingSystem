#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MSG_KEY 1234

// 메시지 구조체 정의
struct message {
	long msg_type;
	char msg_text[100];
};

int main(){
	int msg_id;
	struct message msg;

	// 메시지 큐 생성
	msg_id = msgget(MSG_KEY, IPC_CREAT|0666);
	if (msg_id == -1){
		perror("msgget failed");
		return 1;
	}


	// 메시지 작성
	msg.msg_type = 1;
	strcpy(msg.msg_text, "Hello from Message Queue!");


	// 메시지 큐에 전송
	if(msgsnd(msg_id,  &msg, sizeof(msg.msg_text), 0) == -1){
		perror("msgsnd failed");
		return 1;
	};

	printf("[Sender] Message sent: %s\n", msg.msg_text);
	return 0;
}





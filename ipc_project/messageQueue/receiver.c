#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>


#define MSG_KEY 1234

struct message{
	long msg_type;
	char msg_text[100];
};

int main(){
	int msg_id;
	struct message msg;

	msg_id = msgget(MSG_KEY, 0666);
	if (msg_id == -1){
		perror("msgget failed");
		return 1;
	}


	if(msgrcv(msg_id, &msg, sizeof(msg.msg_text), 1, 0) == -1){
		perror("msgrcv failed");
		return 1;
	};

	printf("[Receiver] Message received: %s\n", msg.msg_text);
	return 0;
	
}

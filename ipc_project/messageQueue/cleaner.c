#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>


#define MSG_KEY 1234

int main(){
	int msg_id;
	msg_id = msgget(MSG_KEY, 0666);
	if(msg_id == -1){
		perror("msgget failed");
		return 1;
	}

	if(msgctl(msg_id, IPC_RMID, NULL) == -1){
		perror("msgctl failed");
		return 1;	
	};

	printf("[Cleaner] Message Queue deleted succesfully.\n");
	return 0;
}

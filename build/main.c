#include<stdio.h>
#include "../include/CRAW.h"

int main(){
	CRAW *handle=CRAW_Init("383ZPhpIhjw_PxY5FZEdMA", "nb0appmgc_uRV5s5Tr3XY4JwmFA4MQ", "_SomeTroller69", "Saksham_20092009", "Confession bot by u/_SomeTroller69");
	CRAW_Account *myinfo=CRAW_Account_Init();
	CRAW_Account_me(handle, myinfo);
	printf("my id: %s", myinfo->id);
	return 0;
}

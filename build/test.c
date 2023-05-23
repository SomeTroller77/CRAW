#include<stdio.h>
#include "../include/CRAW.h"

int main(void){
	CRAW *handle=CRAW_Init("383ZPhpIhjw_PxY5FZEdMA", "nb0appmgc_uRV5s5Tr3XY4JwmFA4MQ", "_SomeTroller69", "Saksham_20092009", "Confession bot by u/_SomeTroller69");
	CRAW_Account *myInfo=CRAW_Account_Init();
	CRAWcode res;
	res=CRAW_Account_me(handle, myInfo);
	printf("my id is: %s", myInfo->id);
	printf("\nstatus code: %d", handle->internal->error_code);
	return 0;
}

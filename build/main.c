#include "../include/CRAW.h"
#include<stdio.h>

int main(){
        CRAW *handle=CRAW_Init("383ZPhpIhjw_PxY5FZEdMA", "nb0appmgc_uRV5s5Tr3XY4JwmFA4MQ", "_SomeTroller69", "Saksham_20092009", "Confession bot by u/_SomeTroller69");
        CRAWcode res;
	CRAW_Account *myinfo=CRAW_Account_Init();
	res=CRAW_Account_me(handle, myinfo);
	if(res == CRAW_PARSE_ERROR){
		printf("parsing error"); 
		return 1;
	}
	if(res == CRAW_TOKEN_ERROR){
		printf("token error");
		return 2;
	}
        printf("\n\n\n%s", myinfo->id);
        return 0;
}

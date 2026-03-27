#include <CRAW/CRAW.h>

int main(){
	CRAW *handle=CRAW_Init(
		NULL, 
		NULL, 
		NULL, 
        NULL,
		"C REDDIT API WRAPPER by u/_SomeTroller69",
		false
	);
	if(handle == NULL){
		return 1;
	}
	CRAW_Free(handle);
	return 0;
}

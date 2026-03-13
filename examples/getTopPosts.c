#include<stdio.h>
#include<stdlib.h>
#include<CRAW/CRAW.h>
#include <cjson/cJSON.h>
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
	CRAW_Listing *a = CRAW_Listing_Init();
	CRAW_getTopPosts(handle, a);
	CRAW_Link *test = a->children[2].data;
	printf("post link:- %s\nTotal posts:- %d", test->permalink, a->array_size);
	CRAW_Listing_Free(a);
	CRAW_Free(handle);
	return 0;
}
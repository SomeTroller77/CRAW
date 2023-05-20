#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<curl/curl.h>
#include "../include/CRAW.h"
#include "../include/cJSON.h"
#include<stdlib.h>
#ifdef _WIN32
#include<Windows.h>
#else
#include<unistd.h>
#endif
static size_t cb(void *buf, size_t size, size_t count, void *userp){
	size_t realbytes=count*size;
	struct memory *mem=(struct memory *)userp;
	char *ptr=realloc(mem->response, mem->size+realbytes+1);
	if(ptr==NULL){
		return 0;
	}
	mem->response=ptr;
	memcpy(&(mem->response[mem->size]), buf, realbytes);
	mem->size+=realbytes;
	mem->response[mem->size]=0;
	return realbytes;

}



CRAW *CRAW_Init(const char *client_id, const char *secret_key, const char *username, const char *password, const char *user_agent){
	struct memory chunk={0};
	#ifdef _WIN32
	Sleep(1000);
	#else
	sleep(1);
	#endif
	CRAW *handle=(CRAW*) malloc(sizeof(CRAW)+1);
	handle->internal=(struct internalInfo *)malloc(sizeof(struct internalInfo)+1);
	if(handle == NULL || handle->internal == NULL) {
	   if (handle != NULL) {
		free(handle);
	   }
	return NULL;
        }
	handle->client_id=client_id;
	handle->secret_key=secret_key;
	handle->username=username;
	handle->password=password;
	handle->user_agent=user_agent;
	char *postString;
	sprintf(postString, "grant_type=password&username=%s&password=%s", handle->username, handle->password);
	CURL *curlhandle=curl_easy_init();
	if(!curlhandle){
		return NULL;
	}
	CURLcode res;
	curl_easy_setopt(curlhandle, CURLOPT_URL, "https://www.reddit.com/api/v1/access_token");
	curl_easy_setopt(curlhandle, CURLOPT_USERNAME, handle->client_id);
	curl_easy_setopt(curlhandle, CURLOPT_PASSWORD, handle->secret_key);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, cb);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, (void *)&chunk);
	curl_easy_setopt(curlhandle, CURLOPT_USERAGENT, handle->user_agent);
	curl_easy_setopt(curlhandle, CURLOPT_POSTFIELDS, postString);
	res=curl_easy_perform(curlhandle);
	const cJSON *access_tokenBuf=NULL;
	cJSON *monitor_json=cJSON_Parse(chunk.response);
	access_tokenBuf=cJSON_GetObjectItemCaseSensitive(monitor_json, "access_token");
	if(access_tokenBuf == NULL){
		return NULL;
	}
	handle->internal->access_token=access_tokenBuf->valuestring;
	cJSON_Delete(monitor_json);
	curl_easy_cleanup(curlhandle);
	free(chunk.response); // Free the memory
	return handle;
}

CRAWcode CRAW_free(CRAW *handle){
	free(handle->internal);
	free(handle);
	return CRAW_OK;
}



#include<stdio.h>
#include "CRAW_Account.h"
#include<curl/curl.h>
#include<cjson/cJSON.h>
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

char *grabData(CRAW *handle, const char *url){
        CURL *curlhandle=curl_easy_init();
        CURLcode res;
        struct memory chunk={0};
	char *authString;
	authString="Authorization: bearer ";
	char *buffer=malloc(strlen(authString)+strlen(handle->internal->access_token)+1);
	strcpy(buffer, authString);
	strcat(buffer, handle->internal->access_token);
	#ifdef _WIN32
	Sleep(1000);
	#else
	sleep(1);
	#endif
	struct curl_slist *list=NULL;
        curl_easy_setopt(curlhandle, CURLOPT_URL, url);
        curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, cb);
        curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curlhandle, CURLOPT_USERAGENT, handle->user_agent);
	list=curl_slist_append(list, buffer);
	curl_easy_setopt(curlhandle, CURLOPT_HTTPHEADER, list);
        res=curl_easy_perform(curlhandle);
	curl_slist_free_all(list);
        curl_easy_cleanup(curlhandle);
        return chunk.response;
}

CRAW_Account *CRAW_Account_Init(){
	CRAW_Account *handle=malloc(sizeof(CRAW_Account)+1);
	return handle;
}
CRAWcode CRAW_Account_me(CRAW *handle, CRAW_Account *accHandle) {
	char *json=grabData(handle, "https://oauth.reddit.com/api/v1/me");
	cJSON *monitor_json=cJSON_Parse(json);
	if(monitor_json == NULL){
		return CRAW_ERROR;
	}
	const cJSON *name=NULL;
	const cJSON *total_karma=NULL;
	const cJSON *id=NULL;
	const cJSON *created_utc=NULL;
	name=cJSON_GetObjectItemCaseSensitive(monitor_json, "name");
	accHandle->name=name->valuestring;
	total_karma=cJSON_GetObjectItemCaseSensitive(monitor_json, "total_karma");
	accHandle->total_karma=total_karma->valuedouble;
	created_utc=cJSON_GetObjectItemCaseSensitive(monitor_json, "created_utc");
	accHandle->created_utc=created_utc->valuedouble;
	id=cJSON_GetObjectItemCaseSensitive(monitor_json, "id");
	accHandle->id=id->valuestring;
	cJSON_Delete(monitor_json);
	return CRAW_OK;
}

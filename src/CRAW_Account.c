/*
Copyright 2023 SomeTroller77 / Saksham Vitwekar and the contributers of the CRAW project

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
DO NOT MESS ANYTHING IN ANY FILE AS IT WILL BREAK THE LIBRARY OR CAUSE IT
TO NOT WORK

YOU HAVE BEEN WARNED
*/
#include<stdio.h>
#include<string.h>
#define CRAW_PRIVATE_DO_NOT_MESS
#include "../include/CRAW_PRIVATE.h"
#include<curl/curl.h>
#include "../include/cJSON.h"
#include<stdlib.h>
#ifdef _WIN32
#include<Windows.h>
#else
#include<unistd.h>
#endif

struct memory{
	char *response;
	size_t size;
};

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

static char *grabData(CRAW *handle, const char *url){
        CURL *curlhandle=curl_easy_init();
        CURLcode res;
        struct memory chunk={0};
	#ifdef _WIN32
	Sleep(1000);
	#else
	sleep(1);
	#endif
	long code;
	struct curl_slist *list=NULL;
        curl_easy_setopt(curlhandle, CURLOPT_URL, url);
        curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, cb);
        curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curlhandle, CURLOPT_USERAGENT, handle->user_agent);
	list=curl_slist_append(list, handle->internal->token_header);
	curl_easy_setopt(curlhandle, CURLOPT_HTTPHEADER, list);
        res=curl_easy_perform(curlhandle);
	curl_easy_getinfo(curlhandle, CURLINFO_RESPONSE_CODE, &code);
	handle->internal->error_code=code;
	curl_slist_free_all(list);
	curl_easy_cleanup(curlhandle);
	return chunk.response;
}

static CRAWcode check_http_code(long code){
	if(code >= 200 && code <=399){
                return CRAW_OK;
        }
	else if(code == 400){
		return CRAW_BAD_REQUEST;
	}
	else if(code == 401){
		return CRAW_UNAUTHORISED;
	}
	else if(code == 403){
		return CRAW_FORBIDDEN;
	}
	else if(code == 429){
		return CRAW_TOO_MANY_REQUESTS;
	}
	else{
                return CRAW_UNKNOWN_CODE;
        }
}

CRAW_Account *CRAW_Account_Init() {
	CRAW_Account *handle=malloc(sizeof(CRAW_Account)+1);
	if (handle == NULL) {
	    return NULL;
	}
	return handle;
}

CRAWcode CRAW_Account_me(CRAW *handle, CRAW_Account *accHandle) {
	char *json=grabData(handle, "https://oauth.reddit.com/api/v1/me");
	if (json == NULL) {
	    return check_http_code(handle->internal->error_code);
	    //return CRAW_GRAB_ERROR;  // TO DO: add this error code
	}
	cJSON *monitor_json=cJSON_Parse(json);
	if(monitor_json == NULL){
	    free(json); // Free json before returning
	    return CRAW_PARSE_ERROR;
	}
	const cJSON *error=NULL;
	error=cJSON_GetObjectItemCaseSensitive(monitor_json, "error");
	if(error != NULL){
		return check_http_code(handle->internal->error_code);
	}
	handle->internal->error_code=0;
	const cJSON *name=NULL;
	const cJSON *total_karma=NULL;
	const cJSON *id=NULL;
	const cJSON *created_utc=NULL;
	name=cJSON_GetObjectItemCaseSensitive(monitor_json, "name");
	if(name == NULL){
		return check_http_code(handle->internal->error_code);
	}
	accHandle->name=name->valuestring;
	total_karma=cJSON_GetObjectItemCaseSensitive(monitor_json, "total_karma");
	if(total_karma == NULL){
		return CRAW_TOKEN_ERROR;
	}
	accHandle->total_karma=total_karma->valuedouble;
	created_utc=cJSON_GetObjectItemCaseSensitive(monitor_json, "created_utc");
	if(created_utc == NULL){
		return CRAW_TOKEN_ERROR;
	}
	accHandle->created_utc=created_utc->valuedouble;
	id=cJSON_GetObjectItemCaseSensitive(monitor_json, "id");
	if(id == NULL){
		return CRAW_TOKEN_ERROR;
	}
	accHandle->id=id->valuestring;
	free(json);
	cJSON_Delete(monitor_json);
	return check_http_code(handle->internal->error_code);
}

CRAWcode CRAW_Account_getUserAbout(CRAW *handle, char *username, CRAW_Account *accHandle){
	char *urlString=(char*) malloc(strlen("https://oauth.reddit.com/user/%s/about")+ strlen(username));
	sprintf(urlString, "https://oauth.reddit.com/user/%s/about", username);
	char *json=grabData(handle, urlString);
	free(urlString);
	if(json == NULL){
		return CRAW_GRAB_ERROR;
	}
	cJSON *root=NULL;
	root=cJSON_Parse(json);
	free(json);
	if(root == NULL){
		return CRAW_PARSE_ERROR;
	}
	const cJSON *data=NULL;
	data=cJSON_GetObjectItemCaseSensitive(root, "data");
	const cJSON *name=NULL;
	const cJSON *id=NULL;
	const cJSON *total_karma=NULL;
	const cJSON *created_utc=NULL;
	name=cJSON_GetObjectItemCaseSensitive(data, "name");
	if(name == NULL){
		return check_http_code(handle->internal->error_code);
	}
	accHandle->name=name->valuestring;
	id=cJSON_GetObjectItemCaseSensitive(data, "id");
	if(id == NULL){
		return CRAW_PARSE_ERROR;
	}
	accHandle->id=id->valuestring;
	total_karma=cJSON_GetObjectItemCaseSensitive(data, "total_karma");
	if(total_karma == NULL){
		return CRAW_PARSE_ERROR;
	}
	accHandle->total_karma=(int)total_karma->valuedouble;
	created_utc=cJSON_GetObjectItemCaseSensitive(data, "created_utc");
	if(created_utc == NULL){
		return CRAW_PARSE_ERROR;
	}
	accHandle->created_utc=(long)created_utc->valuedouble;
	cJSON_Delete(root);
	return CRAW_OK;
}

CRAWcode CRAW_Account_free(CRAW_Account *accHandle){
	free(accHandle);
	return CRAW_OK;
}


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


static CRAWcode check_http_code(long code){
        if(code == 400){
                return CRAW_BAD_REQUEST;
        }
        if(code == 401){
                return CRAW_UNAUTHORISED;
        }
        else{
                return CRAW_OK;
        }
}

CRAW *CRAW_Init(const char *client_id, const char *secret_key, const char *username, const char *password, const char *user_agent){
	struct memory chunk={0};
	#ifdef _WIN32
	Sleep(2000);
	#else
	sleep(2);
	#endif
	CRAW *handle=(CRAW*) malloc(sizeof(CRAW)+1);
	handle->internal=(struct internalInfo *)malloc(sizeof(struct internalInfo)+1);
	if(handle == NULL){
		return NULL;
	}
	handle->client_id=client_id;
	handle->secret_key=secret_key;
	handle->username=username;
	handle->password=password;
	handle->user_agent=user_agent;
	handle->internal->access_token=NULL;
	char *baseString="grant_type=password&username=&password=";
	char *postString=(char *) malloc(strlen(handle->username)+strlen(handle->password)+strlen(baseString)+1);
	sprintf(postString, "grant_type=password&username=%s&password=%s", handle->username, handle->password);
	CURL *curlhandle=curl_easy_init();
	if(!curlhandle){
		return NULL;
	}
	CURLcode res;
	chunk.response=NULL;
	curl_easy_setopt(curlhandle, CURLOPT_URL, "https://www.reddit.com/api/v1/access_token");
	curl_easy_setopt(curlhandle, CURLOPT_USERNAME, handle->client_id);
	curl_easy_setopt(curlhandle, CURLOPT_PASSWORD, handle->secret_key);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, cb);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, (void *)&chunk);
	curl_easy_setopt(curlhandle, CURLOPT_USERAGENT, handle->user_agent);
	curl_easy_setopt(curlhandle, CURLOPT_POSTFIELDS, postString);
	res=curl_easy_perform(curlhandle);
	if(chunk.response == NULL){
		return NULL;
	}
	
	const cJSON *access_tokenBuf=NULL;
	cJSON *monitor_json=cJSON_Parse(chunk.response);
	const cJSON *error=NULL;
        error=cJSON_GetObjectItemCaseSensitive(monitor_json, "error");
	if(error != NULL){
                handle->internal->error_code=(int) error->valuedouble;
                return NULL;
        }
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



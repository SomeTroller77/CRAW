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
#include "CRAW_Main.h"
#define CRAW_PRIVATE_DO_NOT_MESS
#include "../include/CRAW_PRIVATE.h"
#include "CRAW_Main.h"
#include<curl/curl.h>
#include "../include/cJSON.h"
#include<stdlib.h>
#include<string.h>
#ifdef _WIN32
#include<Windows.h>
#define SLEEP(time) Sleep(time*1000)
#else
#include<unistd.h>
#ifndef SLEEP
#define SLEEP(time) sleep(time)
#endif
#endif



// i dont even know what the fuck is this shit
size_t hdf(char* b, size_t size, size_t nitems, void *userdata) {
    struct linked_list *abc = (struct linked_list *)userdata;
    while(abc->i != NULL){
        abc=abc->i;
    }
    abc->header=strdup(b);
    abc->i=malloc(sizeof(struct linked_list));
    abc=abc->i;
    abc->header=NULL;
    abc->i=NULL;
    return size*nitems;
}

// the fallback function required by libcurl to store the data and to process it to be stored in a buffer
size_t cb(void *buf, size_t size, size_t count, void *userp){
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
char *getData(CRAW *handle, const char *url){
    CURL *curlhandle=curl_easy_init();
	handle->internal->error_code=0;
    CURLcode res;
    struct memory chunk={0};
	struct curl_slist *list=NULL;
	if(handle->internal->ratelimit_remaining == 0){
		while(handle->internal->ratelimit_reset != 0){
				fprintf(stdout, "\nRatelimit usage has depleted. Waiting for %d seconds", handle->internal->ratelimit_reset);
				SLEEP(1);
				handle->internal->ratelimit_reset--;
			}
        }
		int len = strlen("https://oauth.reddit.com") + strlen(url) + 1;
		char *fullurl = malloc(len);
		snprintf(fullurl, len, "https://oauth.reddit.com%s", url);
        curl_easy_setopt(curlhandle, CURLOPT_URL, fullurl);
        curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, cb);
        curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curlhandle, CURLOPT_USERAGENT, handle->user_agent);
	list=curl_slist_append(list, handle->internal->token_header);
	curl_easy_setopt(curlhandle, CURLOPT_HTTPHEADER, list);
	curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, hdf);
	struct linked_list test;
	test.header=NULL;
	test.i=NULL;
	curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &test);
        res=curl_easy_perform(curlhandle);
	if(res != CURLE_OK){
		fprintf(stdout, "%s\n", curl_easy_strerror(res));
	}
	curl_easy_getinfo(curlhandle, CURLINFO_RESPONSE_CODE, &handle->internal->error_code);
	curl_slist_free_all(list);
	curl_easy_cleanup(curlhandle);
	struct linked_list *current = &test;
	current = test.i;
	int tempint;
	char temp[256];
	while(current != NULL && current->header!=NULL){
		if(sscanf(current->header, "%[^:]: %d", temp, &tempint) == -1){
			return chunk.response;
		}
		if(strcmp(temp, "x-ratelimit-remaining") == 0){
			handle->internal->ratelimit_remaining=tempint;
		}
		if(strcmp(temp, "x-ratelimit-reset") == 0){
			handle->internal->ratelimit_reset=tempint;
		}
		if(strcmp(temp, "x-ratelimit-used") == 0){
			handle->internal->ratelimit_used= tempint;
		}
		if(current->i == NULL){
			return chunk.response;
		}
		else{
			current=current->i;
		}
	}
    	while (current != NULL) {
        	struct linked_list *temp = current;
        	current = current->i;
        	free(temp->header);
       		free(temp);
    	}
	free(fullurl);
	return chunk.response;
}
CRAWcode check_http_code(long code){
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
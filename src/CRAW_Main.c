/*
Copyright 2022-2026 SomeTroller77 / Saksham Vitwekar and the contributers of the CRAW project

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
#include "../include/cJSON.h"
#include "../include/CRAW_Main.h"
#define CRAW_PRIVATE_DO_NOT_MESS
#include "../include/CRAW_PRIVATE.h"
#include<stdlib.h>


// The initialization function which is takes the parameters given and loads in the access token and other things from the endpoint 

CRAW *CRAW_Init(const char *client_id, const char *secret_key, const char *username, const char *password, const char *user_agent){
	// initializing the memory struct for libcurl
	struct memory chunk={0};
	// allocating memory for the struct CRAW which is the base for all the requests to be sent and utilized by other functions
	CRAW *handle=(CRAW*) malloc(sizeof(CRAW)+1);
	if(handle == NULL){
		return NULL; //memory check
	}
	// allocating memory for the member pointer which holds all the important data like access_token
	// this line was the culprit for causing segmentation fault and core dumping and was fixed after 3 years (commit id 6bd780576da45303ed25b7ed906f3c6b59e7d352)
	handle->internal=(struct internalInfo *)calloc(1, (sizeof(*handle->internal)));
	if(handle->internal == NULL){
		return NULL;
	}
	// loading in the parameters in handle for furthur use
	handle->client_id=client_id;
	handle->secret_key=secret_key;
	handle->username=username;
	handle->password=password;
	handle->user_agent=user_agent;
	handle->internal->token_header=NULL;
	handle->internal->ratelimit_remaining=1;
	// preparing the base string and the post string to be used as data for reddit oauth endpoint
	char *baseString="grant_type=password&username=&password=";
	char *postString=(char *) malloc(strlen(handle->username)+strlen(handle->password)+strlen(baseString)+1);
	sprintf(postString, "grant_type=password&username=%s&password=%s", handle->username, handle->password);
	CURL *curlhandle=curl_easy_init();
	if(!curlhandle){
		return NULL;
	}
	CURLcode res;
	chunk.response=NULL;
	// setting curl parameters
	curl_easy_setopt(curlhandle, CURLOPT_URL, "https://www.reddit.com/api/v1/access_token");
	curl_easy_setopt(curlhandle, CURLOPT_USERNAME, handle->client_id);
	curl_easy_setopt(curlhandle, CURLOPT_PASSWORD, handle->secret_key);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, cb);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, (void *)&chunk);
	curl_easy_setopt(curlhandle, CURLOPT_USERAGENT, handle->user_agent);
	curl_easy_setopt(curlhandle, CURLOPT_POSTFIELDS, postString);
	// sending the request
	res=curl_easy_perform(curlhandle);
	if(chunk.response == NULL){
		return NULL;
	}
	// storing the HTTP response code
	curl_easy_getinfo(curlhandle, CURLINFO_RESPONSE_CODE, &handle->internal->error_code);
	// initializing cJSON
	const cJSON *access_tokenBuf=NULL;
	cJSON *monitor_json=cJSON_Parse(chunk.response);
	access_tokenBuf=cJSON_GetObjectItemCaseSensitive(monitor_json, "access_token");
	if(access_tokenBuf == NULL){
		return NULL;
	}
	handle->internal->token_header=(char*) malloc(strlen("Authorization: bearer ")+strlen(access_tokenBuf->valuestring)+1);
	// copying and concatenating token in the auth header format for furthur use
	strcpy((char*)handle->internal->token_header, "Authorization: bearer ");
	strcat((char*)handle->internal->token_header, access_tokenBuf->valuestring);
	// cleaning memory
	cJSON_Delete(monitor_json);
	curl_easy_cleanup(curlhandle);
	free(postString);
	free(chunk.response); // Free the memory
	return handle;
}
CRAWcode CRAW_getTopPosts(CRAW *handle, CRAW_Listing *list){
    // sending the fucking request
    char *json = getData(handle, "/top");
   
    const cJSON *data = NULL;
    
    // parsing the received json
    cJSON *root = cJSON_Parse(json);
    // ptr safety
    if(root == NULL){
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL){
            fprintf(stderr, "Error before: %s\n", error_ptr);
            fflush(stderr);
        }
        return CRAW_PARSE_ERROR;
    }
    // grabbing the main "data" object
    data = cJSON_GetObjectItemCaseSensitive(root, "data");
    if(data == NULL){
        #ifdef CRAW_DEBUG_MODE
        printf("Data is empty");
        #endif
        return CRAW_PARSE_ERROR;
    }
    CRAW_load_listing(data, list);
    // freeing the memory
    cJSON_Delete(root);
    free(json);
    return CRAW_OK;
}

CRAWcode CRAW_getNewPosts(CRAW *handle, CRAW_Listing *list){
    // sending the fucking request
    char *json = getData(handle, "/new");
   
    const cJSON *data = NULL;
    
    // parsing the received json
    cJSON *root = cJSON_Parse(json);
    // ptr safety
    if(root == NULL){
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL){
            fprintf(stderr, "Error before: %s\n", error_ptr);
            fflush(stderr);
        }
        return CRAW_PARSE_ERROR;
    }
    // grabbing the main "data" object
    data = cJSON_GetObjectItemCaseSensitive(root, "data");
    if(data == NULL){
        #ifdef CRAW_DEBUG_MODE
        printf("Data is empty");
        #endif
        return CRAW_PARSE_ERROR;
    }
    CRAW_load_listing(data, list);
    // freeing the memory
    cJSON_Delete(root);
    free(json);
    return CRAW_OK;
}

CRAWcode CRAW_getRisingPosts(CRAW *handle, CRAW_Listing *list){
    // sending the fucking request
    char *json = getData(handle, "/rising");
   
    const cJSON *data = NULL;
    
    // parsing the received json
    cJSON *root = cJSON_Parse(json);
    // ptr safety
    if(root == NULL){
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL){
            fprintf(stderr, "Error before: %s\n", error_ptr);
            fflush(stderr);
        }
        return CRAW_PARSE_ERROR;
    }
    // grabbing the main "data" object
    data = cJSON_GetObjectItemCaseSensitive(root, "data");
    if(data == NULL){
        #ifdef CRAW_DEBUG_MODE
        printf("Data is empty");
        #endif
        return CRAW_PARSE_ERROR;
    }
    CRAW_load_listing(data, list);
    // freeing the memory
    cJSON_Delete(root);
    free(json);
    return CRAW_OK;
}
// just a random ass free function i thought would be useful
// To be used when CRAW is not needed, or at the end of the program
void CRAW_Free(CRAW *handle){
	free(handle->internal->token_header);
	free(handle->internal);
	free(handle);
}


// fun fact:- the comments are being added after 3 years of starting the project, well you know what they say, its never too late

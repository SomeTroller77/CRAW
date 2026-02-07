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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "../include/cJSON.h"
#define CRAW_PRIVATE_DO_NOT_MESS
#include "../include/CRAW_PRIVATE.h"
#include <stdlib.h>
#include<string.h>
#include "../include/CRAW.h"


// initialization function for the struct CRAW_Subreddit, returns a pointer
CRAW_Subreddit *CRAW_Subreddit_Init(){
    return calloc(1, sizeof(CRAW_Subreddit));
}

// get info about a subreddit
CRAWcode CRAW_Subreddit_getInfo(CRAW *handle, CRAW_Subreddit *subreddit,char *subreddit_name){
    // adding the strlen of all base url and subreddit name to be used to allocate memory
    size_t len = strlen("/r//about/") + strlen(subreddit_name) + 1;
    char *suburl = malloc(len);
    // string interpolation
    snprintf(suburl, len, "/r/%s/about/", subreddit_name);
    // sending the fucking request
    char *json = getData(handle, suburl);
   
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
        free(suburl);
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
    CRAW_load_subreddit(data, subreddit);
    // freeing the memory
    cJSON_Delete(root);
    free(json);
    free(suburl);
    return CRAW_OK;
}

CRAWcode CRAW_Subreddit_getHotPosts(CRAW *handle, CRAW_Listing *list, char *subreddit_name){
    // adding the strlen of all base url and subreddit name to be used to allocate memory
    size_t len = strlen("/r//hot/") + strlen(subreddit_name) + 1;
    char *suburl = malloc(len);
    // string interpolation
    snprintf(suburl, len, "/r/%s/hot/", subreddit_name);
    // sending the fucking request
    char *json = getData(handle, suburl);
   
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
        free(suburl);
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
    free(suburl);
    return CRAW_OK;
}
CRAWcode CRAW_Subreddit_getNewPosts(CRAW *handle, CRAW_Listing *list, char *subreddit_name){
    // adding the strlen of all base url and subreddit name to be used to allocate memory
    size_t len = strlen("/r//new/") + strlen(subreddit_name) + 1;
    char *suburl = malloc(len);
    // string interpolation
    snprintf(suburl, len, "/r/%s/new/", subreddit_name);
    // sending the fucking request
    char *json = getData(handle, suburl);
   
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
        free(suburl);
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
    free(suburl);
    return CRAW_OK;
}
CRAWcode CRAW_Subreddit_getRisingPosts(CRAW *handle, CRAW_Listing *list, char *subreddit_name){
    // adding the strlen of all base url and subreddit name to be used to allocate memory
    size_t len = strlen("/r//rising/") + strlen(subreddit_name) + 1;
    char *suburl = malloc(len);
    // string interpolation
    snprintf(suburl, len, "/r/%s/rising/", subreddit_name);
    // sending the fucking request
    char *json = getData(handle, suburl);
   
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
        free(suburl);
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
    free(suburl);
    return CRAW_OK;
}
CRAWcode CRAW_Subreddit_getTopPosts(CRAW *handle, CRAW_Listing *list, char *subreddit_name){
    // adding the strlen of all base url and subreddit name to be used to allocate memory
    size_t len = strlen("/r//top/") + strlen(subreddit_name) + 1;
    char *suburl = malloc(len);
    // string interpolation
    snprintf(suburl, len, "/r/%s/top/", subreddit_name);
    // sending the fucking request
    char *json = getData(handle, suburl);
   
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
        free(suburl);
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
    free(suburl);
    return CRAW_OK;
}

// the function to free a CRAW_Subreddit pointer safely and efficiently
void CRAW_Subreddit_Free(CRAW_Subreddit *ptr){
    if(!ptr) return;
    free(ptr->description);
    free(ptr->display_name);
    free(ptr->header_img);
    free(ptr->header_title);
    free(ptr->public_description);
    free(ptr->submission_type);
    free(ptr->submit_link_label);
    free(ptr->submit_text_label);
    free(ptr->title);
    free(ptr->url);
    free(ptr);
}
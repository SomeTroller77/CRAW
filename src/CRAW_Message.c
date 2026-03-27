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
#include <cjson/cJSON.h>
#define CRAW_PRIVATE_DO_NOT_MESS
#include "../include/CRAW_PRIVATE.h"
#include <stdlib.h>
#include<string.h>
#include "../include/CRAW.h"

CRAW_Message *CRAW_Message_Init(void){
    return malloc(sizeof(CRAW_Message));
}

CRAWcode CRAW_Message_getInbox(const CRAW *handle, CRAW_Listing *ptr){
    char *raw = getData(handle, "/message/inbox");
    if(raw == NULL){
        return CRAW_GRAB_ERROR;
    }
    cJSON *root = cJSON_Parse(raw);
    if(root == NULL){
        return CRAW_PARSE_ERROR;
    }
    const cJSON *data = cJSON_GetObjectItemCaseSensitive(root, "data");
    if(data == NULL){
        return CRAW_PARSE_ERROR;
    }
    CRAW_load_listing(data, ptr);
    cJSON_Delete(root);
    free(raw);
    return CRAW_OK;
}

void CRAW_Message_free(CRAW_Message *handle){
    free(handle->author);
    free(handle->body);
    free(handle->context);
    free(handle->first_message);
    free(handle->first_message_name);
    free(handle->link_title);
    free(handle->name);
    free(handle->parent_id);
    free(handle->replies);
    free(handle->subject);
    free(handle->subreddit);
    free(handle);
}

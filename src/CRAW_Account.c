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
#include "CRAW_Main.h"
#define CRAW_PRIVATE_DO_NOT_MESS
#include "../include/CRAW_PRIVATE.h"
#include "CRAW_Main.h"
#include<curl/curl.h>
#include "../include/cJSON.h"
#include<stdlib.h>
#include<string.h>
#include "../include/CRAW_Account.h"
#ifdef _WIN32
#include<Windows.h>
#define SLEEP(time) Sleep(time*1000)
#else
#include<unistd.h>
#endif

// initializing the CRAW_Account struct
CRAW_Account *CRAW_Account_Init(){
	CRAW_Account *handle=malloc(sizeof(CRAW_Account));
	if (handle == NULL){
	    return NULL;
	}/*
	handle->name="\0";
	handle->total_karma=-1;
	handle->id="\0";
	handle->created_utc=-1;*/
	return handle;
}

// to grab data about the logged in account 
CRAWcode CRAW_Account_me(CRAW *handle, CRAW_Account *accHandle) {
	char *json=getData(handle, "/api/v1/me");
	if (json == NULL) {
	    return check_http_code(handle->internal->error_code);
	    return CRAW_GRAB_ERROR;  
	}
	cJSON *monitor_json=cJSON_Parse(json); // parsing the json through cJSON
	if(monitor_json == NULL){
	    free(json); // Free json before returning
	    return CRAW_PARSE_ERROR;
	}
	const cJSON *error=NULL;
	error=cJSON_GetObjectItemCaseSensitive(monitor_json, "error"); //if error is present, grab its info
	if(error != NULL){
		return check_http_code(handle->internal->error_code); //returning the error as a CRAWcode
	}
	const cJSON *data = NULL;
	data = cJSON_GetObjectItemCaseSensitive(monitor_json, "data");
	if(data == NULL){
		#ifdef CRAW_DEBUG_MODE
		printf("data not found");
		#endif
		return CRAW_PARSE_ERROR;
	}
	CRAW_load_account(data, accHandle);
	// freeing the data as im nice enough to not cause a memory leak and cause your computer to slow down :D
	cJSON_Delete(monitor_json);
	free(json);
	return check_http_code(handle->internal->error_code);
}

// grabbing someone elses account info (disclaimer: stalking is bad kids)
CRAWcode CRAW_Account_getAbout(CRAW *handle, char *username, CRAW_Account *accHandle){
	// preparing urlString to concatenate the username in the endpoint
	char *urlString=(char*) malloc(strlen("/user//about")+ strlen(username) + 1);
	sprintf(urlString, "/user/%s/about", username); // concatenation
	//sending the request
	char *json=getData(handle, urlString);
	//freeing the malloc'ed urlString
	free(urlString);
	if(json == NULL){
		return CRAW_GRAB_ERROR;
	}
	// initializng cJSON
	cJSON *root=NULL;
	root=cJSON_Parse(json);
	if(root == NULL){
		free(json);
		return CRAW_PARSE_ERROR;
	}
	const cJSON *data=NULL;
	// getting the data object to take user information from it
	data=cJSON_GetObjectItemCaseSensitive(root, "data");
	if(data == NULL){
		#ifdef CRAW_DEBUG_MODE
		printf("data not found");
		#endif
		return CRAW_PARSE_ERROR;
	}
	CRAW_load_account(data, accHandle);
	// freeing up the pointers
	cJSON_Delete(root);
	free(json);
	return CRAW_OK;
}

// another random ass free function
// To be used if CRAW_Account pointer is no longer needed
void CRAW_Account_Free(CRAW_Account *accHandle){
	free(accHandle->name);
	free(accHandle->id);
	free(accHandle);
}

/*
fun fact:- the initials of the program (CRAW_Main.c, CRAW header files, CRAW_Account.c) were created on an android device through the application termux because
the developer didnt have a pc but still wanted to program :)
*/

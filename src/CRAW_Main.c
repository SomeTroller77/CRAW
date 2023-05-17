#include<stdio.h>
#include<stdlib.h>
#include<curl/curl.h>
#include "CRAW.h"
#include<cjson/cJSON.h>

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
	CRAW *handle=(CRAW*) malloc(sizeof(CRAW));
	if(handle == NULL) {
		return NULL;
	}
	handle->client_id=client_id;
	handle->secret_key=secret_key;
	handle->username=username;
	handle->password=password;
	handle->user_agent=user_agent;
	char *postString;
	postString=replaceWord("grant_type=password&username=reddit_bot&password=snoo", "reddit_bot", handle->username);
	postString=replaceWord(postString, "snoo", handle->password);
	CURL *curlhandle=curl_easy_init();
	CURLcode res;
	curl_easy_setopt(curlhandle, CURLOPT_URL, "https://www.reddit.com/api/v1/access_token");
	curl_easy_setopt(curlhandle, CURLOPT_USERNAME, handle->client_id);
	curl_easy_setopt(curlhandle, CURLOPT_PASSWORD, handle->secret_key);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, cb);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, (void *)&chunk);
	curl_easy_setopt(curlhandle, CURLOPT_USERAGENT, handle->user_agent);
	curl_easy_setopt(curlhandle, CURLOPT_POSTFIELDS, postString);
	res=curl_easy_perform(curlhandle);
	const cJSON *access_tokenTest=NULL;
	cJSON *monitor_json=cJSON_Parse(chunk.response);
	access_tokenTest=cJSON_GetObjectItemCaseSensitive(monitor_json, "access_token");
	handle->access_token=access_tokenTest->valuestring;
	curl_easy_cleanup(curlhandle);
	return handle;
}



static char *replaceWord(const char* s, const char* oldW, const char* newW)
{
    char* result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);

    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;

            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }

    // Making new string of enough length
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);

    i = 0;
    while (*s) {
        // compare the substring with the result
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }

    result[i] = '\0';
    return result;
}

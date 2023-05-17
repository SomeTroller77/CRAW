#include<stdio.h>
#include "CRAW_Account.h"
#include<curl/curl.h>

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
	char *buffer=malloc(strlen(authString)+strlen(handle->access_token)+1);
	strcpy(buffer, authString);
	strcat(buffer, handle->access_token);
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

char *CRAW_Account_me(CRAW *handle) {
	char *json=grabData(handle, "https://oauth.reddit.com/api/v1/me");
	return json;
}

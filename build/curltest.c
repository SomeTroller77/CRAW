#include<stdio.h>
#include<curl/curl.h>

int main(void){
	long code;
	CURL *fd=curl_easy_init();
	CURLcode res;
	curl_easy_setopt(fd, CURLOPT_URL, "https://reddit.com/api/v1/access_token");
	res=curl_easy_perform(fd);
	curl_easy_getinfo(fd, CURLINFO_RESPONSE_CODE, &code);
	printf("%ld", code);
	curl_easy_cleanup(fd);
	return 0;
}


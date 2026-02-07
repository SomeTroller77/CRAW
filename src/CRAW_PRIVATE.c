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
#include "CRAW.h"
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
	// setting the curl opts
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
	// scanning for headers to take the ratelimit headers
	while(current != NULL && current->header!=NULL){
		if(sscanf(current->header, "%[^:]: %d", temp, &tempint) == -1 || current == NULL){
			break;
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
		current=current->i;
	}
	free(test.header);
	current = test.i;
	// freeing the linked list nodes
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
void CRAW_load_account(const cJSON *data, CRAW_Account *ptr){
	const cJSON *name=NULL;
	const cJSON *total_karma=NULL;
	const cJSON *id=NULL;
	const cJSON *created_utc=NULL;
	name=cJSON_GetObjectItemCaseSensitive(data, "name");
	if(!cJSON_IsString(name) || name->valuestring == NULL){
		#ifdef CRAW_DEBUG_MODE
		printf("name not found");
		#endif
	}else{
		ptr->name=strdup(name->valuestring);
	} //strdup because i tried to use malloc and after i freed name, it started to cause Seg Fault, dont blame me i was 12 years old
	total_karma=cJSON_GetObjectItemCaseSensitive(data, "total_karma");
	if(!cJSON_IsNumber(total_karma)){
		#ifdef CRAW_DEBUG_MODE
		printf("total_karma not found");
		#endif
	}else{
		ptr->total_karma=total_karma->valueint;
	}
	
	created_utc=cJSON_GetObjectItemCaseSensitive(data, "created_utc");
	if(!cJSON_IsNumber(created_utc)){
		#ifdef CRAW_DEBUG_MODE
		printf("created_utc not found");
		#endif
	}else {
		ptr->created_utc=(long)created_utc->valuedouble;
	}
	id=cJSON_GetObjectItemCaseSensitive(data, "id");
	if(!cJSON_IsString(id) || id->valuestring == NULL){
		#ifdef CRAW_DEBUG_MODE
		printf("id not found");
		#endif
	}else{
		ptr->id=strdup(id->valuestring);
	} //same as name

}
void CRAW_load_subreddit(const cJSON *data, CRAW_Subreddit *ptr){
	// initializing the cJSON ptrs to extract values from the json
	const cJSON *comment_score_hide_mins = NULL;
    const cJSON *description = NULL;
    const cJSON *display_name = NULL;
    const cJSON *header_img = NULL;
    const cJSON *header_title = NULL;
    const cJSON *over18 = NULL;
    const cJSON *public_description = NULL;
    const cJSON *public_traffic = NULL;
    const cJSON *subscribers = NULL;
    const cJSON *submission_type = NULL;
    const cJSON *submit_link_label = NULL;
    const cJSON *submit_text_label = NULL;
    const cJSON *subreddit_type = NULL;
    const cJSON *title = NULL;
    const cJSON *url = NULL;
    const cJSON *created_utc = NULL;
    const cJSON *is_user_banned = NULL;
    const cJSON *is_user_contributor = NULL;
    const cJSON *is_user_moderator = NULL;
    const cJSON *is_user_subscriber = NULL;
	// grabbing the data, verifying its data type and storing it in the given CRAW_Subreddit pointer safely
    comment_score_hide_mins = cJSON_GetObjectItemCaseSensitive(data, "comment_score_hide_mins");
    if(!cJSON_IsNumber(comment_score_hide_mins)){
        #ifdef CRAW_DEBUG_MODE
        printf("comment_score_hide_mins not found");
        #endif
    }else{
        ptr->comment_score_hide_mins = comment_score_hide_mins->valueint;
    }
    description = cJSON_GetObjectItemCaseSensitive(data, "description");
    if(!cJSON_IsString(description) || description->valuestring == NULL){
        #ifdef CRAW_DEBUG_MODE
        printf("description not found");
        #endif
    }else{
        ptr->description = strdup(description->valuestring);
    }
    display_name = cJSON_GetObjectItemCaseSensitive(data, "display_name");
    if(!cJSON_IsString(display_name) || display_name->valuestring == NULL){
        #ifdef CRAW_DEBUG_MODE
        printf("display_name not found");
        #endif
    }else{
        ptr->display_name = strdup(display_name->valuestring);
    }
    header_img = cJSON_GetObjectItemCaseSensitive(data, "header_img");
    if(!cJSON_IsString(header_img) || header_img->valuestring == NULL){
        #ifdef CRAW_DEBUG_MODE
        printf("header_image not found");
        #endif
    }else{
        ptr->header_img = strdup(header_img->valuestring);
    }
    header_title = cJSON_GetObjectItemCaseSensitive(data, "header_title");
    if(!cJSON_IsString(header_title) || header_title->valuestring == NULL){
        #ifdef CRAW_DEBUG_MODE
        printf("header_title not found");
        #endif
    }else{
        ptr->header_title = strdup(header_title->valuestring);
    }
    over18 = cJSON_GetObjectItemCaseSensitive(data, "over18");
    if(!cJSON_IsBool(over18)){
        #ifdef CRAW_DEBUG_MODE
        printf("over18 not found");
        #endif
    }else{
        ptr->over18 = over18->valueint;
    }
    public_description = cJSON_GetObjectItemCaseSensitive(data, "public_description");
    if(!cJSON_IsString(public_description) || public_description->valuestring == NULL){
        #ifdef CRAW_DEBUG_MODE
        printf("public_description not found");
        #endif
    }else{
        ptr->public_description = strdup(public_description->valuestring);
    }
    public_traffic = cJSON_GetObjectItemCaseSensitive(data, "public_traffic");
    if(!cJSON_IsBool(public_traffic)){
        #ifdef CRAW_DEBUG_MODE
        printf("public_traffic not found");
        #endif
    }else{
        ptr->public_traffic = public_traffic->valueint;
    }
    subscribers = cJSON_GetObjectItemCaseSensitive(data, "subscribers");
    if(!cJSON_IsNumber(subscribers)){
        #ifdef CRAW_DEBUG_MODE
        printf("subscribers not found");
        #endif
    }else{
        ptr->subscribers = subscribers->valueint;
    }
    submission_type = cJSON_GetObjectItemCaseSensitive(data, "submission_type");
    if(!cJSON_IsString(submission_type) || submission_type->valuestring == NULL){
        #ifdef CRAW_DEBUG_MODE
        printf("submission_type not found");
        #endif
    }else{
        ptr->submission_type = strdup(submission_type->valuestring);
    }
    submit_link_label = cJSON_GetObjectItemCaseSensitive(data, "submit_link_label");
    if(!cJSON_IsString(submit_link_label) ||submit_link_label->valuestring == NULL){
        #ifdef CRAW_DEBUG_MODE
        printf("submit_link_label not found");
        #endif
    }else{
        ptr->submit_link_label = strdup(submit_link_label->valuestring);
    }
    submit_text_label = cJSON_GetObjectItemCaseSensitive(data, "submit_text_label");
    if(!cJSON_IsString(submit_text_label) || submit_text_label->valuestring == NULL){
        #ifdef CRAW_DEBUG_MODE
        printf("submit_text_label not found");
        #endif
    }else{
        ptr->submit_text_label = strdup(submit_text_label->valuestring);
    }
    title = cJSON_GetObjectItemCaseSensitive(data, "title");
    if(!cJSON_IsString(title) || title->valuestring == NULL){
        #ifdef CRAW_DEBUG_MODE
        printf("title not found");
        #endif
    }else{
        ptr->title = strdup(title->valuestring);
    }
    url = cJSON_GetObjectItemCaseSensitive(data, "url");
    if(!cJSON_IsString(url) || url->valuestring == NULL){
        #ifdef CRAW_DEBUG_MODE
        printf("url not found");
        #endif
    }else{
        ptr->url = strdup(url->valuestring);
    }
    is_user_banned = cJSON_GetObjectItemCaseSensitive(data, "user_is_banned");
    if(!cJSON_IsBool(is_user_banned)){
        #ifdef CRAW_DEBUG_MODE
        printf("is_user_banned not found");
        #endif
    }else{
        ptr->is_user_banned = is_user_banned->valueint;
    }
    is_user_contributor = cJSON_GetObjectItemCaseSensitive(data, "user_is_contributor");
    if(!cJSON_IsBool(is_user_contributor)){
        #ifdef CRAW_DEBUG_MODE
        printf("is_user_contributor not found");
        #endif
    }else{
        ptr->is_user_contributor = is_user_contributor->valueint;
    }
    is_user_moderator = cJSON_GetObjectItemCaseSensitive(data, "user_is_moderator");
    if(!cJSON_IsBool(is_user_moderator)){
        #ifdef CRAW_DEBUG_MODE
        printf("is_user_moderator not found");
        #endif
    }else{
        ptr->is_user_moderator = is_user_moderator->valueint;
    }
    is_user_subscriber = cJSON_GetObjectItemCaseSensitive(data, "user_is_subscriber");
    if(!cJSON_IsBool(is_user_subscriber)){
        #ifdef CRAW_DEBUG_MODE
        printf("is_user_subscriber not found");
        #endif
    }else{
        ptr->is_user_subscriber = is_user_subscriber->valueint;
    }
    created_utc = cJSON_GetObjectItemCaseSensitive(data, "created_utc");
    if(!cJSON_IsNumber(created_utc)){
        #ifdef CRAW_DEBUG_MODE
        printf("created_utc not found");
        #endif
    }else{
        ptr->created_utc = created_utc->valueint;
    }
    subreddit_type = cJSON_GetObjectItemCaseSensitive(data, "subreddit_type");
    if(!cJSON_IsString(subreddit_type) || subreddit_type->valuestring == NULL){
        #ifdef CRAW_DEBUG_MODE
        printf("subreddit_type not found");
        #endif
    }
    // comparing the value of the subreddit_type field and storing it in the form of enum CRAW_SUBREDDIT_TYPE (see CRAW.h for more info)
    if(strcmp(subreddit_type->valuestring, "public") == 0){
        ptr->subreddit_type = CRAW_SUBREDDIT_PUBLIC;
    }else if(strcmp(subreddit_type->valuestring, "private" ) == 0){
        ptr->subreddit_type = CRAW_SUBREDDIT_PRIVATE;
    }else if(strcmp(subreddit_type->valuestring, "restricted") == 0){
        ptr->subreddit_type = CRAW_SUBREDDIT_RESTRICTED;
    }else if(strcmp(subreddit_type->valuestring, "gold_restricted") == 0){
        ptr->subreddit_type = CRAW_SUBREDDIT_GOLD_RESTRICTED;
    }else if(strcmp(subreddit_type->valuestring, "archived") == 0){
        ptr->subreddit_type = CRAW_SUBREDDIT_ARCHIVED;
    }
}
void CRAW_load_link(const cJSON *data, CRAW_Link *ptr){
	// initializing the variables needed for the fields
	const cJSON *author = NULL;
	const cJSON *author_fullname = NULL;
	const cJSON *domain = NULL;
	const cJSON *hidden = NULL;
	const cJSON *is_self = NULL;
	const cJSON *likes = NULL;
	const cJSON *is_locked = NULL;
	const cJSON *num_comments = NULL;
	const cJSON *over_18 = NULL;
	const cJSON *permalink = NULL;
	const cJSON *is_saved = NULL;
	const cJSON *score = NULL;
	const cJSON *selftext = NULL;
	const cJSON *subreddit = NULL;
	const cJSON *subreddit_id = NULL;
	const cJSON *thumbnail = NULL;
	const cJSON *title = NULL;
	const cJSON *url = NULL;
	const cJSON *edited = NULL;
	const cJSON *is_stickied = NULL;
	// using cJSON to parse the json
	author = cJSON_GetObjectItemCaseSensitive(data, "author");
	if(!cJSON_IsString(author) || author->valuestring == NULL){
		#ifdef CRAW_DEBUG_MODE
		printf("author not found");
		#endif
	}else{
		ptr->author = strdup(author->valuestring);
	}
	author_fullname = cJSON_GetObjectItemCaseSensitive(data, "author_fullname");
	if(!cJSON_IsString(author_fullname) || author_fullname->valuestring == NULL){
		#ifdef CRAW_DEBUG_MODE
		printf("author_fullname not found");
		#endif
	}else{
		ptr->author_fullname = strdup(author_fullname->valuestring);
	}
	domain = cJSON_GetObjectItemCaseSensitive(data, "domain");
	if(!cJSON_IsString(domain) || domain->valuestring == NULL){
		#ifdef CRAW_DEBUG_MODE
		printf("domain not found");
		#endif
	}{
		ptr->domain = strdup(domain->valuestring);
	}
	hidden = cJSON_GetObjectItemCaseSensitive(data, "hidden");
	if(!cJSON_IsBool(hidden)){
		#ifdef CRAW_DEBUG_MODE
		printf("hidden not found");
		#endif
	}else{
		ptr->hidden = hidden->valueint;
	}
	is_self = cJSON_GetObjectItemCaseSensitive(data, "is_self");
	if(!cJSON_IsBool(is_self)){
		#ifdef CRAW_DEBUG_MODE
		printf("is_self not found");
		#endif
	}else{
		ptr->is_self = is_self->valueint;
	}
	likes = cJSON_GetObjectItemCaseSensitive(data, "likes");
	if(!cJSON_IsNumber(likes)){
		ptr->vote_status = CRAW_NO_VOTE;
	}else{
		if(likes->valueint == true){
			ptr->vote_status = CRAW_UPVOTED;
		}else{
			ptr->vote_status = CRAW_DOWNVOTED;
		}
	}
	is_locked = cJSON_GetObjectItemCaseSensitive(data, "locked");
	if(!cJSON_IsBool(is_locked)){
		#ifdef CRAW_DEBUG_MODE
		printf("is_locked not found");
		#endif
	}else{
		ptr->is_locked = is_locked->valueint;
	}
	num_comments = cJSON_GetObjectItemCaseSensitive(data, "num_comments");
	if(!cJSON_IsNumber(num_comments)){
		#ifdef CRAW_DEBUG_MODE
		printf("num_comments not found");
		#endif
	}else{
		ptr->num_comments = num_comments->valueint;
	}
	over_18 = cJSON_GetObjectItemCaseSensitive(data, "over_18");
	if(!cJSON_IsBool(over_18)){
		#ifdef CRAW_DEBUG_MODE
		printf("over_18 not found");
		#endif
	}else{
		ptr->over_18 = over_18->valueint;
	}
	permalink = cJSON_GetObjectItemCaseSensitive(data, "permalink");
	if(!cJSON_IsString(permalink) || permalink->valuestring == NULL){
		#ifdef CRAW_DEBUG_MODE
		printf("permalink not found");
		#endif
	}else{
		ptr->permalink = strdup(permalink->valuestring);
	}
	is_saved = cJSON_GetObjectItemCaseSensitive(data, "saved");
	if(!cJSON_IsBool(is_saved)){
		#ifdef CRAW_DEBUG_MODE
		printf("saved not found");
		#endif
	}else{
		ptr->is_saved = is_saved->valueint;
	}
	score = cJSON_GetObjectItemCaseSensitive(data, "score");
	if(!cJSON_IsNumber(score)){
		#ifdef CRAW_DEBUG_MODE
		printf("score not found");
		#endif
	}else{
		ptr->score = score->valueint;
	}
	selftext = cJSON_GetObjectItemCaseSensitive(data, "selftext");
	if(!cJSON_IsString(selftext) || selftext->valuestring == NULL){
		#ifdef CRAW_DEBUG_MODE
		printf("selftext not found");
		#endif
	}else{
		ptr->selftext = strdup(selftext->valuestring);
	}
	subreddit = cJSON_GetObjectItemCaseSensitive(data, "subreddit");
	if(!cJSON_IsString(subreddit) || subreddit->valuestring == NULL){
		#ifdef CRAW_DEBUG_MODE
		printf("subreddit not found");
		#endif
	}else{
		ptr->subreddit = strdup(subreddit->valuestring);
	}
	subreddit_id = cJSON_GetObjectItemCaseSensitive(data, "subreddit_id");
	if(!cJSON_IsString(subreddit_id) || subreddit_id->valuestring == NULL){
		#ifdef CRAW_DEBUG_MODE
		printf("subreddit_id not found");
		#endif
	}else{
		ptr->subreddit_id = strdup(subreddit_id->valuestring);
	}
	thumbnail = cJSON_GetObjectItemCaseSensitive(data, "thumbnail");
	if(!cJSON_IsString(thumbnail) || thumbnail->valuestring == NULL){
		#ifdef CRAW_DEBUG_MODE
		printf("thumbnail not found");
		#endif
	}else{
		ptr->thumbnail = strdup(thumbnail->valuestring);
	}
	title = cJSON_GetObjectItemCaseSensitive(data, "title");
	if(!cJSON_IsString(title) || title->valuestring == NULL){
		#ifdef CRAW_DEBUG_MODE
		printf("title not found");
		#endif
	}else{
		ptr->title = strdup(title->valuestring);
	}
	url = cJSON_GetObjectItemCaseSensitive(data, "url");
	if(!cJSON_IsString(url) || url->valuestring == NULL){
		#ifdef CRAW_DEBUG_MODE
		printf("url not found");
		#endif
	}else{
		ptr->url = strdup(url->valuestring);
	}
	edited = cJSON_GetObjectItemCaseSensitive(data, "edited");
	if(!cJSON_IsNumber(edited)){
		#ifdef CRAW_DEBUG_MODE
		printf("edited not found");
		#endif
	}else{
		ptr->edited = edited->valueint;
	}
	// this is a fucking mess, i know......
	is_stickied = cJSON_GetObjectItemCaseSensitive(data, "stickied");
	if(!cJSON_IsBool(is_stickied)){
		#ifdef CRAW_DEBUG_MODE
		printf("is_stickied not found");
		#endif
	}else{
		ptr->is_stickied = is_stickied->valueint;
	}
}

void CRAW_load_listing(cJSON *data, CRAW_Listing *listing){
	// same shit, initialize and get fucked
	const cJSON *after = NULL;
	const cJSON *dist = NULL;
	const cJSON *children = NULL;
	const cJSON *child = NULL;

	after = cJSON_GetObjectItemCaseSensitive(data, "after");
	if(!cJSON_IsString(after) || after->valuestring == NULL){
		#ifdef CRAW_DEBUG_MODE
		printf("after not found");
		#endif
	}else{
		listing->after = strdup(after->valuestring);
	}
	dist = cJSON_GetObjectItemCaseSensitive(data, "dist");
	if(!cJSON_IsNumber(dist)){
		#ifdef CRAW_DEBUG_MODE
		printf("dist not found");
		#endif
	}else{
		listing->dist = dist->valueint;
	}
	children = cJSON_GetObjectItemCaseSensitive(data, "children");
	if(!cJSON_IsArray(children)){
		#ifdef CRAW_DEBUG_MODE
		printf("children not found");
		#endif
		return;
	}
	// ooooooooh, json arrays in  C, interesing isnt it?
	int array_size = cJSON_GetArraySize(children);
	listing->array_size = array_size;
	// allocating memory for all the CRAW_children present
	listing->children = malloc(sizeof(CRAW_children) * array_size);
	int i = 0;
	// iterating through the json array
	cJSON_ArrayForEach(child, children){
		const cJSON *prefix = cJSON_GetObjectItemCaseSensitive(child, "kind");
		// extracting the prefix and comparing it with the type of data we have to process it
		/*
			TO DO:-
				- implement all the data types
		*/
		if(strcmp(prefix->valuestring ,"t1") == 0){
			
		}else if(strcmp(prefix->valuestring, "t2") == 0){
			const cJSON *obj = NULL;
			listing->children[i].type = CRAW_ACCOUNT;
			// allocating memory
			listing->children[i].data = calloc(1, sizeof(CRAW_Account));
			obj = cJSON_GetObjectItemCaseSensitive(child, "data");
			if(!cJSON_IsObject(obj)){
				#ifdef CRAW_DEBUG_MODE
				printf("data not found");
				#endif
			}
			// loading it into the respective function
			CRAW_load_account(obj, listing->children[i].data);
		}else if(strcmp(prefix->valuestring, "t3") == 0){
			const cJSON *obj = NULL;
			listing->children[i].type = CRAW_LINK;
			listing->children[i].data = calloc(1, sizeof(CRAW_Link));
			obj = cJSON_GetObjectItemCaseSensitive(child, "data");
			if(!cJSON_IsObject(obj)){
				#ifdef CRAW_DEBUG_MODE
				printf("data not found");
				#endif
			}
			CRAW_load_link(obj, listing->children[i].data);
		}else if(strcmp(prefix->valuestring, "t4") == 0){

		}else if(strcmp(prefix->valuestring, "t5") == 0){
			const cJSON *obj = NULL;
			listing->children[i].type = CRAW_SUBREDDIT;
			listing->children[i].data = calloc(1, sizeof(CRAW_Subreddit));
			obj = cJSON_GetObjectItemCaseSensitive(child, "data");
			if(!cJSON_IsObject(obj)){
				#ifdef CRAW_DEBUG_MODE
				printf("data not found");
				#endif
			}
			CRAW_load_subreddit(obj, listing->children[i].data);
		}else if(strcmp(prefix->valuestring, "t6") == 0){
		}
		i++;
	}
}

/*
please do not kill me for this shitty code im just 16 UwU 
*/
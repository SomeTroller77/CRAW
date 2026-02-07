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
    // initializing the cJSON ptrs to extract values from the json
    const cJSON *data = NULL;
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
    // parsing the received json
    cJSON *root = cJSON_Parse(json);
    // ptr safety
    if(root == NULL){
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL){
            fprintf(stderr, "Error before: %s\n", error_ptr);
            fflush(stderr);
        }
    }
    // grabbing the main "data" object
    data = cJSON_GetObjectItemCaseSensitive(root, "data");
    if(data == NULL){
        printf("Data is empty");
        return CRAW_PARSE_ERROR;
    }
    // grabbing the data, verifying its data type and storing it in the given CRAW_Subreddit pointer safely
    comment_score_hide_mins = cJSON_GetObjectItemCaseSensitive(data, "comment_score_hide_mins");
    if(!cJSON_IsNumber(comment_score_hide_mins)){
        printf("comment_score_hide_mins not found");
    }else{
        subreddit->comment_score_hide_mins = comment_score_hide_mins->valueint;
    }
    description = cJSON_GetObjectItemCaseSensitive(data, "description");
    if(!cJSON_IsString(description) || description->valuestring == NULL){
        printf("description not found");
    }else{
        subreddit->description = strdup(description->valuestring);
    }
    display_name = cJSON_GetObjectItemCaseSensitive(data, "display_name");
    if(!cJSON_IsString(display_name) || display_name->valuestring == NULL){
        printf("display_name not found");
    }else{
        subreddit->display_name = strdup(display_name->valuestring);
    }
    header_img = cJSON_GetObjectItemCaseSensitive(data, "header_img");
    if(!cJSON_IsString(header_img) || header_img->valuestring == NULL){
        printf("header_image not found");
    }else{
        subreddit->header_img = strdup(header_img->valuestring);
    }
    header_title = cJSON_GetObjectItemCaseSensitive(data, "header_title");
    if(!cJSON_IsString(header_title) || header_title->valuestring == NULL){
        printf("header_title not found");
    }else{
        subreddit->header_title = strdup(header_title->valuestring);
    }
    over18 = cJSON_GetObjectItemCaseSensitive(data, "over18");
    if(!cJSON_IsBool(over18)){
        printf("over18 not found");
    }else{
        subreddit->over18 = over18->valueint;
    }
    public_description = cJSON_GetObjectItemCaseSensitive(data, "public_description");
    if(!cJSON_IsString(public_description) || public_description->valuestring == NULL){
        printf("public_description not found");
    }else{
        subreddit->public_description = strdup(public_description->valuestring);
    }
    public_traffic = cJSON_GetObjectItemCaseSensitive(data, "public_traffic");
    if(!cJSON_IsBool(public_traffic)){
        printf("public_traffic not found");
    }else{
        subreddit->public_traffic = public_traffic->valueint;
    }
    subscribers = cJSON_GetObjectItemCaseSensitive(data, "subscribers");
    if(!cJSON_IsNumber(subscribers)){
        printf("subscribers not found");
    }else{
        subreddit->subscribers = subscribers->valueint;
    }
    submission_type = cJSON_GetObjectItemCaseSensitive(data, "submission_type");
    if(!cJSON_IsString(submission_type) || submission_type->valuestring == NULL){
        printf("submission_type not found");
    }else{
        subreddit->submission_type = strdup(submission_type->valuestring);
    }
    submit_link_label = cJSON_GetObjectItemCaseSensitive(data, "submit_link_label");
    if(!cJSON_IsString(submit_link_label) ||submit_link_label->valuestring == NULL){
        printf("submit_link_label not found");
    }else{
        subreddit->submit_link_label = strdup(submit_link_label->valuestring);
    }
    submit_text_label = cJSON_GetObjectItemCaseSensitive(data, "submit_text_label");
    if(!cJSON_IsString(submit_text_label) || submit_text_label->valuestring == NULL){
        printf("submit_text_label not found");
    }else{
        subreddit->submit_text_label = strdup(submit_text_label->valuestring);
    }
    title = cJSON_GetObjectItemCaseSensitive(data, "title");
    if(!cJSON_IsString(title) || title->valuestring == NULL){
        printf("title not found");
    }else{
        subreddit->title = strdup(title->valuestring);
    }
    url = cJSON_GetObjectItemCaseSensitive(data, "url");
    if(!cJSON_IsString(url) || url->valuestring == NULL){
        printf("url not found");
    }else{
        subreddit->url = strdup(url->valuestring);
    }
    is_user_banned = cJSON_GetObjectItemCaseSensitive(data, "user_is_banned");
    if(!cJSON_IsBool(is_user_banned)){
        printf("is_user_banned not found");
    }else{
        subreddit->is_user_banned = is_user_banned->valueint;
    }
    is_user_contributor = cJSON_GetObjectItemCaseSensitive(data, "user_is_contributor");
    if(!cJSON_IsBool(is_user_contributor)){
        printf("is_user_contributor not found");
    }else{
        subreddit->is_user_contributor = is_user_contributor->valueint;
    }
    is_user_moderator = cJSON_GetObjectItemCaseSensitive(data, "user_is_moderator");
    if(!cJSON_IsBool(is_user_moderator)){
        printf("is_user_moderator not found");
    }else{
        subreddit->is_user_moderator = is_user_moderator->valueint;
    }
    is_user_subscriber = cJSON_GetObjectItemCaseSensitive(data, "user_is_subscriber");
    if(!cJSON_IsBool(is_user_subscriber)){
        printf("is_user_subscriber not found");
    }else{
        subreddit->is_user_subscriber = is_user_subscriber->valueint;
    }
    created_utc = cJSON_GetObjectItemCaseSensitive(data, "created_utc");
    if(!cJSON_IsNumber(created_utc)){
        printf("created_utc not found");
    }else{
        subreddit->created_utc = created_utc->valueint;
    }
    subreddit_type = cJSON_GetObjectItemCaseSensitive(data, "subreddit_type");
    if(!cJSON_IsString(subreddit_type) || subreddit_type->valuestring == NULL){
        printf("subreddit_type not found");
    }
    // comparing the value of the subreddit_type field and storing it in the form of enum CRAW_SUBREDDIT_TYPE (see CRAW.h for more info)
    if(strcmp(subreddit_type->valuestring, "public")){
        subreddit->subreddit_type = CRAW_SUBREDDIT_PUBLIC;
    }else if(strcmp(subreddit_type->valuestring, "private")){
        subreddit->subreddit_type = CRAW_SUBREDDIT_PRIVATE;
    }else if(strcmp(subreddit_type->valuestring, "restricted")){
        subreddit->subreddit_type = CRAW_SUBREDDIT_RESTRICTED;
    }else if(strcmp(subreddit_type->valuestring, "gold_restricted")){
        subreddit->subreddit_type = CRAW_SUBREDDIT_GOLD_RESTRICTED;
    }else if(strcmp(subreddit_type->valuestring, "archived")){
        subreddit->subreddit_type = CRAW_SUBREDDIT_ARCHIVED;
    }
    // freeing the memory
    cJSON_Delete(root);
    free(json);
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
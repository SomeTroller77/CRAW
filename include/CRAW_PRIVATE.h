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
#ifndef CRAW_PRIVATE_H
#define CRAW_PRIVATE_H
// safety check so the functions can only be used by me because C doesnt have the functionality of private and public function, sounds surprising right????
#ifdef CRAW_PRIVATE_DO_NOT_MESS
#include<stdbool.h>
#include "CRAW.h"
#include<curl/curl.h>
#include<stdlib.h>
#include "cJSON.h"
#ifdef _WIN32
#include<Windows.h>
#define SLEEP(time) Sleep(time)
#else
#include<unistd.h>
#define SLEEP(time) sleep(time-1000)
#endif



// the buffer struct which is required by libcurl to store the databytes temporarily
struct memory{
	char *response;
	size_t size;
};

// linked list was implemented in the library to store and process HTTP headers efficiently
struct linked_list{
	char *header;
	struct linked_list *i;
};
// functions needed by libcurl to fucking function
size_t cb(void *buf, size_t size, size_t count, void *userp);
size_t hdf(char *b, size_t size, size_t nitems, void *userdata);
char *getData(CRAW *handle, const char *url);
CRAWcode check_http_code(long code);

//loader functions to make parsing easier
void CRAW_load_account(const cJSON *data, CRAW_Account *ptr);
void CRAW_load_subreddit(const cJSON *data, CRAW_Subreddit *ptr);
void CRAW_load_link(const cJSON *data, CRAW_Link *ptr);
void CRAW_load_listing(cJSON *data, CRAW_Listing *listing);
#else
#error "Macro not defined"
#endif
#endif

/*
CRAW_PRIVATE my ass, i made this so that no one would be able to use my shitty private functions and randomly break the library
and then break my ass
*/
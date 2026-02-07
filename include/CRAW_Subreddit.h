/*
Copyright 2022 SomeTroller77 / Saksham Vitwekar and the contributers of the CRAW project

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
DO NOT MESS WITH ANYTHING AS IT WILL BREAK THE LIBRARY OR CAUSE IT
TO NOT WORK PROPERLY

YOU HAVE BEEN WARNED
*/

#ifndef CRAW_SUBREDDIT_H
#define CRAW_SUBREDDIT_H
#include "CRAW_Main.h"
#include <stdbool.h>
// implementing CRAW_Reddit_Subreddit for subreddit information
typedef struct CRAW_Reddit_Subreddit{
	int comment_score_hide_mins;
	char *description;
	char *display_name;
	char *header_img;
	char *header_title;
	bool over18;
	char *public_description;
	bool public_traffic;
	long subscribers;
	char *submission_type;
	char *submit_link_label;
	char *submit_text_label;
	long created_utc;
	CRAW_Subreddit_type subreddit_type;
	char *title;
	char *url;
	bool is_user_banned;
	bool is_user_contributor;
	bool is_user_moderator;
	bool is_user_subscriber;
} CRAW_Subreddit;

// the init function to initialize a CRAW_Subreddit
CRAW_Subreddit *CRAW_Subreddit_Init();
// to get info about a subreddit
CRAWcode CRAW_Subreddit_getInfo(CRAW *handle, CRAW_Subreddit *subreddit, char *subreddit_name);
// function to free the CRAW_Subreddit pointer efficiently
void CRAW_Subreddit_Free(CRAW_Subreddit *ptr);
#endif

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

#ifndef CRAW_MAIN_H
#define CRAW_MAIN_H
#include <curl/curl.h>
// The base struct, the daddy of all the functions, ohhhh yeahhhh
typedef struct CRAW_Reddit_Bot{
	const char *client_id;
	const char *secret_key;
	const char *username;
	const char *password;
	const char *user_agent;
	// the struct which stores info about the last request aswell as tracks the info of ratelimiting
	struct internalInfo{
		const char *token_header;
		long error_code;
		int ratelimit_remaining;
		int ratelimit_reset;
		int ratelimit_used;
		CURLcode last_request_status;
	} *internal;
} CRAW;

// CRAW_error_codes for checking the result of a task and for debugging purposes
typedef enum {
	CRAW_OK,
	CRAW_PARSE_ERROR,
	CRAW_TOKEN_ERROR,
	CRAW_GRAB_ERROR,
	CRAW_BAD_REQUEST,
	CRAW_NOT_FOUND,
	CRAW_UNAUTHORISED,
	CRAW_FORBIDDEN,
	CRAW_TOO_MANY_REQUESTS,
	CRAW_UNKNOWN_CODE
} CRAWcode;

// enum to interpret the type of a subreddit
typedef enum {
	CRAW_SUBREDDIT_PUBLIC,
	CRAW_SUBREDDIT_PRIVATE,
	CRAW_SUBREDDIT_RESTRICTED,
	CRAW_SUBREDDIT_GOLD_RESTRICTED,
	CRAW_SUBREDDIT_ARCHIVED
} CRAW_Subreddit_type;

typedef enum {
	CRAW_COMMENT,
	CRAW_ACCOUNT,
	CRAW_LINK,
	CRAW_MESSAGE,
	CRAW_SUBREDDIT,
	CRAW_AWARD,
	CRAW_UNKNOWN_DATATYPE
} CRAW_Datatype;
typedef enum{
	CRAW_UPVOTED,
	CRAW_DOWNVOTED,
	CRAW_NO_VOTE
} CRAW_Vote;
// the init function for the handle of the bot, must be run to be able to use other functions
CRAW *CRAW_Init(const char *client_id, const char *secret_key, const char *username, const char *password, const char *user_agent);

// the function to safely free the handle of a bot
void CRAW_Free(CRAW *handle);

#endif

#ifndef CRAW_H__
#define CRAW_H__

#include<stdio.h>
#include "CRAW_Account.h"

struct memory{
	char *response;
	size_t size;
};

struct internalInfo;
typedef struct CRAW_Reddit_Bot_Info{
	const char *client_id;
	const char *secret_key;
	const char *username;
	const char *password;
	const char *user_agent;
	struct internalInfo *internal;
} CRAW;

struct internalInfo{
	const char *access_token;
};

typedef enum CRAW_error_codes{
	CRAW_OK,
	CRAW_PARSE_ERROR,
	CRAW_TOKEN_ERROR,
	CRAW_GRAB_ERROR
} CRAWcode;

CRAW *CRAW_Init(const char *client_id, const char *secret_key, const char *username, const char *password, const char *user_agent);
CRAWcode CRAW_free(CRAW *handle);
static char *replaceWord(const char* s, const char* oldW, const char* newW);
#endif

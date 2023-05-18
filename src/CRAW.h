#ifndef CRAW_H__
#define CRAW_H__
#define CRAW_OK 0
#define CRAW_INIT_ERROR 1
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
CRAW *CRAW_Init(const char *client_id, const char *secret_key, const char *username, const char *password, const char *user_agent);
int CRAW_free(CRAW *handle);
static char *replaceWord(const char* s, const char* oldW, const char* newW);
#endif

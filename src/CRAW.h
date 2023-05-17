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

typedef struct CRAW_Reddit_Bot_Info{
	char *client_id;
	char *secret_key;
	char *username;
	char *password;
	char *user_agent;
	char *access_token;
} CRAW;


CRAW *CRAW_Init(const char *client_id, const char *secret_key, const char *username, const char *password, const char *user_agent);
static char *replaceWord(const char* s, const char* oldW, const char* newW);
#endif

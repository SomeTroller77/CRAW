#ifndef CRAW_H__
#define CRAW_H__

#include<stdio.h>
#include "CRAW_Account.h"

struct memory{
	char *response;
	size_t size;
};
typedef enum CRAW_error_codes CRAWcode;
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
	int error_code;
};

typedef enum CRAW_error_codes{
	CRAW_OK,
	CRAW_PARSE_ERROR,
	CRAW_TOKEN_ERROR,
	CRAW_GRAB_ERROR,
	CRAW_BAD_REQUEST,
	CRAW_NOT_FOUND,
	CRAW_UNAUTHORISED
} CRAWcode;

CRAW *CRAW_Init(const char *client_id, const char *secret_key, const char *username, const char *password, const char *user_agent);
CRAWcode CRAW_free(CRAW *handle);

#endif

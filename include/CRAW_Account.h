#ifndef CRAW_ACCOUNT_ME
#define CRAW_ACCOUNT_ME
#include "CRAW.h"

typedef struct CRAW_Reddit_Bot_Info CRAW;

typedef enum CRAW_error_codes CRAWcode;

typedef struct CRAW_Account_Info{
	char *id;
	char *name;
	long created_utc;
	int total_karma;
} CRAW_Account;

CRAWcode CRAW_Account_me(CRAW *handle, CRAW_Account *accHandle);

CRAW_Account *CRAW_Account_Init();
#endif


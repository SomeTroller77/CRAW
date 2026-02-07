# CRAW (C Reddit API Wrapper)
![GitHub repo size](https://img.shields.io/github/repo-size/SomeTroller77/CRAW)

CRAW is a Reddit API Wrapper that is being developed in C to scrape data from reddit and make bots in C.

It is still under development and you can only use it to get your own data for now but i will be working on this project to develop it as soon and possible.
You can build it and see if it works or not.
## Table of contents
- [Where to get client id and secret key](#where-to-get-client-id-and-secret-key)
- [How to build](#How-to-build)
- [Data structures](#Data-Structures)
	- [CRAW_Account](#CRAW_Account)
	- [CRAW_Subreddit](#craw_subreddit)
- [Functions](#Functions)
	- [Main functions](#craw_main)
	- [Account](#craw_account-1)
	- [Subreddit](#craw_subreddit-1)
- [CRAWcode list](#CRAWcode-list)
- [Basic example](#Basic-example)


## where to get client id and secret key

If you want to use this Wrapper, you need a reddit client id and secret key which you can get by going [here](https://reddit.com/prefs/apps) and logging in with your credentials

## How to build?
The project has a dependency of libcurl, it can be installed in linux by

Ubuntu:
```
# sudo apt-get install curl
# apt-get install libcurl4-openssl-dev
```
Clone this repo using
```
git clone https://github.com/SomeTroller77/CRAW
```

then go into the CRAW Directory and do
(# means to run the command as root)
```
$ mkdir build && cd build
$ cmake ..
$ make
# make install
```

And you have installed CRAW, lets see hoe you can use it

## Data Structures

CRAW uses struct to store the data. heres the table of the structs in C

| Keyword | Description |
| --- | --- |
| CRAW | It is used to store the developers data that developers provide which will be used to send requests |
| CRAW_Account | It stores the account data which is sent by the reddit API |
| CRAW_Subreddit | It stores the data of subreddit which is sent by the reddit API |
| CRAW_Messages | It stores the data of a message/comment which is sent by the reddit API |

### CRAW_Account

here are some stuff which is stored and usable in CRAW_Account struct pointer

| Data Type | Field | Description |
| --------- | ---- | ----------- |
| char * | id | its stores the reddit id of the account |
| char * | name | it stores the username of the account |
| long | created_utc | it stores the time of creation of account in EPOCH time |
| int | total karma | it stores the total karma of the account |

### CRAW_Subreddit
| Data Type | Field | Description |
| --------- | ----- | ----------- |
| char * | description | sidebar text |
| char * | display_name | human name of the subreddit |
| char * | header_img | full URL to the header image |
| char * | header_title | description of header |
| bool | over18 | whether the subreddit is marked as NSFW |
| char * | public_description | Description shown in subreddit search results |
| bool | public_traffic | whether the subreddit's traffic page is publically accesssible |
| long | subscribers | the number of redditors subscribed to this subreddit |
| long | created_utc | UNIX Timestamp at which the subreddit was created |
| CRAW_Subreddit_type | subreddit_type | the type of the subreddit (public, private, restricted) |
| char * | title | title of the main page |
| char * | url | the relative URL of the subreddit |
| bool | is_user_banned | whether the logged-in user is banned from the subreddit
| bool | is_user_contributor | whether the logged-in user is an approved submitter of the reddit |
| bool | is_user_moderator | whether the logged-in user is a moderator of the subreddit |
| bool | is_user_subscriber | whether the logged-in user is subscribed to the subreddit |


## Functions

Functions implemented:-

### CRAW_Main
| Return type | Function declaration | Description |
| ----------- | -------------------- | ----------- |
| `CRAW *`    | `CRAW_Init("Client id", "secret_key", "username", "password", "user_agent")` | Returns a pointer to the struct CRAW, returns NULL if the servers are down or any arguments are wrong |
| `void` | `CRAW_free(CRAW *handle` | frees the pointer initialised using `CRAW_Init()` |

### CRAW_Account 
| Return type | Function declaration | Description |
| ----------- | -------------------- | ----------- |
| `CRAW_Account *` | `CRAW_Account_Init()` | Returns a pointer to a CRAW_Account struct which can be used to store an account data, returns NULL if your out of memory |
| `CRAWcode` | `CRAW_Account_me(CRAW *handle, CRAW_Account * accHandle)` | Returns a CRAW code and writes the data of logged in account to passed accHandle |
| `CRAWcode` | `CRAW_Account_getAbout(CRAW *handle, char *username, CRAW_Account *accHandle)` | Returns a CRAWcode and writes the provided data from api to the passed accHandle |
| `void` | `CRAW_Account_Free(CRAW_Account *accHandle)` | frees the passed pointer (Must be run on all the CRAW_Account variables initialised using `CRAW_Account_Init()` ) |

### CRAW_Subreddit
| Return type | Function declaration | Description |
| ----------- | -------------------- | ----------- |
| `CRAW_Subreddit *` | `CRAW_Subreddit_Init` | returns the pointer of a CRAW_Subreddit struct |
| `CRAWcode` | `CRAW_Subreddit_GetInfo(CRAW *handle, CRAW_Subreddit *subreddit, char *subreddit_name)` | inputs the information of a subreddit into the pointer subreddit |
| `void` | `CRAW_Subreddit_Free(CRAW_Subreddit *ptr)` | frees the pointer ptr safely |


## CRAWcode list
Heres a list of CRAWcode which can be return by the functions with return type `CRAWcode`

| CRAWcode | Description |
| ------------------ | -------------------------------- |
| CRAW_OK  | If you get this CRAWcode, then your all good to go |
| CRAW_PARSE_ERROR | If you get this CRAWcode, then there a parsing error, recheck your parameters and try again |
| CRAW_GRAB_ERROR | If you get this error, then check your internet as the wrapper couldnt grab the data from API |
| CRAW_NOT_FOUND | If you get this error then check your parameter as the API couldnt find the data your looking for |
| CRAW_UNAUTHORISED | If you get this error then check your username and password as the API was not able to authorize you ( there might be a chance of you being banned by api if you receive this error) |
| CRAW_FORBIDDEN | If you get this error then check your username and password and then try again |
| CRAW_TOO_MANY_REQUESTS | If you get this error, then take a chill pill and try it after 1-5 mins as you have been sending too many requests |
| CRAW_UNKNOWN_CODE | If you get this error, then create a issue and send us your code and we will fix it as theres no CRAWcode registered till now for this |

If you still get any on the CRAWcode after following the instructions, then you are welcome to create a issue and we will help you! 
## Basic example

Heres a basic example on how you could use it right now

```
#include<stdio.h>
#include<CRAW/CRAW_Main.h>

int main(){
	CRAW *handle=CRAW_Init("Your client id", "Your secret key", "Your reddit username", "Your reddit password", "Your user agent");
	if(handle == NULL){
		return 1;
	}
	CRAWcode res;
	CRAW_Account *myInfo=CRAW_Account_Init();
	res=CRAW_Account_me(handle, myInfo);
	if(res != CRAW_OK){
		return 2;
	}
	printf("My Name: %s", myInfo->name);
	CRAW_Account_free(myInfo);
	CRAW_free(handle);
	return 0;
}
```

## Note

This project is not well made as i am still developing it, it might take some time but i will try to make it a good project, issues are welcome



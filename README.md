# CRAW (C Reddit API Wrapper)
<p align="center">
  <img src="assets/logo.png" width="500">
</p>
<p align="center">
  <img src="https://img.shields.io/github/license/SomeTroller77/CRAW" />
  <img src="https://img.shields.io/github/issues/SomeTroller77/CRAW" />
  <img src="https://img.shields.io/github/issues-pr/SomeTroller77/CRAW" />
  <img src="https://img.shields.io/github/last-commit/SomeTroller77/CRAW" />
</p>

<p align="center">
  <img src="https://img.shields.io/github/repo-size/SomeTroller77/CRAW" />
  <img src="https://img.shields.io/github/languages/top/SomeTroller77/CRAW" />
  <img src="https://img.shields.io/github/commit-activity/m/SomeTroller77/CRAW" />
</p>

<p align="center">
  <img src="https://img.shields.io/github/stars/SomeTroller77/CRAW?style=social" />
  <img src="https://img.shields.io/github/forks/SomeTroller77/CRAW?style=social" />
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Reddit-API-orange?logo=reddit" />
  <img src="https://img.shields.io/badge/Language-C-blue?logo=c" />
  <img src="https://img.shields.io/badge/Build-CMake-blue?logo=cmake" />
</p>

## 📌 About

**CRAW (C Reddit API Wrapper)** is a lightweight Reddit API wrapper written in **C**.

It is designed to help developers fetch Reddit data and build bots using pure C, while keeping the library fast, simple, and easy to integrate into any project.

⚠️ **This project is still under development**, and currently supports only limited features. More endpoints and functionality will be added soon.

## Table of contents
- [Where to get client id and secret key](#where-to-get-client-id-and-secret-key)
- [How to build](#How-to-build)
- [Data structures](#Data-Structures)
	- [CRAW_Account](#CRAW_Account)
	- [CRAW_Subreddit](#craw_subreddit)
	- [CRAW_Links](#craw_links)
- [Functions](#Functions)
	- [Main functions](#craw_main)
	- [Account](#craw_account-1)
	- [Subreddit](#craw_subreddit-1)
- [List of enums](#enums)
	-[CRAW_Subreddit_type](#craw_subreddit_type)
	-[CRAW_Vote](#craw_vote)
	- [CRAWcode](#crawcode)
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
| `CRAW` | It is used to store the developers data that developers provide which will be used to send requests |
| `CRAW_Account` | It stores the account data which is sent by the reddit API |
| `CRAW_Subreddit` | It stores the data of subreddit which is sent by the reddit API |
| `CRAW_Messages` | It stores the data of a message/comment which is sent by the reddit API |

### CRAW_Account

here are some stuff which is stored and usable in CRAW_Account struct pointer

| Data Type | Field | Description |
| --------- | ---- | ----------- |
| `char *` | `id` | its stores the reddit id of the account |
| `char *` | `name` | it stores the username of the account |
| `long` | `created_utc` | it stores the time of creation of account in EPOCH time |
| `int` | `total karma` | it stores the total karma of the account |

### CRAW_Subreddit
| Data Type | Field | Description |
| --------- | ----- | ----------- |
| `char *` | `description` | sidebar text |
| `char *` | `display_name` | human name of the subreddit |
| `char *` | `header_img` | full URL to the header image |
| `char *` | `header_title` | description of header |
| `bool` | `over18` | whether the subreddit is marked as NSFW |
| `char *` | `public_description` | Description shown in subreddit search results |
| `bool` | `public_traffic` | whether the subreddit's traffic page is publically accesssible |
| `long`| `subscribers` | the number of redditors subscribed to this subreddit |
| `long` | `created_utc` | UNIX Timestamp at which the subreddit was created |
| `CRAW_Subreddit_type` | subreddit_type | the type of the subreddit (public, private, restricted) |
| `char *` | `title` | title of the main page |
| `char *` | `url` | the relative URL of the subreddit |
| `bool` | `is_user_banned` | whether the logged-in user is banned from the subreddit
| `bool` | `is_user_contributor` | whether the logged-in user is an approved submitter of the reddit |
| `bool` | `is_user_moderator` | whether the logged-in user is a moderator of the subreddit |
| `bool` | `is_user_subscriber` | whether the logged-in user is subscribed to the subreddit |

### CRAW_Links
| Data Type | Field | Description |
| --------- | ----- | ----------- |
| `char *` | `author` | the name of the author |
| `char *` | `author_fullname` | the id of the author |
| `char *` | `domain` | the domain from which the link is originated from |
| `bool` | `hidden` | whether the post is hidden from the user or not |
| `bool` | `is_self` | whether the post is made by the logged-in user |
| `CRAW_Vote` | `vote_status` | whether the logged-in user has voted on the link |
| `bool` | `is_locked` | whether the post is locked |
| `int` | `num_comments` | number of comments the link has |
| `bool` | `over_18` | whether the post is NSFW or not |
| `char *` | `permalink` | the permalink of the post |
| `bool` | `is_saved` | whether the post is saved by the logged-in user |
| `int` | `score` | the difference between the upvotes and the downvotes of the link |
| `char *` | `selftext` | the content of the post |
| `char *` | `subreddit` | the name of the subreddit without /r/ |
| `char *` | `subreddit_id` | the id of the subreddit |
| `char *` | `title` | the title of the post |
| `char *` | `url`| the url of the post |
| `long` | `edited` | the UNIX Timestamp when the post was edited, null when not edited |
| `bool` | `is_stickied` | whether the post has been stickied in a subreddit |

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
| ` CRAWcode` | `CRAW_Subreddit_getHotPosts(CRAW *handle, CRAW_Listing *list, char *subreddit_name) | get the hot posts in a subreddit (default 25, option to customize will be implemented soon) |
| ` CRAWcode` | `CRAW_Subreddit_getNewPosts(CRAW *handle, CRAW_Listing *list, char *subreddit_name) | get the new posts in a subreddit (default 25, option to customize will be implemented soon) |
| ` CRAWcode` | `CRAW_Subreddit_getRisingPosts(CRAW *handle, CRAW_Listing *list, char *subreddit_name) | get the rising posts in a subreddit (default 25, option to customize will be implemented soon) |
| `void` | `CRAW_Subreddit_Free(CRAW_Subreddit *ptr)` | frees the pointer ptr safely |


## Enums 
Enums make it easy for developers to be able to use conditions without needing to remember the int values of different entities

### CRAW_Datatype
The datatypes which are used by CRAW to manage listing as it utilizes void pointers which are needed to be casted to be able to compile it

Heres a list of datatypes which CRAW uses
| CRAW_Datatype | Description |
| ------------------ | -------------------------------- |
| `CRAW_COMMENT` | A reddit comment data |
| `CRAW_ACCOUNT` | A reddit account data |
| `CRAW_LINK` | A reddit link data (eg subreddit posts etc) |
| `CRAW_MESSAGE` | A reddit message data |
| `CRAW_SUBREDDIT` | A subreddit data | 
| `CRAW_AWARD` | A reddit award data |
| `CRAW_UNKNOWN_DATATYPE` | Datatype which has not been implemented into CRAW (yet) |

### CRAW_Subreddit_type
Determines the visibility status of a subreddit
| CRAW_Subreddit_type | Description |
| ------------------ | -------------------------------- |
| `CRAW_SUBREDDIT_PUBLIC` | A public subreddit |
| `CRAW_SUBREDDIT_PRIVATE` | A private subreddit |
| `CRAW_SUBREDDIT_RESTRICTED` | A restricted subreddit |

### CRAW_Vote
Tells you whether the logged-in user has upvoted, downvoted, or not voted to the post
| CRAW_Vote | Description |
| ------------------ | -------------------------------- |
| `CRAW_UPVOTED` | User has upvoted the post |
| `CRAW_DOWNVOTED` | User has downvoted the post |
| `CRAW_NO_VOTE` | User has neither upvoted nor downvoted the post |

### CRAWcode
Heres a list of CRAWcode which may be return by the functions with return type `CRAWcode`

| CRAWcode | Description |
| ------------------ | -------------------------------- |
| `CRAW_OK ` | If you get this CRAWcode, then your all good to go |
| `CRAW_PARSE_ERROR` | If you get this CRAWcode, then there a parsing error, recheck your parameters and try again |
| `CRAW_GRAB_ERROR` | If you get this error, then check your internet as the wrapper couldnt grab the data from API |
| `CRAW_NOT_FOUND` | If you get this error then check your parameter as the API couldnt find the data your looking for |
| `CRAW_UNAUTHORISED` | If you get this error then check your username and password as the API was not able to authorize you ( there might be a chance of you being banned by api if you receive this error) |
| `CRAW_FORBIDDEN` | If you get this error then check your username and password and then try again |
| `CRAW_TOO_MANY_REQUESTS` | If you get this error, then take a chill pill and try it after 1-5 mins as you have been sending too many requests |
| `CRAW_UNKNOWN_CODE` | If you get this error, then create a issue and send us your code and we will fix it as theres no CRAWcode registered till now for this |

If you still get any on the CRAWcode after following the instructions, then you are welcome to create a issue and we will help you! 
## Basic example

Heres a basic example on how you could use it right now

```
#include<stdio.h>
#include<CRAW/CRAW_Main.h>

int main(){
	// initializing the handler (logging into the oauth api)
	CRAW *handle=CRAW_Init("Your client id", "Your secret key", "Your reddit username", "Your reddit password", "Your user agent");
	if(handle == NULL){
		return 1;
	}
	CRAWcode res;
	CRAW_Account *myInfo=CRAW_Account_Init();
	res=CRAW_Account_me(handle, myInfo); // getting the info of the logged-in account
	if(res != CRAW_OK){
		return 2;
	}
	printf("My Name: %s", myInfo->name);
	CRAW_Listing *a = CRAW_Listing_Init();
	CRAW_Subreddit_getNewPosts(handle, a, "hacking");
	CRAW_Link *test = a->children[2].data; // MANDATORY TO CAST THE void * POINTER TO A VALID CRAW_Datatype
	printf("\n\n\n%s\nTotal posts:- %d", test->permalink, a->array_size);
	CRAW_Listing_Free(a);
	CRAW_Account_Free(myInfo);
	CRAW_Free(handle);
	return 0;
}
```

## Note

This project is not well made as i am still developing it, it might take some time but i will try to make it a good project, issues are welcome



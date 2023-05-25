# CRAW (C Reddit API Wrapper)
![GitHub repo size](https://img.shields.io/github/repo-size/SomeTroller77/CRAW)

CRAW is a Reddit API Wrapper that is being developed in C to scrape data from reddit and make bots in C.

It is still under development and you can only use it to get your own data for now but i will be working on this project to develop it as soon and possible.
You can build it and see if it works or not.
## Table of contents
- [Where to get client id and secret key](#where-to-get-client-id-and-secret-key)
- [How to build](#How-to-build)
- [Structure and typedef names](#Data-Structures)
	- [CRAW_Account](#CRAW_Account)
- [Functions](#Functions)
- [CRAWcode list](#CRAWcode-list)
- [Basic example](#Basic-example)


## where to get client id and secret key

If you want to use this Wrapper, you need a reddit client id and secret key which you can get by going [here](https://reddit.com/prefs/apps) and logging in with your credentials

## How to build?
The project has a dependency of libcurl, it can be installed in linux by

Ubuntu:
```
# sudo apt-get install curl
# sudo apt-get install libcurl-dev
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

#### CRAW_Account

here are some stuff which is stored and usable in CRAW_Account struct pointer

| Data type | Name | Description |
| --------- | ---- | ----------- |
| char * | id | its stores the reddit id of the account |
| char * | name | it stores the username of the account |
| long | created_utc | it stores the time of creation of account in EPOCH time |
| int | total karma | it stores the total karma of the account |

## Functions

Here are some functions which can be called to do something

| Return type | Function declaration | Description |
| ----------- | -------------------- | ----------- |
| `CRAW *`    | `CRAW_Init("Client id", "secret_key", "username", "password", "user_agent")` | Returns a pointer to the struct CRAW, returns NULL if the servers are down or any arguments are wrong |
| `CRAW_Account *` | `CRAW_Account_Init()` | Returns a pointer to a CRAW_Account struct which can be used to store an account data, returns NULL if your out of memory |
| `CRAWcode` | `CRAW_Account_me(CRAW *handle, CRAW_Account * accHandle)` | Returns a CRAW code and writes the data of logged in account to passed accHandle |
| `CRAWcode` | `CRAW_Account_getUserAbout(CRAW *handle, char *username, CRAW_Account *accHandle)` | Returns a CRAWcode and writes the provided data from api to the passed accHandle |
| `CRAWcode` | `CRAW_Account_free(CRAW_Account *accHandle)` | returns CRAWcode and frees the passed accHandle (Must be run on all the CRAW_Account variables initialised using `CRAW_Account_Init()` ) |
| `CRAWcode` | `CRAW_free(CRAW *handle` | Returns a CRAWcode and frees the variable initialised using `CRAW_Init()` ) |

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
| CRAW_UNKNOWN_CODE | If you get this error, then create a issue and send us your code and we will fix it as theres no CRAWcpde resgitered till now for this |

If you still get any on the CRAWcode after following the instructions, then you are welcome to create a issue and we will help you! 
## Basic example

Heres a basic example on how you could use it right now

```
#include<stdio.h>
#include<CRAW/CRAW.h>

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
	CRAW_Account_free(myInfo->name);
	CRAW_free(handle);
	return 0;
}
```

## Note

This project is not well made as i am still developing it, it might take some time but i will try to make it a good project, issues are welcome



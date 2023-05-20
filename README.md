## CRAW (C Reddit API Wrapper)
![GitHub repo size](https://img.shields.io/github/repo-size/SomeTroller77/CRAW)

CRAW is a Reddit API Wrapper that is being developed in C to scrape data from reddit and make bots in C.

It is still under development and you can only use it to get your own data for now but i will be working on this project to develop it as soon and possible.
You can build it and see if it works or not.

## How to build?

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

##Data Structures

CRAW uses struct to store the data. heres the table of the structs in C

| Keyword | Description |
| --- | --- |
| CRAW | It is used to store the developers data that developers provide which will be used to send requests |
| CRAW_Account | It stores the account data which is sent by the reddit API |

##CRAW_Account

here are some stuff which is stored and usable in CRAW_Account struct pointer

| Data type | Name | Description |
| --------- | ---- | ----------- |
| char * | id | its stores the reddit id of the account |
| char * | name | it stores the username of the account |
| long | created_utc | it stores the time of creation of account in EPOCH time |
| int | total karma | it stores the total karma of the account |


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
	CRAW_Free(handle);
	return 0;
}
```

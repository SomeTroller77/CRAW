/*
Copyright 2022-2026 SomeTroller77 / Saksham Vitwekar and the contributers of the CRAW project

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
DO NOT MESS WITH ANYTHING AS IT WILL BREAK THE LIBRARY OR CAUSE IT
TO NOT WORK PROPERLY

YOU HAVE BEEN WARNED
*/

#ifndef CRAW_LISTING_H
#define CRAW_LISTING_H
#include "./CRAW_Datatypes.h"
// struct to make dynamic datatype actually fucking work for reddit listings
typedef struct {
    CRAW_Datatype type;
    void *data;
} CRAW_children;

// the daddy struct for listing, ohh yeahhh
typedef struct{
    char *after;
    int array_size;
    int dist;
    CRAW_children *children;
} CRAW_Listing;

//init and free
CRAW_Listing *CRAW_Listing_Init();
void CRAW_Listing_Free(CRAW_Listing *ptr);
#endif
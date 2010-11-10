#pragma once

#ifndef NULL
#define NULL 0
#endif

#define CHECKED_DELETE(x)                     if(x) delete x; x=NULL;
#define CHECKED_RELEASE(x)                    if(x) x->Release(); x=NULL;
#define CHECKED_DELETE_ARRAY(x)               if(x) delete [] x; x=NULL;
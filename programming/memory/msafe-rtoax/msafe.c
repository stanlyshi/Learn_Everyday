#include <malloc.h>
#include <mcheck.h>

#include "msafe.h"




static const char *const msafe_status_str[] = {
    "Memory Safe Disable",
    "Memory OK",
    "Memory Double Free",
    "Memory was Clobbered Before The Block",
    "Memory was Clobbered After The Block",
    "Uknown Msafe Status",
};

const char *const msafe_strerror(enum msafe_status status){
    if(status > MSAFE_TAIL || status < MSAFE_DISABLED) status = MSAFE_TAIL+1;
    
    return msafe_status_str[status+1];
}

//TODO

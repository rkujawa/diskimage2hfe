/*
 * Something like compatibility layer...
 */

#include <stdlib.h>

#include "amiga_compat.h"

char *strdup(const char *s) {
    char *p = malloc(strlen(s) + 1);
    if(p) { strcpy(p, s); }
    return p;
}

short simpleswab16(short from) {
    return ( ((from<<8)&0xFF00) | ((from>>8)&0x00FF) );
}

#ifdef NEED_SWAB
void
swab(const void *from, void *to, ssize_t len)
{
        char temp;
        const char *fp;
        char *tp;

        if (len <= 1)
                return;

        len /= 2;
        fp = (const char *)from;
        tp = (char *)to;
#define STEP    temp = *fp++,*tp++ = *fp++,*tp++ = temp
        /* round to multiple of 8 */
        while ((--len % 8) != 0)
                STEP;
        len /= 8;
        if (len == 0)
                return;
        while (len-- != 0) {
                STEP; STEP; STEP; STEP;
                STEP; STEP; STEP; STEP;
        }
}
#endif

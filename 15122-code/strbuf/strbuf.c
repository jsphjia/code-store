#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "strbuf.h"
#include "lib/contracts.h"
#include "lib/xalloc.h"

/*** Implementation ***/

bool is_strbuf(strbuf *sb)
{
    // NULL check
    if(sb == NULL) return false;
    if(sb->buf == NULL) return false;

    if(sb->limit == 0) return false;
    if(sb->limit < sb->len + 1) return false;
    if(strlen(sb->buf) != sb->len) return false;

    // [0, len) should be non-NUL char
    // [0, len] should be a NUL-terminated string
    for(size_t i = 0; i < sb->len; i++) if(sb->buf[i] == '\0') return false;
    if(sb->buf[sb->len] != '\0') return false;

    return true;
}

// creates a new strbuf
strbuf* strbuf_new(size_t init_limit)
//@requires init_limit > 0;
//@ensures is_strbuf(\result);
{
    REQUIRES(init_limit > 0);
    // create strbuf
    strbuf *sb = xmalloc(sizeof(strbuf));
    char *buf = xmalloc(init_limit);
    sb->limit = init_limit;
    sb->len = 0;
    sb->buf = buf;
    sb->buf[0] = '\0';

    // return strbuf
    ENSURES(is_strbuf(sb));
    return sb;
}

// returns buf from sb
char* strbuf_str(strbuf* sb)
//@requires is_strbuf(sb);
//@ensures \length(\result) == sb->len + 1;
{
    REQUIRES(is_strbuf(sb));
    // create resulting array
    char *res = xmalloc((sb->len + 1));

    // iterate through array until NUL char
    for(size_t i = 0; i < sb->len; i++) res[i] = sb->buf[i];
    res[sb->len] = '\0';

    ENSURES(strlen(res) == sb->len);
    return res;
}

// adds str (with known length) to sb->buf 
void strbuf_add(strbuf* sb, char *str, size_t str_len)
//@requires is_strbuf(sb);
//@requires string_length(string_from_chararray(str)) == str_len;
//@requires sb->len + str_len <= int_max()/2;
{
    REQUIRES(is_strbuf(sb));
    REQUIRES(str != NULL);
    REQUIRES(str[str_len] == '\0');
    REQUIRES(strlen(str) == str_len);
    REQUIRES(sb->len + str_len + 1 <= INT_MAX/2);
    // determine if additional allocation is needed
    if(sb->len + str_len + 1 > sb->limit)
    {
        // create a new large enough array
        char *newBuf = xmalloc(2 * (sb->len + str_len));

        // add the char from sb->buf to newBuf
        for(size_t i = 0; i < sb->len; i++) newBuf[i] = sb->buf[i];
        for(size_t j = 0; j < str_len; j++) newBuf[sb->len + j] = str[j];

        // free old array
        free(sb->buf);

        sb->buf = newBuf;
        sb->limit = 2 * (sb->len + str_len);
    }
    // otherwise, add to original sb->buf
    else for(size_t i = 0; i < str_len; i++) sb->buf[sb->len + i] = str[i];

    sb->len += str_len;
    sb->buf[sb->len] = '\0';
}

// adds str (with unknown length) to sb->buf 
void strbuf_addstr(strbuf* sb, char *str)
//@requires is_strbuf(sb);
{
    REQUIRES(is_strbuf(sb));
    REQUIRES(str != NULL);
    strbuf_add(sb, str, strlen(str));
}

// free sb but not sb->buf
char *strbuf_free(strbuf *sb)
{
    REQUIRES(is_strbuf(sb));
    char *res = sb->buf;
    free(sb);
    return res;
}
/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2019, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
/* <DESC>
 * Multiplexed HTTP/2 downloads over a single connection
 * </DESC>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* somewhat unix-specific */
#include <sys/time.h>
#include <unistd.h>

/* curl stuff */
#include <curl/curl.h>


struct download_transfer {
  CURL *easy;
  FILE *out;
};

static void download_dump(const char *text, unsigned char *ptr, size_t size, char nohex)
{
    size_t i;
    size_t c;

    unsigned int width = 0x10;

    if(nohex)
        /* without the hex output, we can fit more on screen */
        width = 0x40;

    fprintf(stderr, "%s, %lu bytes (0x%lx)\n",
    text, (unsigned long)size, (unsigned long)size);

    for(i = 0; i<size; i += width) {

        fprintf(stderr, "%4.4lx: ", (unsigned long)i);

        if(!nohex) {
            /* hex not disabled, show it */
            for(c = 0; c < width; c++)
            if(i + c < size)
                fprintf(stderr, "%02x ", ptr[i + c]);
            else
                fputs("   ", stderr);
        }

        for(c = 0; (c < width) && (i + c < size); c++) {
            /* check for 0D0A; if found, skip past and start a new line of output */
            if(nohex && (i + c + 1 < size) && ptr[i + c] == 0x0D &&
            ptr[i + c + 1] == 0x0A) {
                i += (c + 2 - width);
                break;
            }
            fprintf(stderr, "%c", (ptr[i + c] >= 0x20) && (ptr[i + c]<0x80)?ptr[i + c]:'.');
            /* check again for 0D0A, to avoid an extra \n if it's at width */
            if(nohex && (i + c + 2 < size) && ptr[i + c + 1] == 0x0D &&
            ptr[i + c + 2] == 0x0A) {
                i += (c + 3 - width);
                break;
            }
        }
        fputc('\n', stderr); /* newline */
    }
}

static int download_debug_function(CURL *handle, curl_infotype type,
             char *data, size_t size,
             void *userp)
{
    const char *text;
    struct download_transfer *t = (struct download_transfer *)userp;

    (void)handle; /* prevent compiler warning */

    switch(type) {
        case CURLINFO_TEXT:
        fprintf(stderr, "== Info: %s", data);
        /* FALLTHROUGH */
        default: /* in case a new one is introduced to shock us */
        return 0;

        case CURLINFO_HEADER_OUT:
        text = "=> Send header";
        break;
        case CURLINFO_DATA_OUT:
        text = "=> Send data";
        break;
        case CURLINFO_SSL_DATA_OUT:
        text = "=> Send SSL data";
        break;
        case CURLINFO_HEADER_IN:
        text = "<= Recv header";
        break;
        case CURLINFO_DATA_IN:
        text = "<= Recv data";
        break;
        case CURLINFO_SSL_DATA_IN:
        text = "<= Recv SSL data";
        break;
    }

    download_dump(text, (unsigned char *)data, size, 1);
    return 0;
}

static void config_curl_download(struct download_transfer *t, const char *url, const char* dst_file)
{
    char filename[128];
    CURL *hnd;

    hnd = t->easy = curl_easy_init();

    snprintf(filename, 128, dst_file);

    t->out = fopen(filename, "wb");

    /* write to this file */
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, t->out);

    /* set the same URL */
    curl_easy_setopt(hnd, CURLOPT_URL, url);

    /* please be verbose */
    curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(hnd, CURLOPT_DEBUGFUNCTION, download_debug_function);
    curl_easy_setopt(hnd, CURLOPT_DEBUGDATA, t);

    /* HTTP/2 please */
    curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);

    /* we use a self-signed test server, skip verification during debugging */
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);

    /* This little trick will just make sure that we don't enable pipelining for
     libcurls old enough to not have this symbol. It is _not_ defined to zero in
     a recent libcurl header. */
#define __CURLPIPE_MULTIPLEX 0
#if (__CURLPIPE_MULTIPLEX > 0)
    /* wait for pipe connection to confirm */
    curl_easy_setopt(hnd, CURLOPT_PIPEWAIT, 1L);
#endif
}

/*
 * Download many transfers over HTTP/2, using the same connection!
 */
int launch_download(const char *url, const char *dst_file)
{
    struct download_transfer trans;
    CURLM *multi_handle;
    int i;
    int still_running = 0; /* keep number of running handles */

    /* init a multi stack */
    multi_handle = curl_multi_init();

    config_curl_download(&trans, url, dst_file);

    /* add the individual download_transfer */
    curl_multi_add_handle(multi_handle, trans.easy);

    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, __CURLPIPE_MULTIPLEX);

    /* we start some action by calling perform right away */
    curl_multi_perform(multi_handle, &still_running);

    while(still_running) {
        struct timeval timeout;
        int rc; /* select() return code */
        CURLMcode mc; /* curl_multi_fdset() return code */

        fd_set fdread;
        fd_set fdwrite;
        fd_set fdexcep;
        int maxfd = -1;

        long curl_timeo = -1;

        FD_ZERO(&fdread);
        FD_ZERO(&fdwrite);
        FD_ZERO(&fdexcep);

        /* set a suitable timeout to play around with */
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        curl_multi_timeout(multi_handle, &curl_timeo);
        if(curl_timeo >= 0) {
            timeout.tv_sec = curl_timeo / 1000;
            if(timeout.tv_sec > 1)
                timeout.tv_sec = 1;
            else
                timeout.tv_usec = (curl_timeo % 1000) * 1000;
        }

        /* get file descriptors from the transfers */
        mc = curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

        if(mc != CURLM_OK) {
            fprintf(stderr, "curl_multi_fdset() failed, code %d.\n", mc);
            break;
        }

        /* On success the value of maxfd is guaranteed to be >= -1. We call
        select(maxfd + 1, ...); specially in case of (maxfd == -1) there are
        no fds ready yet so we call select(0, ...) --or Sleep() on Windows--
        to sleep 100ms, which is the minimum suggested value in the
        curl_multi_fdset() doc. */

        if(maxfd == -1) {
            /* Portable sleep for platforms other than Windows. */
            struct timeval wait = { 0, 100 * 1000 }; /* 100ms */
            rc = select(0, NULL, NULL, NULL, &wait);
        } else {
            /* Note that on some platforms 'timeout' may be modified by select().
            If you need access to the original value save a copy beforehand. */
            rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
        }

        switch(rc) {
            case -1:
                /* select error */
                break;
            case 0:
            default:
                /* timeout or readable/writable sockets */
                curl_multi_perform(multi_handle, &still_running);
                break;
        }
    }

    curl_multi_remove_handle(multi_handle, trans.easy);
    curl_easy_cleanup(trans.easy);

    curl_multi_cleanup(multi_handle);

    return 0;
}



int main(int argc, char **argv)
{
    launch_download("http://10.170.6.66/study/a.out", "../rongtao.TXT");

    return 0;
}


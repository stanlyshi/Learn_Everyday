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
 * Multiplexed HTTP/2 uploads over a single connection
 * </DESC>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <libgen.h> //basename

/* somewhat unix-specific */
#include <sys/time.h>
#include <unistd.h>

/* curl stuff */
#include <curl/curl.h>

#define __CURLPIPE_MULTIPLEX 0


struct upload_input {
  FILE *in;
  size_t bytes_read; /* count up */
  CURL *hnd;
};

static void upload_dump(const char *text, unsigned char *ptr, size_t size, char nohex)
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
      fprintf(stderr, "%c",
              (ptr[i + c] >= 0x20) && (ptr[i + c]<0x80)?ptr[i + c]:'.');
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

static int upload_debug_function(CURL *handle, curl_infotype type,
             char *data, size_t size,
             void *userp)
{
  char timebuf[60];
  const char *text;
  struct upload_input *i = (struct upload_input *)userp;
  static time_t epoch_offset;
  static int    known_offset;
  struct timeval tv;
  time_t secs;
  struct tm *now;
  (void)handle; /* prevent compiler warning */

  gettimeofday(&tv, NULL);
  if(!known_offset) {
    epoch_offset = time(NULL) - tv.tv_sec;
    known_offset = 1;
  }
  secs = epoch_offset + tv.tv_sec;
  now = localtime(&secs);  /* not thread safe but we don't care */
  snprintf(timebuf, sizeof(timebuf), "%02d:%02d:%02d.%06ld",
           now->tm_hour, now->tm_min, now->tm_sec, (long)tv.tv_usec);

  switch(type) {
  case CURLINFO_TEXT:
    fprintf(stderr, "%s Info: %s", timebuf, data);
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

  upload_dump(text, (unsigned char *)data, size, 1);
  return 0;
}

static size_t upload_read_function(void *ptr, size_t size, size_t nmemb, void *userp)
{
  struct upload_input *i = userp;
  size_t retcode = fread(ptr, size, nmemb, i->in);
  i->bytes_read += retcode;
  return retcode;
}

static void config_curl_upload(struct upload_input *i, const char *upload, const char *URL)
{
    FILE *out;
    char url[256];
    char filename[128];
    struct stat file_info;
    curl_off_t uploadsize;
    CURL *hnd;

    hnd = i->hnd = curl_easy_init();
    snprintf(filename, 128, "upload-log.html");
    out = fopen(filename, "wb");

    snprintf(url, 256, URL);
//    snprintf(url, 256, "http://10.170.6.66:80/upload.cgi");

    
    /* get the file size of the local file */
    stat(upload, &file_info);
    uploadsize = file_info.st_size;

    i->in = fopen(upload, "rb");

    /* write to this file */
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, out);

    /* we want to use our own read function */
    curl_easy_setopt(hnd, CURLOPT_READFUNCTION, upload_read_function);
    /* read from this file */
    curl_easy_setopt(hnd, CURLOPT_READDATA, i);
    /* provide the size of the upload */
    curl_easy_setopt(hnd, CURLOPT_INFILESIZE_LARGE, uploadsize);

    /* send in the URL to store the upload as */
    curl_easy_setopt(hnd, CURLOPT_URL, url);

    /* upload please */
    curl_easy_setopt(hnd, CURLOPT_UPLOAD, 1L);

    /* please be verbose */
    curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(hnd, CURLOPT_DEBUGFUNCTION, upload_debug_function);
    curl_easy_setopt(hnd, CURLOPT_DEBUGDATA, i);

    /* HTTP/2 please */
    curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);

    /* we use a self-signed test server, skip verification during debugging */
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(hnd, CURLOPT_SSL_VERIFYHOST, 0L);

#if (__CURLPIPE_MULTIPLEX > 0)
    /* wait for pipe connection to confirm */
    curl_easy_setopt(hnd, CURLOPT_PIPEWAIT, 1L);
#endif
}

/*
 * Upload all files over HTTP/2, using the same physical connection!
 */
static int launch_upload(const char *filename, const char *url)
{
    struct upload_input trans;
    CURLM *multi_handle;
    int i;
    int still_running = 0; /* keep number of running handles */

    /* init a multi stack */
    multi_handle = curl_multi_init();

    config_curl_upload(&trans, filename, url);

    /* add the individual transfer */
    curl_multi_add_handle(multi_handle, trans.hnd);

    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, __CURLPIPE_MULTIPLEX);

    /* We do HTTP/2 so let's stick to one connection per host */
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, 1L);

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
        }
        else {
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

    curl_multi_cleanup(multi_handle);

    curl_multi_remove_handle(multi_handle, trans.hnd);
    curl_easy_cleanup(trans.hnd);

    return 0;
}



int main(int argc, char **argv)
{
    launch_upload("uploadthis.txt", "http://10.170.6.66:8443/");
    return 0;
}


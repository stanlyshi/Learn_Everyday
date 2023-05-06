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
 * Use CURLOPT_RESOLVE to feed custom IP addresses for given host name + port
 * number combinations. 使用CURLOPT_RESOLVE为给定的主机名+端口*数字组合提供自定义IP地址。
 * </DESC>
 */
#include <stdio.h>
#include <curl/curl.h>

int main(void)
{
  CURL *curl;
  CURLcode res = CURLE_OK;

  /* Each single name resolve string should be written using the format
     HOST:PORT:ADDRESS where HOST is the name libcurl will try to resolve,
     PORT is the port number of the service where libcurl wants to connect to
     the HOST and ADDRESS is the numerical IP address   增加HTTP Header
   */
  struct curl_slist *host = curl_slist_append(NULL,
                                              "localhost:80:127.0.0.1");

  curl = curl_easy_init();
  if(curl) {
//    Provide  a  custom  address  for  a specific host and port pair. Using this, 
//        you can make the curl requests(s) use a specified address and prevent the otherwise  normally  
//        resolved  address  to  be used.  Consider  it a sort of /etc/hosts alternative provided on the 
//        command line. The port number  should be the number used for the specific protocol the host 
//        will be used for. It means  you  need several entries if you want to provide address for the same host but different ports.
//    提供特定主机和端口对的自定义地址。 使用此功能，
//        可以使卷曲请求使用指定的地址，并防止使用通常可以正常解析的地址。 
//        考虑它是命令行上提供的/ etc / hosts替代方案。 端口号应为主机将用于特定协议的号码。 
//        这意味着如果要提供同一主机但不同端口的地址，则需要几个条目。
    curl_easy_setopt(curl, CURLOPT_RESOLVE, host);
    curl_easy_setopt(curl, CURLOPT_URL, "http://10.170.6.66");
    res = curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);
  }

  curl_slist_free_all(host);

  return (int)res;
}

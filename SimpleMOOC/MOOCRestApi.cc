/*
 * Copyright (C) 2014 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/


#include <curl/curl.h>
#include <cstring>
#include <stdlib.h>

#include <iostream>
#include "MOOCRestApi.hh"

using namespace gazebo;
using namespace std;



struct data {
  char trace_ascii; /* 1 or 0 */
};

static
void dump(const char *text,
          FILE *stream, unsigned char *ptr, size_t size,
          char nohex)
{
  size_t i;
  size_t c;

  unsigned int width=0x10;

  if(nohex)
    /* without the hex output, we can fit more on screen */
    width = 0x40;

  fprintf(stream, "%s, %10.10ld bytes (0x%8.8lx)\n",
          text, (long)size, (long)size);

  for(i=0; i<size; i+= width) {

    fprintf(stream, "%4.4lx: ", (long)i);

    if(!nohex) {
      /* hex not disabled, show it */
      for(c = 0; c < width; c++)
        if(i+c < size)
          fprintf(stream, "%02x ", ptr[i+c]);
        else
          fputs("   ", stream);
    }

    for(c = 0; (c < width) && (i+c < size); c++) {
      /* check for 0D0A; if found, skip past and start a new line of output */
      if (nohex && (i+c+1 < size) && ptr[i+c]==0x0D && ptr[i+c+1]==0x0A) {
        i+=(c+2-width);
        break;
      }
      fprintf(stream, "%c",
              (ptr[i+c]>=0x20) && (ptr[i+c]<0x80)?ptr[i+c]:'.');
      /* check again for 0D0A, to avoid an extra \n if it's at width */
      if (nohex && (i+c+2 < size) && ptr[i+c+1]==0x0D && ptr[i+c+2]==0x0A) {
        i+=(c+3-width);
        break;
      }
    }
    fputc('\n', stream); /* newline */
  }
  fflush(stream);
}

static
int my_trace(CURL *handle, curl_infotype type,
             char *data, size_t size,
             void *userp)
{
  struct data *config = (struct data *)userp;
  const char *text;
  (void)handle; /* prevent compiler warning */

  switch (type) {
  case CURLINFO_TEXT:
    fprintf(stderr, "== Info: %s", data);
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

  dump(text, stderr, (unsigned char *)data, size, config->trace_ascii);
  return 0;
}


// private data structure used to
// read libcurl response
struct MemoryStruct {
  char *memory;
  size_t size;
};

// callback for libcurl when data is read from http response
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = (char*) realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}


MOOCRestApi::MOOCRestApi()
{

}

MOOCRestApi::~MOOCRestApi()
{
  curl_global_cleanup();
}


std::string post(const char *url, const char *user, const char *pass, const char *json)
{


}


std::string MOOCRestApi::Login(const char* urlStr, const char* userStr, const char* passStr)
{
  this->url = urlStr;
  this->user = userStr;
  this->pass = passStr;
//  std::string resp = this->Request(urlStr);

  this->url = "https://97.76.231.101/events/new";
  this->user = "myuser";
  this->pass = "mypass";

  string json = "{ \"Type\": \"GAZEBO_OBJECTIVE_COMPLETED\", \"Results\": { ";
  json += "\"startTime\": \"2014-07-08T16:01:18.636Z\", ";
  json += "\"completedAt\": \"2014-07-08T16:23:15.636Z\", ";
  json += "\"timeSpent\": \"1087.3\", \"world\": \"simple.world\", ";
  json += "\"score\": \"11\", \"issues\": [\"Simple MOOC plugin\", ";
  json += "\"connecting pully\"] }, \"Resources\": [\"intro.mpg\", ";
  json += "\"lesson1.mpg\"], \"userid\": \"myuser\"  }";

   cout << endl << endl << json << endl << endl;

  string resp;
  resp = this->Request("https://97.76.231.101/events/new", json.c_str());
//  resp = this->Request("https://97.76.231.101/events/all");

//  this->isLoggedIn = true;

  return resp;
}


std::string MOOCRestApi::Request(const char* _reqUrl, const char* _postJsonStr)
{

  if(this->url.empty())
    throw MOOCException("A URL must be specified for the Learning Companion");
  if(this->user.empty())
    throw MOOCException("No user specified for the Learning Companion. Please login.");

  // this->url = "https://97.76.231.101/events/all";
  cout << "MOOCRestApi::Request" << endl;
  // verify login credentials
  CURL *curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, url.c_str() );


  struct data config;
  config.trace_ascii = 1; /* enable ascii tracing */
  curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);
  curl_easy_setopt(curl, CURLOPT_DEBUGDATA, &config);


  struct MemoryStruct chunk;
  chunk.memory = (char*) malloc(1);  // will be grown as needed by the realloc above
  chunk.size = 0;            // no data at this point

  bool secure = false;
  if(!secure)
  {
    // skip peer verification
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    // skip host verification
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
  }

  // send all data to this function
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  //  we pass our 'chunk' struct to the callback function
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
  // some servers don't like requests that are made without a user-agent
  // field, so we provide one
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  // set user name and password for the authentication
  // curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);
  curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
  string userpass = this->user + ":" + this->pass;
  curl_easy_setopt(curl, CURLOPT_USERPWD, userpass.c_str());


  // when things go wrong
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

  // is this a POST?
  struct curl_slist *slist = NULL;
  if(_postJsonStr)
  {
    curl_easy_setopt (curl, CURLOPT_UPLOAD, 0L);  // disable PUT
    curl_easy_setopt(curl, CURLOPT_POST, 1); // enable POST
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, _postJsonStr);

    slist = curl_slist_append(slist, "Content-Type: application/json");
    slist = curl_slist_append(slist, "charsets: utf-8");
//    slist = curl_slist_append( slist, "Expect:");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
  }

  CURLcode res = curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  if(res != CURLE_OK) {
    cerr << "Request to " << url << " failed: " << curl_easy_strerror(res) << endl;
    throw MOOCException(curl_easy_strerror(res));
  }

  // copy the data into a string
  std::string response(chunk.memory, chunk.size);

  // clean up
  curl_slist_free_all(slist);

  if(chunk.memory)
    free(chunk.memory);

  return response;
}

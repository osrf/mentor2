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

  string json = "{ \"Type\": \"GAZEBO_OBJECTIVE_COMPLETED\", \"Results\": { \
        \"startTime\": \"2014-07-08T16:01:18.636Z\", \
        \"completedAt\": \"2014-07-08T16:23:15.636Z\", \
        \"timeSpent\": \"1087.3\", \
        \"world\": \"simple.world\", \
        \"score\": \"11\", \
        \"issues\": [\"Simple MOOC plugin\", \"connecting pully\"] \
    }, \
    \"Resources\": [\"intro.mpg\", \"lesson1.mpg\"], \
    \"userid\": \"myuser\"  }";

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
    std::string json(_postJsonStr);
    curl_easy_setopt (curl, CURLOPT_UPLOAD, 0L);  // disable PUT
    curl_easy_setopt(curl, CURLOPT_POST, 1); // enable POST
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str()); 
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, json.length());

    slist = curl_slist_append(slist, "Content-Type: application/json");
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


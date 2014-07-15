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
#include <iostream>
#include "MOOCRestApi.hh"

using namespace gazebo;
using namespace std;


MOOCRestApi::MOOCRestApi()
{

}

MOOCRestApi::~MOOCRestApi()
{
}


std::string MOOCRestApi::Login(const char* urlStr, const char* userStr, const char* passStr)
{
  this->url = urlStr;
  this->user = userStr;
  this->pass = passStr;
  std::string resp = this->Request(urlStr);
  this->isLoggedIn = true;
  return resp;
}


std::string MOOCRestApi::Request(const char* _req)
{

    // this->url = "https://97.76.231.101/events/all";
    cout << "MOOCRestApi::Request" << endl;
    // verify login credentials
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str() );

    bool secure = false;
    if(!secure)
    {
      // skip peer verification
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
      // skip host verification
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    }
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long)CURLAUTH_ANY);

    // set user name and password for the authentication  
    string userpass = this->user + ":" + this->pass;
    curl_easy_setopt(curl, CURLOPT_USERPWD, userpass.c_str());
    
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if(res != CURLE_OK) {
      cerr << "Request to " << url << " failed: " << curl_easy_strerror(res) << endl;
      throw MOOCException(curl_easy_strerror(res));
    }
    return std::string("watta watta watta");
}


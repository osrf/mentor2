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
  :isLoggedIn(false)
{

}

MOOCRestApi::~MOOCRestApi()
{
  cout << "~MOOCRestApi()" << endl;    
}


void MOOCRestApi::Login(const char* urlStr, const char* userStr, const char* passStr)
{
    this->url = urlStr;
    this->user = userStr;
    this->pass = passStr;

    isLoggedIn = false;

    cout << "MOOCRestApi::Login" << endl;
    // verify login credentials
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str() );
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if(res != CURLE_OK) {
      cerr << "Login to " << url << " failed: " << curl_easy_strerror(res) << endl;
      throw MOOCException(curl_easy_strerror(res));
    }
    isLoggedIn = true;
    cout << "User " << user << "  is logged in? :  " << isLoggedIn << endl;
}


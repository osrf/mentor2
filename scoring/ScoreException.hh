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
 * limitations under the License.k *
*/  
#ifndef _SCOREEXCEPTION_HH_
#define _SCOREEXCEPTION_HH_

#include <stdexcept>

namespace gazebo
{ 

  class ScoreException : public std::runtime_error
  {
    /// \brief Constructor
    /// param[in] m the exception message 
    public: ScoreException(const char*m) :std::runtime_error(m){}
  };
    
}


#endif

/*
 * copyright (C) 2014 Open Source Robotics Foundation
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


#ifndef  _MOOCLOGIN_DIALOG_HH_
#define _MOOCLOGIN_DIALOG_HH_

#include <gazebo/gui/qt.h>

namespace gazebo
{

  class LearningCompanion
  {
    public: LearningCompanion(const char* url, const char* user, const char* pass);
    public: bool Login(std::string &msg); 

    private:
      std::string url;
      std::string user;
      std::string pass;
  };

  namespace gui
  {

    class MOOCUIWidget;

    class MOOCLoginDialog : public QDialog
    {
      Q_OBJECT
        
      public:
        /// \brief Consotructor
        /// \param[in] _parent Parent QWidget
        MOOCLoginDialog(QWidget *_parent = 0, const char* defautlUrl= "");

      signals:
        void acceptLogin(QString &url, QString& username, QString& password);

      public slots:
        void slotAcceptLogin();

      private:
        /// \brief A label for the url component
        QLabel *labelUrl;
        /// \brief A label for the username component
        QLabel *labelUsername;
        /// \brief A label for the password
        QLabel *labelPassword;
        /// \brief A text field for the default url
        QLineEdit* editUrl;
        /// \brief A text field to enter the user name
        QLineEdit* editUsername;
        /// \brief A text field to enter the password
        QLineEdit* editPassword;

        /// \brief A label to displau errors and information
        QLabel *labelInfo;

        /// \brief The standard dialog buttons
        QDialogButtonBox *buttons;

      public: std::string getUsername() {return username;}
      public: std::string getPassword() {return password;}
      public: std::string getUrl() {return url;}

      private:
        std::string username;
        std::string password;
        std::string url;
    };
  }
}

#endif

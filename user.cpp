#include "user.h"

User::User()
{
}
User::~User()
{
}

User::User(QString firstName, QString lastName, QString userID, QString photoURL)
{
    this->setInformation(firstName,lastName,userID,photoURL);
}

void User::setInformation(QString firstName, QString lastName, QString userID, QString photoURL)
{
    this->setFirstName(firstName);
    this->setLastName(lastName);
    this->setUserID(userID);
    this->setPhotoUrl(photoURL);
}

void User::getInformation(QString &firstName, QString &lastName, QString &userID, QString &photoURL)
{
    firstName = this->firstName;
    lastName = this->lastName;
    userID = this->userID;
    photoURL = this->photoURL;
}

void User::setFirstName(QString firstName)
{
     this->firstName = firstName;
}

void User::setLastName(QString lastName)
{
     this->lastName = lastName;
}

void User::setUserID(QString userID)
{
    this->userID = userID;
}

void User::setPhotoUrl(QString photoURL)
{
    this->photoURL = photoURL;
}

QString User::getFirstName()
{
    return firstName;
}

QString User::getLastName()
{
    return lastName;
}

QString User::getUserID()
{
    return userID;
}

QString User::getPhotoUrl()
{
    return photoURL;
}

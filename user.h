#ifndef USER_H
#define USER_H
#include <QString>

class User
{
public:
    User();
    User(QString firstName, QString lastName, QString userID, QString photoURL = "");

    void setInformation(QString firstName, QString lastName, QString userID, QString photoURL);
    void getInformation(QString &firstName, QString &lastName, QString &userID, QString &photoURL);

    void setFirstName(QString firstName);
    void setLastName(QString lastName);
    void setUserID(QString userID);
    void setPhotoUrl(QString photoURL);

    QString getFirstName();
    QString getLastName();
    QString getUserID();
    QString getPhotoUrl();

    ~User();
private:
    QString firstName;
    QString lastName;
    QString userID;
    QString photoURL;


};

#endif // USER_H

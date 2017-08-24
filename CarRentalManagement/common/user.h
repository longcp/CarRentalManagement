#ifndef USER_H
#define USER_H

#include <QObject>
#include <datatype.h>

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent = 0);
    User(QString name, QString passwd, UserRight right) {
        this->name = name;
        this->passward = passwd;
        this->right = right;
    }

    User& operator=(const User& u) {
        name = u.name;
        passward = u.passward;
        right = u.right;
        return *this;
    }

    User(const User &u) {
        name = u.name;
        passward = u.passward;
        right = u.right;
    }

    QString getUserRightStr(UserRight right) {
        switch(right) {
        case RIGHT_ROOT:
            return USER_RIGHT_ROOT_STR;
        case RIGHT_NORMAL:
            return USER_RIGHT_NORMAL_STR;
        default:
            break;
        }

        return "";
    }

    UserRight getUserRightInStr(QString right) {
        if (right == USER_RIGHT_ROOT_STR)
            return RIGHT_ROOT;
        else if (right == USER_RIGHT_NORMAL_STR)
            return RIGHT_NORMAL;
        else
            return UNKNOWN_RIGHT;
    }

    bool isRoot() {
        return right == RIGHT_ROOT ? true : false;
    }

    QString name;
    QString passward;
    UserRight right;

signals:

public slots:
};

#endif // USER_H

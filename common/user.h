#ifndef USER_H
#define USER_H

#include <QObject>

#define USER_RIGHT_ROOT_STR     "管理员"
#define USER_RIGHT_NORMAL_STR   "普通用户"

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent = 0);
    enum UserRight {RIGHT_ROOT, RIGHT_NORMAL, UNKNOWN_RIGHT};
    User(QString name, QString passwd, UserRight right) {
        this->name = name;
        this->passward = passwd;
        this->right = right;
    }

    QString name;
    QString passward;
    UserRight right;

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

signals:

public slots:
};

#endif // USER_H

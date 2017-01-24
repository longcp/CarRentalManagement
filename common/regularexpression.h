#ifndef REGULAREXPRESSION_H
#define REGULAREXPRESSION_H

#include <QObject>

class RegularExpression : public QObject
{
    Q_OBJECT
public:
    explicit RegularExpression(QObject *parent = 0);

    /**
     * @brief 英文字母和数字
     */
    static const QRegExp getLetterAndNumRegExp() {
        QRegExp letterAndNumRegExp("[a-zA-Z0-9]+$");
        return letterAndNumRegExp;
    }
    /**
     * @brief email规则
     */
    static const QRegExp getEmailRegExp() {
        QRegExp emailRegExp("[0-9a-zA-Z@_.]+$");
        return emailRegExp;
    }
    /**
     * @brief 名字规则
     */
    static const QRegExp getNameRegExp() {
        QRegExp nameRegExp("[a-zA-Z0-9\w\u4E00-\u9FA5()]+$");               //名字规则
        return nameRegExp;
    }
    /**
     * @brief 手机固话传真
     */
    static const QRegExp getTelRegExp() {
        QRegExp telRegExp("\d{3,4}-)?\d{7,8})$|^((1[0-9][0-9]\d{8}$");      //手机固话传真
        return telRegExp;
    }

signals:

public slots:
};

#endif // REGULAREXPRESSION_H

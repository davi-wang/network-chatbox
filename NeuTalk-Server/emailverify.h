#ifndef EMAILVERIFY_H
#define EMAILVERIFY_H

#include <QDateTime>
#include <QMap>
#include "mailsender.h"

class EmailVerify
{
public:
    enum VerificationError {
        NoError = 0,
        InvalidEmail = 1,
        Timeout = 2,
        WrongCode = 3
    };
    struct Node {
        QDateTime datetime;
        QByteArray code;
    };
    EmailVerify();
    void requestVerify(const QByteArray &);
    VerificationError verify(const QByteArray &, const QByteArray &);

private:
    QByteArray generateCode();

    unsigned int verification_code_length;
    MailSender* sender;
    QMap<QByteArray, Node> storage;
};

#endif // EMAILVERIFY_H

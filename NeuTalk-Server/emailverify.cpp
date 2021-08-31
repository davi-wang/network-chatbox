#include "emailverify.h"

EmailVerify::EmailVerify()
{
    srand(time(nullptr));
    verification_code_length = 6;
}

void EmailVerify::requestVerify(const QByteArray &email)
{
    QByteArray verification_code = generateCode();
    // 注意 此函数将阻塞式发邮件 可能造成服务端短期无响应
    qDebug() << "[INFO] Sending verification email to"  <<  QString(email);
    sender.sendEmail(email, "[NeuTalk] 您的验证码是" + QString(verification_code), "Sorry.");
    QDateTime now = QDateTime::currentDateTime();
    storage[email] = Node({now, verification_code});
}
EmailVerify::VerificationError EmailVerify::verify(const QByteArray &email, const QByteArray &code)
{
    if (!storage.contains(email)) {
        return InvalidEmail;
    }
    QDateTime now = QDateTime::currentDateTime();
    Node node = storage.value(email);
    if (node.datetime.addSecs(30 * 60) < now) {
        return Timeout;
    }
    if (node.code != code) {
        return WrongCode;
    }
    return NoError;
}

QByteArray EmailVerify::generateCode()
{
    QByteArray code;
    for (size_t i = 0; i < verification_code_length; i++) {
        int digit = rand() % 10;
        code += char('0' + digit);
    }
    return code;
}

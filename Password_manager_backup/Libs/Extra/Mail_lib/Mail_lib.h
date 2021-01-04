#pragma once
#ifndef MAIL_LIB_FLAG
#define MAIL_LIB_FLAG

#include <Poco/Net/SecureStreamSocket.h>
#include <Poco/Net/SMTPClientSession.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/MailMessage.h>
#include <Poco/ByteOrder.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/SecureSMTPClientSession.h>
#include <memory>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/NetSSL.h>


int send_mail(const char* recipient, const char* subject, const char* content);

#endif //MAIL_LIB_FLAG
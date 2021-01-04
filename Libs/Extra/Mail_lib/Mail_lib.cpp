#include <iostream>
#include "Mail_lib.h"

int send_mail(const char* recipient, const char* subject, const char* content) {
    Poco::Net::MailMessage msg;
    msg.addRecipient (Poco::Net::MailRecipient (Poco::Net::MailRecipient::PRIMARY_RECIPIENT, recipient));
    msg.setSubject(subject);
    msg.encodeWord(content);
    msg.setContent(content);


    //initialize connection
    Poco::UInt16 effective_port(587);

    Poco::Net::SecureSMTPClientSession session("smtp.gmail.com", effective_port);

    session.open();

    Poco::Net::initializeSSL();

    Poco::Net::InvalidCertificateHandler* ptrHandler = new Poco::Net::AcceptCertificateHandler(false);

    Poco::Net::Context::Ptr ptrContext = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_RELAXED, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");

    Poco::Net::SSLManager::instance().initializeClient(0, ptrHandler, ptrContext);

    try
    {
        // SSL
        session.login();
        if (session.startTLS(ptrContext))
        {
             session.login(Poco::Net::SMTPClientSession::AUTH_LOGIN, "scaccabarozzi.enea@gmail.com", "Ezio_log");
             session.sendMessage(msg);
        }
        session.close();
        Poco::Net::uninitializeSSL();
    }
    catch (Poco::Net::SMTPException& e)
    {
        std::cout << e.message() << std::endl;
        session.close();
        Poco::Net::uninitializeSSL();
        std::cin.get();
        return -1;
    }
    std::cin.get();
    return 0;
}


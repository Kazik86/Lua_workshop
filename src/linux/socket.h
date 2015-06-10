#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <sys/un.h>
#include <sys/socket.h>

class eSocket
{
public:
    eSocket(const std::string& aAddress);
    ~eSocket();

    int getFd() const { return iFd; }
    ssize_t receive(void* aBuffer, size_t aLength);

private:
    eSocket(const eSocket& aOther);
    eSocket& operator=(const eSocket& aOther);

    void setServerAddress(const std::string& aAddress);
    void bind(int aFd, const sockaddr_un* aSocketAddr);

private:
    sockaddr_un iSocketAddr;
    int iFd;
    std::string iServerAddr;
};

#endif // SOCKET_H

#include "socket.h"

#include <cstdio>
#include <cerrno>
#include <unistd.h>
#include <stdexcept>

eSocket::eSocket(const std::string& aAddress)
{
    iFd = ::socket(AF_UNIX, SOCK_DGRAM | SOCK_NONBLOCK, 0);
    if (iFd == -1)
	throw std::runtime_error("Couldn't create socket.");

    try {
	setServerAddress(aAddress);
	bind(iFd, &iSocketAddr);
    } catch(...) {
	::close(iFd);
	throw;
    }
}

eSocket::~eSocket()
{
    ::close(iFd);
}

void eSocket::setServerAddress(const std::string& aAddress)
{
    if (::remove(aAddress.c_str()) == -1 && errno != ENOENT)
	throw std::runtime_error("Couldn't remove existing socket path.");

    ::memset(&iSocketAddr, 0, sizeof(sockaddr_un));
    iSocketAddr.sun_family = AF_UNIX;
    ::strncpy(iSocketAddr.sun_path, aAddress.c_str(), sizeof(iSocketAddr.sun_path) - 1);
    iServerAddr = aAddress;
}

void eSocket::bind(int aFd, const sockaddr_un* aSocketAddr)
{
    if (::bind(aFd, (sockaddr*) aSocketAddr, sizeof(sockaddr_un)) == -1)
	throw std::runtime_error("Couldn't bind socket.");
}

ssize_t eSocket::receive(void* aBuffer, size_t aLength)
{
    sockaddr_un clientAddr;
    socklen_t len = sizeof(sockaddr_un);
    return recvfrom(iFd, aBuffer, aLength, 0, (sockaddr*) &clientAddr, &len);
}

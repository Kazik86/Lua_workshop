#ifndef REAL_TIME_UPDATE_H
#define REAL_TIME_UPDATE_H

#include "socket.h"

struct sModule;

class eRealTimeUpdate
{
public:
    eRealTimeUpdate(const std::string& aSocketAddress);

    void update();
    const sModule* getModule() const { return iModule; }
    const std::string& getSnippetName() const { return iSnippetName; }
    bool isAffectDerived() const { return iAffectDerived; }

private:
    eRealTimeUpdate(const eRealTimeUpdate& aOther);
    eRealTimeUpdate& operator=(const eRealTimeUpdate& aOther);

    void parseMsg(char* aBuf, /* out */ std::string& aModule, /* out */ std::string& aFile,
                  /* out */ std::string& aSnippetName, /* out */ bool& aAffectDerived);

private:
    eSocket iSocket;
    const sModule* iModule;
    std::string iSnippetName;
    bool iAffectDerived;
};

#endif // REAL_TIME_UPDATE_H

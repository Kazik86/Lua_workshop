#include "realTimeUpdate.h"

#include "game.h"
#include "luaModule.h"
#include "luaState.h"

#include <iostream>

eRealTimeUpdate::eRealTimeUpdate(const std::string& aSocketAddress):
    iSocket(aSocketAddress)
{

}

void eRealTimeUpdate::update()
{
    lua_State* lua = eGame::getMe()->getLua()->getRaw();

    if (iModule && !iSnippetName.empty()) {
        eLuaModuleMgr::getMe()->removeSnippet(lua, iModule->iRef, iSnippetName.c_str());
    }

    iModule = 0;
    char buf[256] = {0};
    ssize_t n = iSocket.receive(buf, sizeof(buf));

    if (n > 0) {
        std::cout << "-- RTU info ------------------------------------------------------------\n"
                  << "RTU: received " << n << " bytes." << std::endl;

        std::string module, file;
        parseMsg(buf, module, file, iSnippetName, iAffectDerived);

        try {
            if (iSnippetName.empty())
                iModule = eLuaModuleMgr::getMe()->reloadModule(lua, module);
            else
                iModule = eLuaModuleMgr::getMe()->callSnippet(lua, module, file);

        } catch (const std::exception& aErr) {
            std::cout << "RTU error: " << aErr.what() << std::endl;
            lua_settop(lua, 0);
        }
    }
}

void eRealTimeUpdate::parseMsg(char* aBuf, /* out */ std::string& aModule, /* out */ std::string& aFile,
                               /* out */ std::string& aSnippetName, /* out */ bool& aAffectDerived)
{
    const char* delim = ";";

    char* token = ::strtok(aBuf, delim);
    aModule = token ? token : "";

    token = ::strtok(0, delim);
    aFile = token ? token : "";

    token = ::strtok(0, delim);
    aSnippetName = token ? token : "";

    token = ::strtok(0, delim);
    aAffectDerived = token ? strcmp(token, "true") ? false : true : false;

    std::cout << "RTU: module '" << aModule  << "', file '" << aFile << "', function '" << aSnippetName << "', affect derived '" << aAffectDerived << "'" << std::endl;
}

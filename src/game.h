#ifndef GAME_H
#define GAME_H

#ifdef KPR_REAL_TIME_UPDATE
#include "linux/socket.h"
#endif

class eLuaState;
struct sGameResources;
class sModule;

class eGame
{
public:
    eGame();
    ~eGame();

    static eGame* getMe() { return iMe; }
    static void cleanup();

    eLuaState* getLua();
    void mainLoop();

private:
    eGame(const eGame& aOther);
    eGame& operator=(const eGame& aOther);

    void createModules();
    void destroyModules();
    void handleEvents();

#ifdef KPR_REAL_TIME_UPDATE
private:
    eSocket iRtuSocket;
    const sModule* iRtuModule;
public:
    void realTimeUpdate();
    const sModule* getRtuModule() const { return iRtuModule; }
#endif

private:
    static eGame* iMe;
    bool iIsRunning;
    int iAccumulator;
    unsigned int iLastUpdateTime;
    bool iPause;
    static sGameResources* iResources;
};

#endif // GAME_H

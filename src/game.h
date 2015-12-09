#ifndef GAME_H
#define GAME_H

#ifdef KPR_REAL_TIME_UPDATE
#include "linux/realTimeUpdate.h"
#endif

#include <string>

class eLuaState;
struct sGameResources;
class sModule;

struct sGameProps
{
    sGameProps():
        iMainActorScript("scripts/Main.lua")
    {}

    std::string iMainActorScript;
};

class eGame
{
public:
    eGame(const sGameProps& aProps);
    ~eGame();

    static eGame* getMe() { return iMe; }
    static void cleanup();

    eLuaState* getLua();
    void mainLoop();
    const sGameProps& getGameProps() const { return iGameProps; }

#ifdef KPR_REAL_TIME_UPDATE
    public:
        eRealTimeUpdate& getRtu() { return iRtu; }
#endif

private:
    eGame();
    eGame(const eGame& aOther);
    eGame& operator=(const eGame& aOther);

    void createModules();
    void destroyModules();
    void handleEvents();

private:
    const sGameProps iGameProps;
#ifdef KPR_REAL_TIME_UPDATE
    eRealTimeUpdate iRtu;
#endif
    static eGame* iMe;
    bool iIsRunning;
    int iAccumulator;
    unsigned int iLastUpdateTime;
    bool iPause;
    static sGameResources* iResources;
};

#endif // GAME_H

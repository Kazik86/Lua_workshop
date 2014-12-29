#ifndef GAME_H
#define GAME_H

class eActorMgr;
class eLuaModuleMgr;
class eLuaState;
class eRenderer;
class eTextureMgr;

#include <memory>

class eGame
{
public:
    eGame();
    ~eGame();

    static eGame* getMe() { return iMe; }

    eLuaState* getLua() { return iLua.get(); }
    void mainLoop();

private:
    eGame(const eGame& aOther);
    eGame& operator=(const eGame& aOther);

    void createModules();
    void destroyModules();
    void handleEvents();

private:
    static eGame* iMe;
    bool iIsRunning;
    int iAccumulator;
    unsigned int iLastUpdateTime;
    bool iPause;

    // modules
    std::unique_ptr<eLuaState> iLua;
    std::unique_ptr<eLuaModuleMgr> iModuleMgr;
    std::unique_ptr<eActorMgr> iActorMgr;
    std::unique_ptr<eRenderer> iRenderer;
    std::unique_ptr<eTextureMgr> iTextureMgr;
};

#endif // GAME_H

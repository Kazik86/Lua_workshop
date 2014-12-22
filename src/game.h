#ifndef GAME_H
#define GAME_H

class eLuaState;
class eActorMgr;

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

    // modules
    std::unique_ptr<eLuaState> iLua;
    std::unique_ptr<eActorMgr> iActorMgr;
};

#endif // GAME_H

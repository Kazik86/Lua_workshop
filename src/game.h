#ifndef GAME_H
#define GAME_H

class eLuaState;
struct sGameResources;

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

private:
    static eGame* iMe;
    bool iIsRunning;
    int iAccumulator;
    unsigned int iLastUpdateTime;
    bool iPause;
    static sGameResources* iResources;
};

#endif // GAME_H

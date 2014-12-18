#ifndef GAME_H
#define GAME_H

class eLuaState;

namespace Game
{
    void init();
    void cleanup();
    void mainLoop();
    eLuaState& getLua();
}

#endif // GAME_H

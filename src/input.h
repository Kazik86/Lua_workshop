#ifndef INPUT_H
#define INPUT_H

#include "userdata.h"

#include <SDL2/SDL.h>

class eInput
{
    DECLARE_USERDATA_CLASS()

public:
    eInput();
    ~eInput();

    static eInput* getMe() { return iMe; }
    bool isKeyDown(SDL_Scancode aKey) { return iKeyboardState[aKey]; }

private:
    eInput(const eInput& aOther);
    eInput& operator=(const eInput& aOther);

private:
    static eInput* iMe;
    const Uint8* iKeyboardState;
};

#endif // INPUT_H

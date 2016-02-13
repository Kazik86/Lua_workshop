#include "gSdlInput.h"

#include "game.h"

// events
DEFINE_GADGET_EVENT(gSdlInput, keyboard)
DEFINE_GADGET_EVENT(gSdlInput, mouseButton)
DEFINE_GADGET_EVENT(gSdlInput, mouseMove)
DEFINE_GADGET_EVENT(gSdlInput, mouseWheel)

DEFINE_GADGET_API(gSdlInput)
{
    // events
    REGISTER_GADGET_EVENT(keyboard)
    REGISTER_GADGET_EVENT(mouseButton)
    REGISTER_GADGET_EVENT(mouseMove)
    REGISTER_GADGET_EVENT(mouseWheel)
    {0, 0}
};

DEFINE_GADGET_CLASS(gSdlInput)

gSdlInput::gSdlInput():
    iMouseWheel(false),
    iMouseWheelSetInCurrentFrame(false),
    iMouseMove(false),
    iMouseMoveSetInCurrentFrame(false)
{
}

void gSdlInput::begin(lua_State* aLua)
{
    Script::setField(aLua, iEventData, "state",    unsigned());
    Script::setField(aLua, iEventData, "scancode", int());
    Script::setField(aLua, iEventData, "button",   unsigned());
    Script::setField(aLua, iEventData, "x", int());
    Script::setField(aLua, iEventData, "y", int());
}

int gSdlInput::update(lua_State* aLua, float /* aDelta */)
{
    iMouseWheelSetInCurrentFrame = false;
    iMouseMoveSetInCurrentFrame = false;

    while (SDL_PollEvent(&iEvent)) {
        switch (iEvent.type) {
            case SDL_QUIT:
                eGame::getMe()->shutdown();
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                {
                    const SDL_KeyboardEvent& event = iEvent.key;

                    if (! event.repeat) {
                        Script::setField(aLua, iEventData, "state",    static_cast<unsigned int>(event.state));
                        Script::setField(aLua, iEventData, "scancode", static_cast<int>(event.keysym.scancode));

                        emit(aLua, keyboard, iEventData);
                    }
                }

                break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                {
                    const SDL_MouseButtonEvent& event = iEvent.button;

                    Script::setField(aLua, iEventData, "state",  static_cast<unsigned int>(event.state));
                    Script::setField(aLua, iEventData, "button", static_cast<unsigned int>(event.button));
                    Script::setField(aLua, iEventData, "x",      static_cast<int>(event.x));
                    Script::setField(aLua, iEventData, "y",      static_cast<int>(event.y));

                    emit(aLua, mouseButton, iEventData);
                }

                break;

            case SDL_MOUSEMOTION:
                {
                    if (iMouseMove) { iMouseMoveSetInCurrentFrame = true; break; }

                    Script::setField(aLua, iEventData, "state",  unsigned(1));

                    iMouseMove = true;
                    iMouseMoveSetInCurrentFrame = true;

                    emit(aLua, mouseMove, iEventData);
                }

                break;

            case SDL_MOUSEWHEEL:
                {
                    // we can't handle the lack of counter action exactly like
                    // in 'SDL_MOUSEMOTION' i.e. by setting
                    // 'iMouseWheelSetInCurrentFrame = true' because we need to
                    // send 'x' and 'y' parameters. There is no such need in
                    // 'SDL_MOUSEMOTION' cause we can read them later using
                    // 'SDL_GetMouseState'. There is no such function for wheel
                    // unfortunately.

                    if (iMouseWheel) break;

                    const SDL_MouseWheelEvent& event = iEvent.wheel;

                    Script::setField(aLua, iEventData, "state",  unsigned(1));
                    Script::setField(aLua, iEventData, "x",      static_cast<int>(event.x));
                    Script::setField(aLua, iEventData, "y",      static_cast<int>(event.y));

                    iMouseWheel = true;
                    iMouseWheelSetInCurrentFrame = true;

                    emit(aLua, mouseWheel, iEventData);
                }

                break;

            default:
                break;
        }
    }

    // because there is no counter action to 'SDL_MOUSEWHEEL' and
    // 'SDL_MOUSEMOTION' (such as 'SDL_KEYUP' and 'SDL_KEYDOWN'), we have to
    // reset respective actions in '_G.Input' manually.

    if (iMouseWheel && !iMouseWheelSetInCurrentFrame) {
        iMouseWheel = false;
        Script::setField(aLua, iEventData, "state",  unsigned(0));
        emit(aLua, mouseWheel, iEventData);
    }

    if (iMouseMove && !iMouseMoveSetInCurrentFrame) {
        iMouseMove = false;
        Script::setField(aLua, iEventData, "state",  unsigned(0));
        emit(aLua, mouseMove, iEventData);
    }

    return 0;
}


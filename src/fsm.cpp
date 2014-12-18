#include "fsm.h"

namespace
{
    const unsigned int KExecutingUpdate = 0x00000001;
    const unsigned int KExecutingLeave  = 0x00000002;
}

eFsm::eFsm():
    iFlags(0)
{
}

eFsm::~eFsm()
{
}

void eFsm::update(lua_State* aLua)
{
    setFlag(KExecutingUpdate);
    iCurrentState.execute(aLua, "update");
    unsetFlag(KExecutingUpdate);
}

void eFsm::shift(lua_State* aLua)
{
    //to zapobiega przepełnieniu stosu gdy ktoś w leave'ie stanu
    //spróbuje zrobić operację 'shift'
    if(checkFlag(KExecutingLeave))
	return 0;

    me->iAttribs |= KeFsm_executingLeave;
    me->iCurrentState.execute(aLua, "leave");
    me->iAttribs &= ~KeFsm_executingLeave;

    for(uint i = 0; i < me->iGadgetsNum; ++i)
    {
        eGadget* g = me->iGadgets[i];
        
        if(g->iIsEnabled)
        {
            g->disable();
            g->iIsEnabled = false;
        }

        if(g->iIsPropModified)
            g->restoreDefaultPropValues();
    }

    me->iCurrentState.setState(aLua, KPR_LUA_GET_STRING(aLua, -1));
    me->iCurrentState.execute(aLua, "enter");
    
    return 0;
}


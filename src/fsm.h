#ifndef FSM_H
#define FSM_H

class eLuaState;

class eFsm
{
public:
    eFsm();
    ~eFsm();

    void update(lua_State* aLua);

private:
    eFsm(const eFsm& aOther);
    eFsm& operator=(const eFsm& aOther);

    void shift(lua_State* aLua);

    void setFlag(unsigned int aFlag) { iFlags |= aFlag; }
    void unsetFlag(unsigned int aFlag) { iFlags &= ~aFlag; }
    bool checkFlag(unsigned int aFlag) const { return iFlags & aFlag; }

private:
    unsigned int iFlags;
};

#endif // FSM_H

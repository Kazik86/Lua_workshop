#include "luaModule.h"

#include "luaState.h"

#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace
{

int replaceEnv(lua_State* aLua)
{
    lua_getfield(aLua, 1, "Data");

    if (! lua_isnil(aLua, -1)) {
	lua_pushvalue(aLua, 2);
	lua_gettable(aLua, -2);

	if (lua_isfunction(aLua, -1)) {
	    /* nie każda funkcja musi posiadać jako upvalue swoje _ENV. Z tego co widzę
	     * z eksperymentów, _ENV nie mają np. funkcje, które operują tylko na
	     * przesłanych jej argumentach (brak odwołań do jakichkolwiek danych spoza
	     * zakresu funkcji). W dodatku nawet jeśli mamy _ENV to niekoniecznie jako
	     * upvalue na pozycji 1.
	     */
	    for (int i = 1; ; ++i) {
		const char* upvalueName = lua_getupvalue(aLua, -1, i);
		if (upvalueName == 0)
		    break;
		else if(strcmp(upvalueName, "_ENV") == 0) {
		    lua_pushvalue(aLua, 1);
		    lua_setupvalue(aLua, -3, i);
		    lua_pop(aLua, 1);
		    break;
		}

		lua_pop(aLua, 1);
	    }
	} else if (lua_isnil(aLua, -1)) {
	    // tu wystarczy log bo za chwilę gdzieś dalej i tak posypie się
	    // błąd w rodzaju 'attempt to index local 'foo' (a nil value)'
	    // i program zostanie przerwany. Poza tym dzięki temu, że nie
	    // przerywam programu poniżej, dostaję coś w rodzaju
	    // 'callstack' dla kolejnych poziomów dziedziczenia i ostatni
	    // log przed wystąpieniem błędu będzie wskazywał na winnego.
	    lua_getfield(aLua, 1, "Script");
	    std::cerr << "Uninitialized variable: '" << lua_tostring(aLua, 2)  << "' in script: " << lua_tostring(aLua, -1) << std::endl;
	    lua_pop(aLua, 1);
	}
    }

    return 1;
}

int derive(lua_State* aLua)
{
    luaL_checkstring(aLua, 1);

    const sModule& super = eLuaModuleMgr::getMe()->load(aLua, lua_tostring(aLua, 1));
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, super.iRef);

    return 1;
}

}

DEFINE_USERDATA_API(eLuaModuleMgr)
{
    {"derive", ::derive},
    {0, 0}
};

DEFINE_USERDATA_CLASS(eLuaModuleMgr)

sModule::sModule():
    iRef(LUA_NOREF),
    iLua(0)
{}

sModule::~sModule()
{
    luaL_unref(iLua, LUA_REGISTRYINDEX, iRef);
}

eLuaModuleMgr* eLuaModuleMgr::iMe = 0;

eLuaModuleMgr::eLuaModuleMgr()
{
    if (iMe)
	throw std::runtime_error("eLuaModuleMgr: multiple instances not allowed.");

    iMe = this;
}

eLuaModuleMgr::~eLuaModuleMgr()
{
    iMe = 0;
}

void eLuaModuleMgr::setClass(lua_State* aLua, sModule& aModule)
{
    // na stosie leżą:
    // -2: tablica główna;
    // -1: tablica 'Data'
    lua_getfield(aLua, -1, "Class");

    if (! lua_isstring(aLua, -1))
	throw std::runtime_error("Non-existent, empty or wrong type in field 'Class'.");

    /* spr. czy nazwa klasy nie jest użyta jako identyfikator np.
     * Class = "Foo"
     * Foo = 1
     */
    lua_pushvalue(aLua, -1);
    lua_rawget(aLua, -3);
    if (! lua_isnil(aLua, -1))
	throw std::runtime_error("Class name is used as an identifier.");

    lua_pop(aLua, 1);

    aModule.iClass = lua_tostring(aLua, -1);

    lua_pushvalue(aLua, -3);
    lua_settable(aLua, -4);
    lua_pop(aLua, 1);
}

void eLuaModuleMgr::setGlobal(lua_State* aLua)
{
    lua_pushglobaltable(aLua);
    lua_setfield(aLua, -2, "_G");
}

void eLuaModuleMgr::setScript(lua_State* aLua, const std::string& aScript)
{
    lua_pushstring(aLua, aScript.c_str());
    lua_setfield(aLua, -2, "Script");
}

sModule* eLuaModuleMgr::checkClassUniqueness(lua_State* aLua, sModule& aModule)
{
    sModule* result = 0;
    lua_rawgeti(aLua, LUA_REGISTRYINDEX, aModule.iRef);
    lua_getfield(aLua, -1, "Class");

    // if it is called then there is smth wrong with setClass
    assert(lua_isstring(aLua, -1));

    std::string c = lua_tostring(aLua, -1);

    for (sModule* m : aModule.iInheritanceHierarchy) {
	lua_rawgeti(aLua, LUA_REGISTRYINDEX, m->iRef);
	lua_getfield(aLua, -1, "Class");

	assert(lua_isstring(aLua, -1));

	if (c == lua_tostring(aLua, -1))
	    result = m;

	lua_pop(aLua, 2);

	if (result) break;
    }

    lua_pop(aLua, 2);

    return result;
}

sModule& eLuaModuleMgr::add(lua_State* aLua, const std::string& aName)
{
    try {
	sModule& m = iModules[aName];

	if (m.iRef == LUA_NOREF) {
	    if (m.iLua != 0) {
		// m.iLua pełni rolę strażnika zapętlonego dziedziczenia
		// tzn. gdy skrypt A dziedziczy po skrypcie, który znów
		// dziedziczy po A
		throw std::runtime_error("Circular inheritance detected!");
	    }

	    m.iLua = aLua;

	    /* główna tablica Aktora, zawiera pola:
	     *     Data - tablica do której będzie za chwilę ładowany skrypt;
	     *     __index - bo tablica główna będzie jednocześnie swoją metatablicą;
	     *     Script - ścieżka skryptu;
	     *     'NazwaKlasy' - to co w pliku .lua zapisano w 'Class'.
	     *
	     * W tablicy 'Data' jest wszystko to co zdefiniowano z poziomu
	     * skryptu plus namespace'y używane w skryptach np.: _G;
	     * W tablicy głównej wszystkie inne dodatkowe dane.
	     *
	     * Podział na tablicę 'główną' i 'Data' jest podyktowany
	     * koniecznością podmiany _ENV dla każdego wywołania funkcji. Aby
	     * tą podmianę uczynić, muszę wiedzieć kiedy następuje to
	     * wywołanie. Jedynym sposobem aby się tego dowiedzieć jest
	     * umieszczenie zawartości skrytpu, a więc również definicji funkcji, w
	     * oddzielnej tablicy. Dostęp do tej tablicy odbywa się za pomocą funkcji
	     * zapisanej w '__index', a w niej sprawdzam czy poszukiwany
	     * identyfikator jest funkcją. Jeśli tak - podmieniam _ENV.
	     */
	    lua_newtable(aLua);
	    setScript(aLua, aName);

	    // do tablicy 'Data' załadowane zostaną wszystkie dane ze skryptu
	    lua_newtable(aLua);
	    setGlobal(aLua);
	    lua_setfield(aLua, -2, "Data");

	    lua_pushcfunction(aLua, replaceEnv);
	    lua_setfield(aLua, -2, "__index");
	    
	    // główna tablica jest jednocześnie swoją metatablicą
	    lua_pushvalue(aLua, -1);
	    lua_setmetatable(aLua, -2);

	    if (luaL_loadfile(aLua, aName.c_str()) != LUA_OK)
		throw std::runtime_error(lua_tostring(aLua, -1));

	    lua_getfield(aLua, -2, "Data");
	    lua_setupvalue(aLua, -2, 1);

	    if (lua_pcall(aLua, 0, 0, 0) != LUA_OK)
		throw std::runtime_error(lua_tostring(aLua, -1));

	    // hierarchia dziedziczenia
	    lua_getfield(aLua, -1, "Data");
	    lua_getfield(aLua, -1, "Super");

	    if (lua_istable(aLua, -1)) {
		// 'Data' jest swoją własną metatablicą
		lua_pushvalue(aLua, -2);
		lua_setmetatable(aLua, -3);

		// Data[__index] = Super
		lua_pushvalue(aLua, -1);
		lua_setfield(aLua, -3, "__index");

		lua_getfield(aLua, -1, "Script");
		sModule& super = iModules[std::string(lua_tostring(aLua, -1))];
		lua_pop(aLua, 1);

		m.iInheritanceHierarchy = super.iInheritanceHierarchy;
		m.iInheritanceHierarchy.push_front(&super);
	    } else if (! lua_isnil(aLua, -1))
		throw std::runtime_error("'Super' must be a table.");

	    lua_pop(aLua, 1);

	    setClass(aLua, m);

	    m.iRef = luaL_ref(aLua, LUA_REGISTRYINDEX);
	    m.iScript = aName;

	    if (sModule* redef = checkClassUniqueness(aLua, m))
		throw std::runtime_error("Class defined in " + m.iScript + " redefinied in " + redef->iScript);
	}

	return m;

    } catch (const std::exception& aErr) {
	throw std::runtime_error("While loading module '" + aName + "': " + aErr.what());
    }
}


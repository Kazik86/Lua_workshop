app_NAME := lua_test.exe
app_SRC  := $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
app_OBJ  := $(app_SRC:.cpp=.o)
app_DEP  := $(app_SRC:.cpp=.d)

test_DIR      := tests
test_NAME     := unit_test.exe
test_SRC      := $(wildcard $(test_DIR)/*.cpp) $(filter-out src/main.cpp, $(app_SRC))
test_OBJ      := $(test_SRC:.cpp=.o)
test_DEP      := $(test_SRC:.cpp=.d)
test_LDLIBS   := -lUnitTest++

CXX := g++
CXXFLAGS += -g -Wall -Wextra -std=c++0x -pedantic -I./src -I./system/include
LDFLAGS  += -L./system/lib
LDLIBS   += -llua -lSDL2 -lSDL2_image -ldl

ifdef KPR_REAL_TIME_UPDATE
    CXXFLAGS += -DKPR_REAL_TIME_UPDATE
endif

.PHONY: all test clean

all: $(app_NAME)

test: $(test_NAME)

$(app_NAME): $(app_OBJ)
	$(LINK.cpp) $^ $(LDLIBS) -o $@

$(test_NAME): $(test_OBJ)
	$(LINK.cc) $(test_OBJ) $(test_LDLIBS) $(LDFLAGS) $(LDLIBS) -o $@

clean:
	@- $(RM) $(app_NAME)
	@- $(RM) $(app_OBJ)
	@- $(RM) $(app_DEP)
	@- $(RM) $(test_NAME)
	@- $(RM) $(test_OBJ)
	@- $(RM) $(test_DEP)

ifneq "$(MAKECMDGOALS)" "clean"
  -include $(app_DEP)
endif

ifeq "$(MAKECMDGOALS)" "test"
  -include $(test_DEP)
endif

define make-depend
  $(CXX) -MM -MF $3 -MP -MT $2 $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $1
endef

%.o: %.cpp
	@$(call make-depend,$<,$@,$(subst .o,.d,$@))
	$(COMPILE.cpp) $< -o $@

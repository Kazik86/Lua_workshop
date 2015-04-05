gvim --cmd "set exrc" $(find . \( -path ./.git -o -path ./lua  -o -path ./system  -o -path ./android-project \) -prune -o \( -name '*.lua' -o -name '*.cpp' -o -name '*.h' \) -type f -print)

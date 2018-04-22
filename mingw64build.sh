# Run with sh mingw64build.sh

COMPILE="g++ -g -D MINGW -std=c++14 -static-libgcc -static-libstdc++ -I./include/ -I./thirdparty/old/glm/ -o lab.exe ./src/*.cpp -lmingw32 -lSDL2main -lSDL2 -mwindows"

echo $COMPILE
echo "-------------------------------------------"
eval $COMPILE

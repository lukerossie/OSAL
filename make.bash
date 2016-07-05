g++ -c -fPIC -std=c++11 cpp/net.cpp cpp/input.cpp cpp/system.cpp cpp/sound.cpp cpp/graphics.cpp cpp/util.cpp -O0 -pedantic-errors -Wall -Wextra -Werror
rm *.lib
ar rcs OSAL.lib *.o
rm *.o

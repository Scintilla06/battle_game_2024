cmake -S . -B ./build -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
cd build
make
./src/battle_game/battle_game
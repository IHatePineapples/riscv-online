#!/bin/sh

############################## Currently broken ##############################


# Configure, build and install Boost (locally), for cross-compiling ONLY
top=$(dirname "$(readlink -f "$0")")
BOOSTDIR="$top/extern/boost"

rand_id=$(date --iso-8601=seconds)


cd "$BOOSTDIR" || exit 1

[ -e "$HOME/user-config.jam" ] && mv "$HOME"/user-config.jam "$HOME/$rand_id.jam.bak" 

rm -r "$top/Boost_root.d"

git clean -xfdf
git submodule foreach --recursive git clean -xffd
git reset --hard
git submodule foreach --recursive git reset --hard
git submodule update --init --recursive

echo "using gcc : arm : arm-none-eabi-g++ ;" > "$HOME/user-config.jam"

./bootstrap.sh 
./b2 optimization=space variant=release address-model=32 architecture=arm strip=on toolset=gcc-arm threading=single define=BOOST_DISABLE_THREADS=1,BOOST_SP_DISABLE_THREADS=1,BOOST_ASIO_DISABLE_THREADS=1 "-j$(nproc)" --without-thread --without-mpi --without-graph_parallel

./b2 --prefix="$top/Boost_root.d" install

[ -e "$HOME/$rand_id.jam.bak" ] && cat "$HOME/$rand_id.jam.bak" > "$HOME"/user-config.jam

! [ -e "$HOME/$rand_id.jam.bak" ] && rm "$HOME"/user-config.jam
[ -e "$HOME/$rand_id.jam.bak" ] && rm "$HOME/$rand_id.jam.bak"

cd - || exit 0



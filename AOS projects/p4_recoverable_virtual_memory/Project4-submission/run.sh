make
g++ $1 librvm.a -g -o obj
./obj
rm -rf rvm_segments
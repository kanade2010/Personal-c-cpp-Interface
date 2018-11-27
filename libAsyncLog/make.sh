rm ./Src/*.o *.so
g++ -c -fPIC ./Src/*.cpp
g++ -shared -o libAsyncLog.so *.o
rm *.o

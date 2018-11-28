rm *.o *.so *.a
g++ -c -fPIC ./Src/*.cpp
#g++ -shared -o libAsyncLog.so *.o
ar crv libAsyncLog.a *.o
rm ./*.o
g++ test.cpp  -o test.out -L. -lAsyncLog -lpthread

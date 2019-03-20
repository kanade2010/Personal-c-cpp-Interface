protoc user_message.proto --cpp_out=./
g++ *.cpp *.cc -o aaa -L. -lprotobuf

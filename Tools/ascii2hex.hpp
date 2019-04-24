class ascii2hex {
public:
  ascii2hex(char* chs, size_t len);

  const char* hex() { return ascii_; }
public:
  ~ascii2hex();

private:
  char* ascii_;
};

ascii2hex::ascii2hex(char* chs, size_t len){
  //用于接收到的串转换成要用的十六进制串返回主窗口调用
  char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8','9', 'A', 'B', 'C', 'D', 'E', 'F'};

  ascii_ = (char*)calloc( len * 3 + 1, sizeof(char) );// calloc ascii

  size_t i= 0;
  while( i < len )
  {
    size_t b = chs[i] & 0x000000ff;
    ascii_[i*2] = hex[b/16];
    ascii_[i*2+1] = hex[b%16];
    ++i;
  }
}

ascii2hex::~ascii2hex(){
  free(ascii_);
}
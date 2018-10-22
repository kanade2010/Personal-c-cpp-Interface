#include <string>

class String2Hex{
public:
	String2Hex(const std::string &str):m_size(0),m_str(str){
		assert((m_str.size()/2) <= kBufferSize);
		assert(m_str.size()%2 == 0);
		memset(m_buffer, 0 , sizeof m_buffer);
		for(int i = 0; i < m_str.size(); m_size++, i += 2)
		{
			int times = 1;
			do{
				int pos = i + 1 - times;
				if(m_str[pos] >= '0' && m_str[pos] <= '9'){
					m_buffer[m_size] += (m_str[pos] - '0') * (times == 1 ? 16 : 1);
				}
				if(m_str[pos] >= 'A' && m_str[pos] <= 'F'){
					m_buffer[m_size] += (m_str[pos] - 'A' + 10) * (times == 1 ? 16 : 1);
				}
				if(m_str[pos] >= 'a' && m_str[pos] <= 'f'){
					m_buffer[m_size] += (m_str[pos] - 'a' + 10) * (times == 1 ? 16 : 1);
				}
			}while(times--);
		}
	}
	~String2Hex(){}

	unsigned char* hex() {return m_buffer;}
	std::string& str() {return m_str;}
	size_t size() {return m_size;}

private:
	static const size_t kBufferSize = 4096;
	unsigned char m_buffer[kBufferSize];
	size_t m_size;
	std::string m_str;
};
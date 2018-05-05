#ifndef MD5_H
#define MD5_H
 
typedef struct
{
    unsigned int count[2];
    unsigned int state[4];
    unsigned char buffer[64];   
}MD5_CTX;

void MD5Init(MD5_CTX *context);
void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen);
void MD5Final(MD5_CTX *context,unsigned char digest[16]);
void MD5Transform(unsigned int state[4],unsigned char block[64]);
void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len);
void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len);

int get_md5(char* src,char* dst);
int get_md5_withlen(char* src,int len,unsigned char* dst);
int get_md5_32_withlen(char* src,int len,char* dst,int dstlen);

#include <string>
#include <stdint.h>

using std::string;
#define uint8  uint8_t
#define uint32 uint32_t

class MD5
{
private:
	struct md5_context
	{
		uint32 total[2];
		uint32 state[4];
		uint8 buffer[64];
	};

	void md5_starts(struct md5_context * ctx);
	void md5_process(struct md5_context *ctx, uint8 data[64]);
	void md5_update(struct md5_context * ctx, uint8 * input, uint32 length);
	void md5_finish(struct md5_context *ctx, uint8 digest[16]);
	uint32_t m_data[4];
public:
	MD5();
	MD5 & GenerateMD5(const string & input);
    	string toString(void);
};


#endif

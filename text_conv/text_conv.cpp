
#include <string.h>
#include <iconv.h>
#include <errno.h>
#include <string>
#include "text_conv.h"

using namespace std;

#define CONVERT_BUF_SIZE 1024

#define ER_NOTREADY "Iconv descriptor isnt opened"

void TextConv::init()
{
	fromCharset = 0;
	toCharset = 0;
	cd = (iconv_t)-1;
	ready = false;
}

void TextConv::init(const char *from, const char *to)
{
	init();
	
	fromCharset = new char[strlen(from)+1];
	strcpy(fromCharset, from);
	toCharset = new char[strlen(to)+1];
	strcpy(toCharset, to);
	
	cd = iconv_open(toCharset, fromCharset);
	if( cd == (iconv_t)-1 ) {
		throw TextConvException(errno);
	}
	else {
		ready = true;
	}
}

TextConv::~TextConv()
{
	if( fromCharset )
		delete fromCharset;
	if( toCharset )
		delete toCharset;
	if( cd != (iconv_t)-1 ) {
		iconv_close(cd);
	}
}

size_t TextConv::convert(char *dst, size_t dst_size, const char *src, size_t src_size)
{
	if( !ready )
		throw TextConvException(0, ER_NOTREADY);
	
	size_t dst_left = dst_size;
	char *ptr = (char *)src;
	size_t n = iconv(cd, &ptr, &src_size, &dst, &dst_left);
	if( n == (size_t)-1 ) {
		throw TextConvException(errno);
		return (size_t)-1;
	}
	
	return dst_size - dst_left;
}

string TextConv::convert(const char *text, size_t size)
{
	string dst;
	char buf[CONVERT_BUF_SIZE];
	
	if( !ready )
		throw TextConvException(0, ER_NOTREADY);
	
	char *ptr = (char *)text;
	while( size > 0 ) {
		size_t buf_size = CONVERT_BUF_SIZE;
		char *buf_ptr = buf;
		
		size_t n = iconv(cd, &ptr, &size, &buf_ptr, &buf_size);
		if( n == (size_t)-1 ) {
			if( errno != E2BIG ) {
				throw TextConvException(errno);
			}
		}
		
		dst.append(buf, CONVERT_BUF_SIZE-buf_size);
	}
	
	return dst;
}

string TextConv::convert(const char *text)
{
	return convert(text, strlen(text));
}

string TextConv::convert(string text)
{
	return convert(text.c_str(), text.size());
}







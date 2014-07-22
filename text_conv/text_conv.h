/* 
 * File:   text_conv.h
 * Author: alevandal
 *
 * Created on 25 Июль 2012 г., 17:47
 */

#ifndef TEXT_CONV_H
#define	TEXT_CONV_H

#include <iconv.h>
#include <string.h>
#include <errno.h>

#include <string>

class TextConvException {
protected:
	int code;
	std::string message;
public:
	TextConvException() {}
	TextConvException(int c) : code(c) { message.assign(strerror(code)); }
	TextConvException(int c, std::string m) : code(c), message(m) {}
	~TextConvException() {}
	int getCode() { return code; }
	std::string getMessage() { return message; }
};

class TextConv {
protected:
	char *fromCharset;
	char *toCharset;
	iconv_t cd;
	bool ready;
	
	void init();
	void init(const char *from, const char *to);
	
public:
	TextConv() { init(); }
	TextConv(const char *from, const char *to) { init(from, to); }
	TextConv(std::string from, std::string to) { init(from.c_str(), from.c_str()); }
	~TextConv();
	
	size_t convert(char *dst, size_t dst_size, const char *src, size_t src_size);
	std::string convert(const char *text, size_t size);
	std::string convert(const char *text);
	std::string convert(std::string text);
};

#endif	/* TEXT_CONV_H */


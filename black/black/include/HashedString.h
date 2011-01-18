#ifndef __HASHED_STRING_H__
#define __HASHED_STRING_H__

#include <iostream>

class HashedString
{
public:
	HashedString(const char * string)
		: mString(string), mHash( hash(string) )
		{}

	unsigned long getHash() const { return mHash; }
	std::string getString() const { return mString; }

private:
	std::string mString;
	unsigned long mHash;

	static unsigned long hash(const char *string);

};

#endif // __HASHED_STRING_H__

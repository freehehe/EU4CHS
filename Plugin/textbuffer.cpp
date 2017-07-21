#include "stdinc.h"
#include "textbuffer.h"

static std::ptrdiff_t GetLengthOfPreviousCodePoint(char *p)
{
	char *p2 = p;

	utf8::unchecked::prior(p);

	return p2 - p;
}

void CTextBuffer::Patch()
{

}

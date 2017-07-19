#include "textbuffer.h"
#include "../include/utf8cpp/utf8.h"

std::ptrdiff_t bytes_of_previous_code_point(char *p)
{
	char *p2 = p;
	utf8::unchecked::prior(p);

	return p2 - p;
}

void CTextBuffer::Patch()
{

}

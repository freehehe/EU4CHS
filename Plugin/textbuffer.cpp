#include "stdinc.h"
#include "textbuffer.h"

static std::ptrdiff_t GetLengthOfPreviousCodePoint(char *p)
{
	char *p2 = p;

	utf8::unchecked::prior(p);

	return p2 - p;
}

struct CTextBuffer_GetCharacterAt_0x20
{
	void operator()(injector::reg_pack &regs) const
	{
		regs.eax = utf8::unchecked::peek_next((const char *)(regs.ecx) + regs.eax);
	}
};

struct CTextBuffer_GetCharacterAt_0x29_6
{
	void operator()(injector::reg_pack &regs) const
	{
		regs.eax = utf8::unchecked::peek_next(((CTextBuffer *)(regs.esi))->field_1C()->sso_head + regs.eax);
	}
};

void CTextBuffer::Patch()
{

}

#include "textbuffer.h"

CString * CTextBuffer::GetString()
{
	return this->field<CString, 0xB4>();
}

void CTextBuffer::InitAndPatch()
{

}

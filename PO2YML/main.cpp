#include "PO2YML.h"

int main(int argc, char **argv)
{
	PO2YML object;

	if (argc > 1)
	{
		object.ConvertAll(argv[1]);
	}
	else
	{
		object.ConvertAll(R"(C:\POs)");
	}

	return 0;
}

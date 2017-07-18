#include <windows.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <map>
#include <fstream>
#include <set>
#include <iterator>
#include "../include/utf8cpp/utf8.h"

using namespace std;
using namespace utf8::unchecked;

void fuck(const char *file)
{
	vector<uint32_t> wbuffer;
	set<uint32_t> wset;

	ifstream ifs(file);

	if (!ifs)
	{
		return;
	}

	istreambuf_iterator<char> bufit(ifs);
	istreambuf_iterator<char> bufend;

	utf8to32(bufit, bufend, back_inserter(wbuffer));

	wset.insert(wbuffer.begin(), wbuffer.end());

	cout << *wset.begin();
}


int main()
{
	fuck("C:\\allin1.yml");
	return 0;
}

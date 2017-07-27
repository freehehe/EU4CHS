#include <filesystem>
#include <string>
#include <string_view>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <map>
#include <fstream>
#include <set>
#include <iterator>
#include <string_view>

using namespace std;
using namespace std::experimental;

void fuck(const char *folder)
{
	filesystem::v1::recursive_directory_iterator recur_it(folder);
	std::string_view root(folder);

	while (recur_it != filesystem::v1::recursive_directory_iterator())
	{
		if (filesystem::is_regular_file(recur_it->path()))
		{
			cout << recur_it->path().string().data() + root.length() + 1 << '\n';
		}

		++recur_it;
	}
}

int main()
{
	fuck(R"(C:\Program Files\Git)");
	return 0;
}

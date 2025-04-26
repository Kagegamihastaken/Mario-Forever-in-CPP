// Resource_Build.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>

constexpr int BLOCK_SIZE = 10000;
bool compare_num(int a) {
	if (a == 101 || a == 1001 || a == 40001) return false;
	else return true;
}
int num_not_count(int a) {
	return (a >= 101) + (a >= 1001) + (a >= 40001);
}
int main() {
	std::vector<std::pair<std::string, std::string>> res;
	std::pair<std::string, std::string> temp;
	std::set<std::string> id;
	std::string line;
	std::ifstream foi("../Mario Forever in C++/Texture.rc");
	std::cout << "Building resource.h ...\n";
	bool flag = false;
	std::string loop;
	int counter = 0;
	while (std::getline(foi, line)) {
		loop = "";
		counter = 0;
		flag = false;
		for (const auto& i : line) {
			if (i == '#') {
				flag = true;
				break;
			}
		}
		if (!flag) {
			if (line.size() == 3) continue;
			for (const auto& i : line) {
				if (i == '"' || i == '\n') break;
				if (i != ' ' && i != 0) loop += i;
				else if (i == ' ') {
					if (counter == 0) temp.first = loop;
					else if (counter == 1) {
						temp.second = loop;
						id.insert(loop);
					}
					loop = "";
					++counter;
				}
			}
			res.push_back(temp);
		}
	}
	if (BLOCK_SIZE - int(id.size() + res.size() + num_not_count(BLOCK_SIZE)) < 10) std::cout << "Error: Overflowed ID\n";
	else {
		std::ofstream foo("../Mario Forever in C++/resource.h");
		std::sort(res.begin(), res.end(), [](const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b) {
			return a.second < b.second;
			});
		int sub_c = 0;
		//init resource.h
		foo << "//{{NO_DEPENDENCIES}}\n";
		foo << "// Microsoft Visual C++ generated include file.\n";
		foo << "// Used by Mario Forever in C++.rc\n";
		foo << "\n";
		foo << "// Next default values for new objects\n";
		foo << "// \n";
		foo << "#ifdef APSTUDIO_INVOKED\n";
		foo << "#ifndef APSTUDIO_READONLY_SYMBOLS\n";
		foo << "#define _APS_NEXT_RESOURCE_VALUE        102\n";
		foo << "#define _APS_NEXT_COMMAND_VALUE         40001\n";
		foo << "#define _APS_NEXT_CONTROL_VALUE         1001\n";
		foo << "#define _APS_NEXT_SYMED_VALUE           101\n";
		foo << "#endif\n";
		foo << "#endif\n";
		foo << "\n";
		foo << "#define IDI_ICON1 101\n";
		//
		counter = 0;
		for (const auto& i : id) {
			if (!compare_num(BLOCK_SIZE - sub_c)) ++sub_c;
			foo << "#define " << i << " " << BLOCK_SIZE - sub_c << "\n";
			++sub_c;
			for (int j = counter; j < res.size(); ++j) {
				if (res[j].second != i) {
					counter = j;
					break;
				}
				if (!compare_num(BLOCK_SIZE - sub_c)) ++sub_c;
				foo << "#define " << res[j].first << " " << BLOCK_SIZE - sub_c << "\n";
				++sub_c;
			}
			foo << "\n";
		}
		std::cout << "Done!\n";
		foo.close();
	}
	foi.close();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <tuple>
#include <set>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

#define tu tuple<int, int, int>

tu LongestCommonStrand(vector<unsigned char> X, vector<unsigned char> Y);

vector<set<string>>::iterator find_home(vector<set<string>>& book, string key);

string to_res_string(string fname, int offset);
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <tuple>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include "threadpool.h"

using namespace std;

/* offset file1 | offset file2 | length | filename1 | filename2 */
#define tu tuple<int, int, int, string, string>

tu LongestCommonStrand(vector<unsigned char> X, vector<unsigned char> Y, string fname1, string fname2);

vector<set<string>>::iterator find_home(vector<set<string>>& book, string key);

string to_res_string(string fname, int offset);

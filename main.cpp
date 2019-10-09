#include "main.h"

#define NUM_FILES 10

int main() {
	string dir = "../";

	auto timer1 = std::chrono::high_resolution_clock::now();

	threadpool pool(50);

	vector<future<tu>> results;

	for (size_t i = 2; i <= NUM_FILES; i++) {
		string fname2 = "sample." + to_string(i);
		ifstream input2(dir + fname2, ios::binary);
		vector<unsigned char> Y(istreambuf_iterator<char>(input2), {});

		for (size_t j = 1; j < i; j++) {
			string fname1 = "sample." + to_string(j);
			ifstream input1(dir + fname1, ios::binary);
			vector<unsigned char> X(std::istreambuf_iterator<char>(input1), {});

			results.emplace_back(pool.commit(LongestCommonStrand, X, Y, fname1, fname2));
		}
	}

	vector<tu> cache;
	for (auto && result : results) {
		cache.push_back(result.get());
	}

	set<string> book;
	int maxLength = 0;
	for (tu t : cache) {
		string fname1 = get<3>(t), fname2 = get<4>(t);
		int offset1 = get<0>(t), offset2 = get<1>(t), len = get<2>(t);
		string key1 = to_res_string(fname1, get<0>(t));
		string key2 = to_res_string(fname2, get<1>(t));

		if (len > maxLength) {
			book.clear();
			book.insert({ key1, key2 });
			maxLength = len;
		}
		else if (len == maxLength) {
			if (book.find(key1) != book.end()) {
				book.insert(key2);
			}
			if (book.find(key2) != book.end()) {
				book.insert(key1);
			}
		}

		// function call 
		cout << fname1 << " and " << fname2 << ">>>>>>>" << endl 
			<< "OffsetX: " << offset1 << endl
			<< "OffsetY: " << offset2 << endl
			<< "Strand Length: " << len << endl;
	}

	cout << "============= Conclusion ============\n";
	cout << "Length(In-Bytes): " << maxLength << endl;
	for (string s : book) {
		cout << s << endl;
	}

	auto timer2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(timer2 - timer1).count();

	cout << "Execution Time: " << duration / 1000000.0 << " s" << endl;
	cin.get();
	return 0;
}

vector<set<string>>::iterator find_home(vector<set<string>>& book, string key) {
	return find_if(book.begin(), book.end(), [=](set<string> s) {
		return s.find(key) != s.end();
	});
}

string to_res_string(string fname, int offset) {
	return fname + " " + "offset: " + to_string(offset);
}

tu LongestCommonStrand(vector<unsigned char> X, vector<unsigned char> Y, string fname1, string fname2) {
	// Find length of both the strings. 
	int m = X.size();
	int n = Y.size();

	int result = 0;

	int endX, endY;

	vector<vector<int>> dp(2, vector<int>(n + 1, 0));

	int currRow = 0;

	for (int i = 0; i <= m; i++) {
		for (int j = 0; j <= n; j++) {
			if (i == 0 || j == 0) {
				dp[currRow][j] = 0;
			}
			else if (X[i - 1] == Y[j - 1]) {
				dp[currRow][j] = dp[1 - currRow][j - 1] + 1;
				if (dp[currRow][j] > result) {
					result = dp[currRow][j];
					endX = i - 1;
					endY = j - 1;
				}
			}
			else {
				dp[currRow][j] = 0;
			}
		}

		currRow = 1 - currRow;
	}

	// If there is no common substring, print -1. 
	if (result == 0) {
		return tu{ 0, 0, 0, fname1, fname2 };
	}

	// Thread run in paralled, characters printed could be in bad order
	//cout << "Finish thread " << fname1 << " and " << fname2 << ">>>>>>" << endl;
	//cout << "Common length: " << result << endl;
	cout << "...";

	return tu{ endX - result + 1, endY - result + 1, result, fname1, fname2 };
}
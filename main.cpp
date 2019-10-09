#include "main.h"

int main() {
	set<string> book;
	int maxLength = 0;
	for (size_t i = 2; i <= 10; i++) {
		string fname2 = "sample." + to_string(i);
		std::ifstream input2("../" + fname2, std::ios::binary);
		std::vector<unsigned char> Y(std::istreambuf_iterator<char>(input2), {});
		for (size_t j = 1; j < i; j++) {
			string fname1 = "sample." + to_string(j);
			std::ifstream input1("../" + fname1, std::ios::binary);
			std::vector<unsigned char> X(std::istreambuf_iterator<char>(input1), {});
			tu res = LongestCommonStrand(X, Y);
			string key1 = to_res_string(fname1, get<0>(res));
			string key2 = to_res_string(fname2, get<1>(res));

			if (get<2>(res) > maxLength) {
				book.clear();
				book.insert({ key1, key2 });
				maxLength = get<2>(res);
			}
			else if (get<2>(res) == maxLength) {
				if (book.find(key1) != book.end()) {
					book.insert(key2);
				}
				if (book.find(key2) != book.end()) {
					book.insert(key1);
				}
			}

			// function call 
			cout << fname1 << " and " << fname2 << ">>>>>>>" << endl 
				<< "OffsetX: " << get<0>(res) << endl
				<< "OffsetY: " << get<1>(res) << endl
				<< "Strand Length: " << get<2>(res) << endl;
		}
	}

	int maxIndex = 0;
	cout << "MaxLength: " << maxLength << endl;
	for (string s : book) {
		cout << s << endl;
	}
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

tu LongestCommonStrand(vector<unsigned char> X, vector<unsigned char> Y) {
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
		return tu{ 0, 0, 0 };
	}

	return tu{ endX - result + 1, endY - result + 1, result };
}
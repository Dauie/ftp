#include <bits/stdc++.h>

using namespace std;

int luckBalance(int n, int k, vector < vector<int> > contests) {
	int x, y = 0;
	for (int i = 0; i < n; i ++){

	}

}

int main() {
	int n;
	int k;
	cin >> n >> k;
	vector< vector<int> > contests(n,vector<int>(2));
	for(int contests_i = 0;contests_i < n;contests_i++){
		for(int contests_j = 0;contests_j < 2;contests_j++){
			cin >> contests[contests_i][contests_j];
		}
	}
	int result = luckBalance(n, k, contests);
	cout << result << endl;
	return 0;
}

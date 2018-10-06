#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(void)
{
        int test_case,num,idx;
        cin >> test_case;

        for (int i = 0; i < test_case ; i++) {
                cin >> num;
                vector <int> arr(10003,0);
                for (int j = 0; j < num; j++) {
                        cin >> idx;
                        arr[idx]++;
                }
                int rem = 0;
                int answer = 0;
                for (int j = 0; j < 10003; j++) {
                        arr[j] += rem;
                        if (arr[j] != rem)
                                rem = arr[j];
                }
                int k;
                for (int j = 0; j < 10003; j++) {
                        k = arr[10002] - arr[j -1];
                        if (arr[j] == arr[j - 1])
                                continue;
                        if (k == j && (num - k >= arr[j - 1])) {
                                answer = k;
                                break;
                        }
                }
                cout << answer << endl;
        }
        return 0;
}


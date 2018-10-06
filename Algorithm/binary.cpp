#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
using namespace std;

int main(void)
{
        int test_case,t,temp;
        cin >> test_case;

        for (int i = 0; i < test_case ; i++) {
                scanf("%d",&t);
                int mul = 1;
                int num = 0;
                int acu = 0;
                for (int j = 0; j < t; j++) {
                        mul *= 2;
                        num += mul;
                }

                vector<int> arr;
                arr.push_back(0);
                arr.push_back(0);
                for (int j = 0; j < num; j++) {
                        scanf("%d",&temp);
                        arr.push_back(temp);
                        // Accumulate origin number
                        acu += temp;
                }
                int go;
                for (int j = t; j > 0 ; j--) {
                        go = 1;
                        // find target floor
                        for (int k = 0; k < j; k++) {
                                go *= 2;
                        }
                        // calculate diff and add
                        for (int k = go; k < go*2 ; k += 2) {
                                // Add diff
                                acu += abs(arr[k] - arr[k + 1]);
                                arr[k / 2] += max(arr[k], arr[k + 1]);
                        } 
                }
                // answer is acu
                cout << acu << endl;
        }
        return 0;
}


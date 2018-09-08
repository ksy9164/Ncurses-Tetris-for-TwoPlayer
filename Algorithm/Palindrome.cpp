#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
void make_decimal(int dec, int num, vector<int>& arr)
{
        while (num != 0) {
                arr.push_back(num % dec);
                num /= dec;
        }
        return;
}

bool check_pal(vector<int>& arr)
{
        for (int i = 0; i < (int)(arr.size() / 2); ++i) {
                if (arr[i] != arr[arr.size() - 1 - i])
                        return false;
        }
        return true;
}
int main(void)
{
        int test_case,ans,num;
        cin>>test_case;

        for (int i = 0; i < test_case; ++i) {
                ans = 0;
                cin >> num;
                for (int j = 2; j <= 64; j++) {
                        vector <int> arr;
                        make_decimal(j, num, arr);
                        
                        if (check_pal(arr)) { // if it is pal
                                ans = 1; 
                                break;
                        }
                }
                cout << ans << "\n";
                
        }
        return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main(void)
{
        int test_case;
        cin >> test_case;

        for (int i = 0; i < test_case ; i++) {
                /* clock_t b,e;
                 * b = clock(); */

                int nums,idx,t_m,target,temp,min,answer,term,val;
                min = 200000003;
                t_m = -20000003;
                answer = 0;
                cin >> nums >>target;
                term = target % 2;
                int *arr = new int[nums];
                /* vector <int> arr(nums); */
                for (int j = 0; j < nums; j++) {
                        cin >> temp;
                        arr[j] = temp - target / 2;
                }
                int st,end;
                sort(arr,arr+nums);
                if (arr[nums - 1] < 0 || arr[0] >= 0) {
                        answer = 1;
                        cout << answer <<endl;
                        continue;
                }
                for (int j = 0; j < nums; j++) {
                        if (arr[j] < 0 && arr[j] > t_m) {
                                t_m = arr[j];
                                idx = j;
                        }
                        if (arr[j] > 0)
                                break;
                }
                st = idx;
                end = idx + 1;
                while (1) {
                       temp = arr[st] + arr[end] - term;
                       val = abs(temp);
                       if (val < min) {
                               min = val;
                               answer = 1;
                       } else if (val == min) {
                               answer++;
                       }
                       if (temp <= 0) {
                               if (end == nums)
                                       break;
                               end++;
                       } else {
                               if (st == 0)
                                       break;
                               st--;
                       }
                }
                cout << answer <<endl;
                /* e = clock(); */
                /* cout << ((e-b))<<endl; */
        }
        return 0;
}

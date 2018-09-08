#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main(void)
{
        int num,ans,test_case;
        cin>>test_case;
        for (int k = 0; k < test_case; k++) {
                cin>>num;
                vector <int> arr(num);

                for (int i = 0; i < num; ++i) 
                        cin>>arr[i];

                sort(arr.begin(),arr.end());

                /* for (int i = 0; i < num; ++i) 
                 *         cout<<arr[i]<<" "; */
                
                int a1,a2;
                int t1,t2,t3;

                //calculate 2 multiple number
                t1 = arr[arr.size() - 1] * arr[arr.size() - 2];
                t2 = arr[0] * arr[1];

                if (t1 < t2) {
                        a1 = t2;
                } else {
                        a1 = t1;
                }

                //calculate 3 multiple number
                t1 = arr[arr.size() - 1] * arr[arr.size() - 2] * arr[arr.size() - 3];
                t2 = arr[0] * arr[1] * arr[arr.size() - 1];

                if (t1 < t2) {
                        a2 = t2;
                } else {
                        a2 = t1;
                }

                if (a2 > a1)
                        ans = a2;
                else
                        ans = a1;
                cout<<ans<<endl;

        }
}

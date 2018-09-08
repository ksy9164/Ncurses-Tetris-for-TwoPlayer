#include <iostream>
#include <vector>
using namespace std;

int main(void)
{
        int test_case,team,match,t1,t2;
        double a,b;
        cin>>test_case;

        for (int i = 0; i < test_case; ++i) {
                cin>>team>>match;
                vector <double> good(team+1,0);
                vector <double> bad(team+1,0);
                vector <int> ans(team+1,0);
                
                //input
                for (int j = 0; j < match; j++) {
                        cin>>t1>>t2>>a>>b;
                        good[t1] += a;
                        good[t2] += b;
                        bad[t1] += b;
                        bad[t2] += a;
                }
                //calculate num
                for (int j = 1; j < team + 1; j++) {
                        double tem;

                        if (good[j] == 0 && bad[j] == 0) {
                                tem = 0;
                         } else {
                                tem = (double)(good[j] * good[j] / (good[j] * good[j] + bad[j] * bad[j]));
                                tem *= 1000;
                        }
                        
                        ans[j] = (int)(tem / 1.0);
                }
                //answer
                int min = 1000;
                int max = 0;
                for (int j = 1; j < team + 1; j++) {
                        if(ans[j] >= max)
                                max = ans[j];
                        if (ans[j] <= min)
                                min = ans[j];
                }
                cout<<max<<endl<<min<<endl;
                
        }
        
        return 0;
}

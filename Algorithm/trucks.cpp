#include<iostream>
#include<vector>
#include<fstream>

using namespace std;

int main()
{
    int i,t,n,W,L,test_case;
    ifstream in;
    in.open("input.txt");
    in>>test_case;
    for ( int q = 0; q<test_case; q++) {
        in>>n>>W>>L;
        vector <int> truck(n,0);
        for (i = 0; i<n; i++)
            in>>truck[i];
        vector <int> ans(1000003,0);
        t = 0;
        for (i = 0; i< 1000003; i++){
            if (ans[i+1] + ans[i] + truck[t] <= L){
                ans[i+1] += ans[i] + truck[t];
                ans[i+1+W] -= truck[t];
                t++;
            }else{
                ans[i+1] += ans[i];
            }
            if( t >= n )
                break;
        }

        cout<<i+W+1<<endl;
    }
}

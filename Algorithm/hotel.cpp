#include<iostream>
#include<fstream>
using namespace std;

int main()
{
    ifstream in;
    in.open("input.txt");
    int Test_case;
    int H,W,N;
    in>>Test_case;
    for(int i = 0; i<Test_case; i++){
        int h_num,w_num = 0;
        in>>H>>W>>N;
        w_num = (N-1)/H+1;
        h_num = N%H;
        if (h_num == 0)
            h_num = H;
        h_num *= 100;

        int ans = h_num + w_num;

        cout<<ans<<endl;
    }
}

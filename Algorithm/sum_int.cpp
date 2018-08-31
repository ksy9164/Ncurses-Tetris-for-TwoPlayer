#include<iostream>
#include<fstream>
using namespace std;

int main()
{
        ifstream in;
        in.open("input.txt");

        int test_case,n1,n2,ans,mul,ect;

        in>>test_case;

        for (int i = 0; i < test_case; i++) {
                in>>n1>>n2;
                mul = (n2 - n1 + 1) / 2;
                ect = (n2 - n1 + 1) % 2;
                
                if (n2 == n1)
                        ans = n1;
                else if (ect == 1)
                        ans = mul * (n2 + n1) + (n2 + n1) / 2;
                else
                        ans = mul * (n2 + n1);
                
                cout<<ans<<endl;
       } 

}

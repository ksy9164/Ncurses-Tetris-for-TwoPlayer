#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;
void make_eureka(vector<vector<int> > &arr)
{
        int st = 1;
        int num;
        //base 1
        while (1) {
                num = st*(st + 1) / 2;
                if (num > 1000)
                        break;
                arr[0][num] = 1;
                st++;
        }
        //base 2
        for (int i = 0; i < arr[0].size(); ++i) {
                if (arr[0][i] == 1) {
                        for (int j = i; j < arr[0].size(); j++) {
                                if (i + j > 1000)
                                        break;
                                if (arr[0][j] == 1)
                                        arr[1][i + j] = 1;
                        }
                }
        }
        //base 3
        for (int i = 0; i < arr[0].size(); ++i) {
                if (arr[0][i] == 1) {
                        for (int j = 0; j < arr[1].size(); j++) {
                                if (i + j > 1000)
                                        break;
                                if (arr[1][j] == 1)
                                        arr[2][i + j] = 1;
                        }
                }
        }
}
int main(void)
{
        ifstream in;
        in.open("input.txt");

        int test_case,num;
        in >> test_case;
        vector<vector<int> >arr(3,vector<int>(1001,0));
        make_eureka(arr);
        for (int i = 0; i < test_case ; i++) {
                in >> num;
                cout << arr[2][num] << endl;
        }
        return 0;
}

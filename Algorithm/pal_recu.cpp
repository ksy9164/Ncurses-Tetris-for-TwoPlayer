#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int check_pal(string &a, int first, int last)
{
        if (first >= last)
                return 1;
        if (a[first] != a[last])
                return 0;
        else
                check_pal(a , first+1, last-1);
}
int main(void)
{
        ifstream in;
        in.open("input.txt");

        int test_case;
        in >> test_case;

        for (int i = 0; i < test_case ; i++) {
                string a;
                in >> a;
                cout << check_pal(a,0,a.size() - 1) << endl;
        }
        return 0;
}

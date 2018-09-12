#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int pac(int num)
{
        if (num == 0)
                return 1;
        if (num == 1)
                return 1;
        if (num == 2)
                return 2;
        else
                return num * pac(num - 1);
}

int main(void)
{
        ifstream in;
        in.open("input.txt");

        int test_case;
        in >> test_case;

        for (int i = 0; i < test_case ; i++) {
                int num;
                in >> num;
                cout << pac(num) << "\n";
        }
        return 0;
}

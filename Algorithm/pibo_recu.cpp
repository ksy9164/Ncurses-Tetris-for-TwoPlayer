#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int pibo(int num)
{
        if (num == 0)
                return 0;
        else if (num == 1)
                return 1;
        else
                return pibo(num-1) + pibo(num-2);
}

int main(void)
{
        ifstream in;
        in.open("input.txt");

        int test_case, num;
        in >> test_case;

        for (int i = 0; i < test_case ; i++) {
                in >> num;
                cout << pibo(num) << "\n";
        }
        return 0;
}


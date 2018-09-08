#include <iostream>
using namespace std;

int main(void)
{
    int test_case,a,b,ta,tb;
    cin >> test_case;

    for (int i = 0; i < test_case; ++i) {
        cin >> a >> b;
        while (1) {
            tb = b / a;
            ta = 1;
            if (tb * a != b)
                tb++;
            // sub
            a = (tb * a ) - (b * ta);
            b = tb * b;
            // if 0 => answer
            if (a == 0)
                break;
            // make smallize
            for (int j = 2; j <= a; j++) {
                if (b % j == 0 && a % j == 0) {
                    b /= j;
                    a /= j;
                    j--;
                }
                
            }
        }
        cout<<tb<<endl;
    }
    return 0;
}

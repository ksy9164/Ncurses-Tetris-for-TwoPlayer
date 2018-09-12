#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void make_prime(vector <int> &prime)
{
        prime.push_back(2);
        prime.push_back(3);
        for (int i = 4; i < 1000; i++) {
                for (int j = 0; j < (int)prime.size(); j++) {
                        if (i % prime[j] == 0)
                                break;
                        else if (j == (int)prime.size() - 1)
                                prime.push_back(i);
                }
        }
}

int main(void)
{
        int test_case ,num, ans1, ans2,min;
        cin >> test_case;
        vector <int> prime;
        make_prime(prime);
        for (int i = 0; i < test_case ; i++) {
                cin >> num;
                min = (int)prime.size()+1;
                for (int j = 0; j < (int)prime.size(); j++) {
                        if (prime[j] > num / 2)
                                break;
                        for (int k = j; k < (int)prime.size(); k++) {
                                if (prime[j] + prime[k] > num )
                                       break;
                                else if (prime[j] + prime[k] == num) {
                                       if (min > k - j) {
                                               ans1 = j;
                                               ans2 = k;
                                       }
                                       
                               } 
                        }
                        
                }
                cout << prime[ans1] << " " << prime[ans2] << endl;
        }
        return 0;
}


#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <time.h>
using namespace std;

int main(void)
{
        unsigned long long test_case,n,m;
        cin >> test_case;
/*         clock_t begin,end;
 *
 *         begin = clock(); */
        for (unsigned long long i = 0; i < test_case ; i++) {
                cin >> n >> m;
                vector< vector<unsigned long long> > arr(n,vector<unsigned long long>(m));
                vector<unsigned long long> max_n(n);
                vector<unsigned long long> max_m(m);
                unsigned long long max,cnt,box_num;
                box_num = 0;
                cnt = 0;
                for (unsigned long long j = 0; j < n; j++) {
                        for (unsigned long long k = 0; k < m; k++) {
                                scanf("%llu",&arr[j][k]);
                                box_num += arr[j][k];
                        }
                }

                for (unsigned long long j = 0; j < n; j++) {
                        max = *max_element(arr[j].begin(),arr[j].end());
                        max_n[j] = max;
                }
                
                for (unsigned long long j = 0; j < m; j++) {
                        max = 0;
                        for (unsigned long long k = 0; k < n; k++) {
                                if (arr[k][j] > max)
                                        max = arr[k][j];
                        }
                        max_m[j] = max;
                }
                for (unsigned long long j = 0; j < n; j++) {
                        for (unsigned long long k = 0; k < m; k++) {
                                if (arr[j][k] != max_n[j] && arr[j][k] != max_m[k])
                                        cnt += arr[j][k];
                        }
                }

                cout << cnt << endl;
        }
        /* end = clock();
         * cout << end - begin << endl;
         * cout << CLOCKS_PER_SEC; */
        return 0;
}


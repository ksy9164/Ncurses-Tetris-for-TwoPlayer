#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <time.h>
using namespace std;

/* room number */
int room_num;

/* move */
int x[4] = {1,-1,0,0};
int y[4] = {0,0,1,-1};
bool desc(int a, int b)
{
        return a > b;
}
int test_case,wid,hig;

vector <int> check;
vector< vector <int> > arr;

void gogo(int yp, int xp)
{
        /* printf(" xp is %d yp is %d \n",xp,yp); */
        arr[yp][xp] = room_num;
        check[room_num]++;
        for (int i = 0; i < 4; ++i) {
                if (xp + x[i] < wid && yp + y[i] < hig && xp + x[i] >= 0 && yp + y[i] >= 0 && arr[yp + y[i]][xp + x[i]] == 0)
                        gogo(yp + y[i], xp + x[i]);
        }
}

int main(void)
{
        ifstream in;
        in.open("input.txt");
        /* clock_t be,en;
         * be = clock(); */
        char temp;
        int t;

        in >> test_case;

        for (int i = 0; i < test_case ; i++) {
                room_num = 1;
                in >> wid >> hig;
                
                check.clear();
                arr.clear();

                check.resize(100003,0);
                arr.resize(hig, vector <int> (wid,0));

                for (int j = 0; j < hig; j++) {
                        for (int k = 0; k < wid; k++) {
                                in >> temp;
                                t = (int)temp;
                                if (t == 43)
                                        t = -1;
                                else
                                        t = 0;
                                arr[j][k] = t;
                        }
                }
                
                for (int j = 0; j < hig; j++) {
                        for (int k = 0; k < wid; k++) {
                                if (arr[j][k] == 0) {
                                        gogo(j,k);
                                        room_num++;
                                }
                        }
                }

/*                 for (int j = 0; j < hig; j++) {
 *                         for (int k = 0; k < wid; k++) {
 *                                 cout << arr[j][k] << " ";
 *                         }
 *                         cout << endl;
 *                 }
 *  */
                sort(check.begin(), check.end(),desc);
                cout << room_num - 1 << endl;
                for (int j = 0; j < room_num - 1; j++) {
                        cout << check[j] << " ";
                }
                cout << endl;
        }
        /* en = clock();
         * cout << "time is " << en - be << endl; */
        return 0;
}

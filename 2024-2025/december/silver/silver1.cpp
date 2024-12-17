#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <cstring>
using namespace std;
using ll = long long;

int main() {

    int T; cin >> T;
    while (T--) {
        int N; cin >> N;
        int a[N];
        ll pre[N];
        memset(pre,0,sizeof(pre));

        cin >> a[0];
        pre[0] = a[0];
        ll total = a[0];

        for(int i=1;i<N;i++){
            cin >> a[i];
            pre[i] = a[i] + pre[i-1];
            total += a[i];
        }
        ll least_B = pre[N/2];
        for(int i=0;i<N/2-1;i++){
            least_B = min(least_B,pre[i+N/2+1]-pre[i]);
        }

        cout << least_B << " " << total - least_B << endl;

    }

    return 0;
}

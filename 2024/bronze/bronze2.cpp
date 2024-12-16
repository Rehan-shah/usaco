
#include <stdio.h>
#include <stdlib.h>

int main() {
    int N, Q;
    scanf("%d %d", &N, &Q);
    int *line_x = (int*)malloc(sizeof(int)*N*N);
    int *line_y = (int*)malloc(sizeof(int)*N*N);
    int *line_z = (int*)malloc(sizeof(int)*N*N);

    for (int i = 0; i < N*N; i++) {
        line_x[i] = N;
        line_y[i] = N;
        line_z[i] = N;
    }

    long long empty_lines = 0;

    for (int i = 0; i < Q; i++) {
        int x, y, z;
        scanf("%d %d %d", &x, &y, &z);

        int idx_x = y*N + z;
        int idx_y = x*N + z;
        int idx_z = x*N + y;

        line_x[idx_x]--;
        if (line_x[idx_x] == 0) {
            empty_lines++;
        }

        line_y[idx_y]--;
        if (line_y[idx_y] == 0) {
            empty_lines++;
        }

        line_z[idx_z]--;
        if (line_z[idx_z] == 0) {
            empty_lines++;
        }

        printf("%lld\n", empty_lines);
    }

    free(line_x);
    free(line_y);
    free(line_z);

    return 0;
}

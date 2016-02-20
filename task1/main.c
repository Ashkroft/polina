#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, const char* argv[]) {
    int n, i, j, size;
    scanf("%d", &n);
    int **table = (int **) malloc((n + 1) * sizeof(int *));
    for (i = 1; i <= n; ++i) {
        table[i] = (int *) malloc((n + 1) * sizeof(int));
        for (j = 1; j <= n; ++j) {
            table[i][j] = i * j;
        }
    }
    int x1, y1, x2, y2;
    scanf("%d", &x1);
    while (x1) {
        scanf("%d %d %d", &y1, &x2, &y2);
        size = (int)log10(table[x2][y2]) + 2;
        for (i = x1; i <= x2; ++i) {
            for (j = y1; j <= y2; ++j) {
                printf("%*d", size, table[i][j]);
            }
            printf("\n");
        }
        scanf("%d", &x1);
        printf("\n");
    }
    for (i = 1; i <= n; ++i) {
        free(table[i]);
    }
    free(table);
    return 0;
}

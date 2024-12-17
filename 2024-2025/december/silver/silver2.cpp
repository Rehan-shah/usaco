
#include <stdio.h>
#include <stdlib.h>

// Fenwicks (BIT)
typedef struct {
    int n;
    int *bit;
} Fenwicks;

Fenwicks* fenwicks_create(int n) {
    Fenwicks *f = (Fenwicks*)malloc(sizeof(Fenwicks));
    f->n = n;
    f->bit = (int*)calloc(n+1, sizeof(int));
    return f;
}

void fenwicks_update(Fenwicks *f, int i, int v) {
    while (i <= f->n) {
        f->bit[i] += v;
        i += i & (-i);
    }
}

int fenwicks_query(Fenwicks *f, int i) {
    int s = 0;
    while (i > 0) {
        s += f->bit[i];
        i -= i & (-i);
    }
    return s;
}

int fenwicks_range_query(Fenwicks *f, int l, int r) {
    if (r < l) return 0;
    return fenwicks_query(f, r) - fenwicks_query(f, l-1);
}

typedef struct {
    long long l, r;
    int t;
} Interval;

int cmp_interval(const void *a, const void *b) {
    const Interval *x = (const Interval*)a;
    const Interval *y = (const Interval*)b;
    if (x->r < y->r) return -1;
    if (x->r > y->r) return 1;
    return 0;
}

int cmp_trees(const void *a, const void *b) {
    long long x = *(long long*)a;
    long long y = *(long long*)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

// We need a binary search function for lower_bound and upper_bound
int lower_bound_long(long long *arr, int n, long long x) {
    int left = 0, right = n;
    while (left < right) {
        int mid = (left+right) >> 1;
        if (arr[mid] < x) left = mid+1; else right = mid;
    }
    return left;
}

int upper_bound_long(long long *arr, int n, long long x) {
    int left = 0, right = n;
    while (left < right) {
        int mid = (left+right) >> 1;
        if (arr[mid] <= x) left = mid+1; else right = mid;
    }
    return left;
}

// MaxHeap for long long
typedef struct {
    long long *data;
    int size;
    int capacity;
} MaxHeap;

MaxHeap* heap_create(int capacity) {
    MaxHeap* h = (MaxHeap*)malloc(sizeof(MaxHeap));
    h->data = (long long*)malloc(sizeof(long long)*capacity);
    h->size = 0;
    h->capacity = capacity;
    return h;
}

void heap_swap(long long *x, long long *y) {
    long long temp = *x;
    *x = *y;
    *y = temp;
}

void heap_push(MaxHeap *h, long long val) {
    if (h->size == h->capacity) {
        h->capacity *= 2;
        h->data = (long long*)realloc(h->data, sizeof(long long)*h->capacity);
    }
    h->data[h->size] = val;
    int idx = h->size;
    h->size++;
    while (idx > 0) {
        int parent = (idx-1)/2;
        if (h->data[parent] < h->data[idx]) {
            heap_swap(&h->data[parent], &h->data[idx]);
            idx = parent;
        } else break;
    }
}

long long heap_top(MaxHeap *h) {
    if (h->size == 0) return -1;
    return h->data[0];
}

void heap_pop(MaxHeap *h) {
    if (h->size == 0) return;
    h->data[0] = h->data[h->size-1];
    h->size--;
    int idx = 0;
    while (1) {
        int left = 2*idx+1;
        int right = 2*idx+2;
        int largest = idx;
        if (left < h->size && h->data[left] > h->data[largest]) largest = left;
        if (right < h->size && h->data[right] > h->data[largest]) largest = right;
        if (largest != idx) {
            heap_swap(&h->data[largest], &h->data[idx]);
            idx = largest;
        } else break;
    }
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        int N, K;
        scanf("%d %d", &N, &K);
        long long *trees = (long long*)malloc(sizeof(long long)*N);
        for (int i = 0; i < N; i++) {
            scanf("%lld", &trees[i]);
        }

        Interval *intervals = (Interval*)malloc(sizeof(Interval)*K);
        for (int i = 0; i < K; i++) {
            scanf("%lld %lld %d", &intervals[i].l, &intervals[i].r, &intervals[i].t);
        }

        qsort(trees, N, sizeof(long long), cmp_trees);
        qsort(intervals, K, sizeof(Interval), cmp_interval);

        Fenwicks *f = fenwicks_create(N);
        MaxHeap *heap = heap_create(N);

        int tree_idx = 0;
        int chosen_count = 0;

        for (int i = 0; i < K; i++) {
            long long L = intervals[i].l;
            long long R = intervals[i].r;
            int T_req = intervals[i].t;

            // Add all trees with x_j <= R
            while (tree_idx < N && trees[tree_idx] <= R) {
                heap_push(heap, trees[tree_idx]);
                tree_idx++;
            }

            // Count how many chosen in [L, R]
            int left_idx = lower_bound_long(trees, N, L) + 1; // 1-based
            int right_idx = upper_bound_long(trees, N, R); // still 0-based +1 for fenwicks
            // fenwicks is 1-based, so no need to add 1 to right_idx again:
            // upper_bound returns index in [0..N], so this is correct:
            // Actually, upper_bound returns count of elements <= R, so right_idx is already 1-based offset if we consider fenwicks:
            // Wait, we did +1 for left_idx. Let's do the same logic for right_idx.
            // lower_bound and upper_bound return 0-based indices.
            // For fenwicks we use 1-based indexing.
            // If lower_bound = lb (0-based), then fenwicks_index = lb+1.
            // If upper_bound = ub (0-based), there are ub elements <= R.
            // Those elements are at indices [0..ub-1].
            // So fenwicks query should go up to ub (1-based).
            int chosen_in_interval = 0;
            if (right_idx >= left_idx - 1) {
                chosen_in_interval = fenwicks_range_query(f, left_idx, right_idx);
            }

            if (chosen_in_interval < T_req) {
                int need = T_req - chosen_in_interval;
                while (need > 0 && heap->size > 0) {
                    long long top = heap_top(heap);
                    heap_pop(heap);
                    if (top < L || top > R) {
                        // not suitable
                        continue;
                    }
                    // Check if chosen
                    int pos_idx = lower_bound_long(trees, N, top) + 1;
                    int chosen_val = fenwicks_range_query(f, pos_idx, pos_idx);
                    if (chosen_val == 1) {
                        // already chosen
                        continue;
                    }
                    fenwicks_update(f, pos_idx, 1);
                    chosen_count++;
                    need--;
                }
                // If need > 0 and we can't find more, constraints can't be met.
                // Problem guarantees initial feasibility, so not handling this case.
            }
        }

        int max_cut = N - chosen_count;
        printf("%d\n", max_cut);

        free(trees);
        free(intervals);
        free(f->bit);
        free(f);
        free(heap->data);
        free(heap);
    }
    return 0;
}

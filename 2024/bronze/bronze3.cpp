
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define MAXN 20000

static inline int encode_triplet(char a, char b, char c) {
    return ((a - 'a') * 26 + (b - 'a')) * 26 + (c - 'a');
}

static inline void decode_triplet(int code, char trip[4]) {
    int c = code % 26; code /= 26;
    int b = code % 26; code /= 26;
    int a = code % 26;
    trip[0] = 'a' + a;
    trip[1] = 'a' + b;
    trip[2] = 'a' + c;
    trip[3] = '\0';
}

static int freq_table_size;
static int *freq_keys;   // store encoded key
static int *freq_values; // store frequencies
static char *freq_used;  // to mark used slots

typedef struct {
    int size;
    int capacity;
    int *data;
} IntArray;

static void ia_init(IntArray *arr) {
    arr->size = 0;
    arr->capacity = 16;
    arr->data = (int*)malloc(sizeof(int)*arr->capacity);
}

static void ia_push_back(IntArray *arr, int val) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->data = (int*)realloc(arr->data, sizeof(int)*arr->capacity);
    }
    arr->data[arr->size++] = val;
}

static int result_table_size;
static int *result_keys;
static char *result_used;

static void result_set_init(int size) {
    result_table_size = size;
    result_keys = (int*)malloc(sizeof(int)*result_table_size);
    result_used = (char*)calloc(result_table_size, 1);
}

static int result_set_insert(int key) {
    int h = key % result_table_size;
    if (h < 0) h += result_table_size;
    while (result_used[h]) {
        if (result_keys[h] == key) return 0; // already in set
        h = (h+1)%result_table_size;
    }
    result_used[h] = 1;
    result_keys[h] = key;
    return 1;
}

static void freq_map_init(int size) {
    freq_table_size = size;
    freq_keys = (int*)malloc(sizeof(int)*freq_table_size);
    freq_values = (int*)malloc(sizeof(int)*freq_table_size);
    freq_used = (char*)calloc(freq_table_size,1);
}

static void freq_map_free() {
    free(freq_keys);
    free(freq_values);
    free(freq_used);
}

static int freq_map_get(int key) {
    int h = key % freq_table_size;
    if (h < 0) h += freq_table_size;
    while (freq_used[h]) {
        if (freq_keys[h] == key) return freq_values[h];
        h = (h+1)%freq_table_size;
    }
    return 0;
}

static void freq_map_increment(int key) {
    int h = key % freq_table_size;
    if (h < 0) h += freq_table_size;
    while (freq_used[h]) {
        if (freq_keys[h] == key) {
            freq_values[h]++;
            return;
        }
        h = (h+1)%freq_table_size;
    }
    freq_used[h] = 1;
    freq_keys[h] = key;
    freq_values[h] = 1;
}

static void freq_map_decrement(int key) {
    // We'll do a search first
    int h = key % freq_table_size;
    if (h < 0) h += freq_table_size;
    while (freq_used[h]) {
        if (freq_keys[h] == key) {
            freq_values[h]--;
            return;
        }
        h = (h+1)%freq_table_size;
    }
}

static inline int is_moo(char x, char y, char z) {
    return (y == z && x != y);
}

int main(void) {
    int N, F;
    scanf("%d %d", &N, &F);
    static char s[MAXN+1];
    scanf("%s", s);

    freq_map_init(5*N+100);

    for (int i = 0; i+2 < N; i++) {
        char x = s[i], y = s[i+1], z = s[i+2];
        if (is_moo(x,y,z)) {
            int code = encode_triplet(x,y,z);
            freq_map_increment(code);
        }
    }

    result_set_init(20000);

    for (int i = 0; i < freq_table_size; i++) {
        if (freq_used[i] && freq_values[i] >= F) {
            result_set_insert(freq_keys[i]);
        }
    }

   for (int i = 0; i < N; i++) {
        char orig = s[i];
        int affected_indices[3];
        affected_indices[0] = i-2;
        affected_indices[1] = i-1;
        affected_indices[2] = i;

        int old_triplets[3];
        int old_valid[3]; // whether they were valid moos

        for (int k = 0; k < 3; k++) {
            old_valid[k] = 0;
            if (affected_indices[k] >= 0 && affected_indices[k] + 2 < N) {
                char x = s[affected_indices[k]];
                char y = s[affected_indices[k]+1];
                char z = s[affected_indices[k]+2];
                if (is_moo(x,y,z)) {
                    old_triplets[k] = encode_triplet(x,y,z);
                    old_valid[k] = 1;
                }
            }
        }

        for (int c = 0; c < ALPHABET_SIZE; c++) {
            char ch = 'a' + c;
            if (ch == orig) continue; // no change
            int new_triplets[3];
            int new_valid[3];
            for (int k = 0; k < 3; k++) {
                new_valid[k] = 0;
                int start = affected_indices[k];
                if (start >= 0 && start+2 < N) {
                    char x = (start == i ? ch : s[start]);
                    char y = (start+1 == i ? ch : s[start+1]);
                    char z = (start+2 == i ? ch : s[start+2]);
                    if (is_moo(x,y,z)) {
                        new_triplets[k] = encode_triplet(x,y,z);
                        new_valid[k] = 1;
                    }
                }
            }
            int key_change_codes[6];
            int key_change_offset[6];
            int change_count = 0;

            // Add decrements first
            for (int kk = 0; kk < 3; kk++) {
                if (old_valid[kk]) {
                    int oc = old_triplets[kk];
                    // Check if we already have oc in key_change_codes
                    int found = -1;
                    for (int jj = 0; jj < change_count; jj++) {
                        if (key_change_codes[jj] == oc) { found = jj; break; }
                    }
                    if (found == -1) {
                        key_change_codes[change_count] = oc;
                        key_change_offset[change_count] = -1;
                        change_count++;
                    } else {
                        key_change_offset[found] -= 1;
                    }
                }
            }
            // Add increments
            for (int kk = 0; kk < 3; kk++) {
                if (new_valid[kk]) {
                    int nc = new_triplets[kk];
                    // Check if we already have nc in key_change_codes
                    int found = -1;
                    for (int jj = 0; jj < change_count; jj++) {
                        if (key_change_codes[jj] == nc) { found = jj; break; }
                    }
                    if (found == -1) {
                        key_change_codes[change_count] = nc;
                        key_change_offset[change_count] = 1;
                        change_count++;
                    } else {
                        key_change_offset[found] += 1;
                    }
                }
            }

            // Now we have a list of changes:
            // Apply them hypothetically and check which of these reach >= F
            for (int jj = 0; jj < change_count; jj++) {
                int kcode = key_change_codes[jj];
                int offset = key_change_offset[jj];
                int base_freq = freq_map_get(kcode);
                int new_freq = base_freq + offset;
                if (new_freq >= F) {
                    // Insert into result set
                    result_set_insert(kcode);
                }
            }

        } // end for c
    } // end for i

    // Collect all results from result set
    IntArray results;
    ia_init(&results);
    for (int i = 0; i < result_table_size; i++) {
        if (result_used[i]) {
            ia_push_back(&results, result_keys[i]);
        }
    }

    // Sort results lexicographically
    // To do this we must decode each code and compare
    // We'll use qsort with a custom comparator
    int cmp_func(const void* a, const void* b) {
        int ca = *(int*)a;
        int cb = *(int*)b;
        char A[4], B[4];
        decode_triplet(ca, A);
        decode_triplet(cb, B);
        return strcmp(A,B);
    }

    qsort(results.data, results.size, sizeof(int), cmp_func);

    // Print output
    printf("%d\n", results.size);
    for (int i = 0; i < results.size; i++) {
        char trip[4];
        decode_triplet(results.data[i], trip);
        printf("%s\n", trip);
    }

    // Free memory
    free(results.data);
    freq_map_free();
    free(result_keys);
    free(result_used);

    return 0;
}

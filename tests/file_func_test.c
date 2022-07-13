#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/file_func.h"

char* ts[2] = {"./tests/t1.txt", "./tests/t2.txt"};

int get_proc_no_test(void) {
    FILE* f = fopen(ts[1], "r");
    assert(f);
    size_t n = get_proc_no(f);
    fclose(f);
    assert(n == 16);
    return 0;
}

int get_line_suffix_test(void) {
    char* out[2] = {
        "  11614 9 3430 873538 2114 777 142 0 0 0",
        " 1689 0 580 108933 206 50 18 0 0 0"
    };
    
    FILE* f = fopen(ts[0], "r");
    assert(f);
    char buf[256];
    for (size_t n = 0; n < 2; ++n) {
        char* nbuf = get_line_suffix(sizeof buf, buf, f);
        assert(!strcmp(nbuf, out[n]));
    }
    fclose(f);
    return 0;
}

int fill_from_line_test(void) {
    unsigned long long stat_raw[2][stat_count_];
    FILE* f = fopen(ts[0], "r");
    assert(f);
    char buf[256];
    for (size_t n = 0; n < 2; ++n) {
        char* nbuf = get_line_suffix(sizeof buf, buf, f);
        if (nbuf) {
            fill_from_line(stat_raw[n], nbuf, 10);
        }
    }
    assert(stat_raw[0][stat_idle] == 873538);
    assert(stat_raw[1][stat_user] == 1689);
    return 0;
}


int main(void) {
    printf("== Begin tests for file_func.c ==\n");
    get_line_suffix_test();
    printf("get_line_suffix passed.\n");
    get_proc_no_test();
    printf("get_proc_no passed.\n");
    fill_from_line_test();
    printf("fill_from_line passed.\n");
    printf("== All tests passed for file_func.c ==\n");
}



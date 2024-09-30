#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ./collatz N MIN MAX (min cannot be 0)

int collatz(int rn);
int lru_wrapper(int rn);
int fifo_wrapper(int rn);

int *cache = NULL;
int *priority = NULL;

int count = 1;
int cache_size = 100;
int element = 0;
float cache_attempts = 0;
float cache_hits = 0;

int main(int argc, char *argv[]) {
    if (atoi(argv[2]) == 0) {
        printf("MIN cannot be 0\n");
        return 0;
    }

    int n = atoi(argv[1]);
    int min = atoi(argv[2]);
    int max = atoi(argv[3]);

    cache = calloc(cache_size, sizeof(int));
    priority = calloc(cache_size, sizeof(int));

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        count = 1;
        int rn = rand() % (max - min + 1) + min;
        //printf("Random Number: %d, Steps: %d\n", rn, collatz(rn));
        printf("Random Number: %d, Steps: %d\n", rn, lru_wrapper(rn));
        //printf("Random Number: %d, Steps: %d\n", rn, fifo_wrapper(rn));
    }
    printf("Hitrate: %.0f / %.0f = %.4f\n", cache_hits, cache_attempts, cache_hits / cache_attempts);

    free(cache);
    free(priority);
    priority = NULL;
    cache = NULL;
    return 0;
}

int collatz(int rn) {
    count++;
    if (rn == 1)
        return count;
    if (rn % 2 == 0) {
        count++;
        //return collatz(rn / 2);
        return lru_wrapper(rn / 2);
        //return fifo_wrapper(rn / 2);
    }
    else {
        //return collatz((3 * rn) + 1);
        return lru_wrapper((3 * rn) + 1);
        //return fifo_wrapper((3 * rn) + 1);
    }
}

int lru_wrapper(int rn) {
    cache_attempts++;
    priority[cache_size] = 0;
    for (int i = 0; i < cache_size; i++) {
            priority[i + 1] = priority[i];
        }
    if (cache[rn] != 0) {
        //printf("Cache hit!\n");
        cache_hits++;
        priority[0] = cache[rn];
        return count;
    }
    if (element >= cache_size) {
        printf("Cache full, making space for %d\n", rn);
        priority[0] = collatz(rn);
        return collatz(rn);
    }
    else
        printf("No hit, caching %d..\n", rn);
        cache[rn] = collatz(rn);
        priority[0] = cache[rn];
        element++;
        return count;
}

int fifo_wrapper(int rn) {
    cache_attempts++;
    priority[cache_size] = 0;
    if (cache[rn] != 0) {
        //printf("Cache hit!\n");
        cache_hits++;
        return count;
    }
    if (element >= cache_size) {
        printf("Cache full, making space for %d\n", rn);
        for (int i = 0; i < cache_size; i++) {
            priority[i + 1] = priority[i];
        }
        priority[0] = collatz(rn);
        return collatz(rn);
    }
    else
        printf("No hit, caching %d..\n", rn);
        cache[rn] = collatz(rn);
        for (int i = 0; i < cache_size; i++) {
            priority[i + 1] = priority[i];
        }
        priority[0] = cache[rn];
        element++;
        return count;
}

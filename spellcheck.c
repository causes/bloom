/**
 * file name  : spellcheck.c
 * authors    : Dave Pederson
 * created    : Jul 20, 2011
 *
 * modifications:
 * Date:         Name:            Description:
 * ------------  ---------------  ----------------------------------------------
 * Jul 20, 2011  Dave Pederson    Creation
 */
#include <stdio.h>
#include <string.h>
#include "bloom.h"

/**
 * Strip '\r' or '\n' chars from a string
 */
static int strip(char *string)
{
    char *c;
    if ((c = strchr(string, '\r'))) {
        *c = 0;
    }
    if ((c = strchr(string, '\n'))) {
        *c = 0;
    }
}

/**
 * Ad-hoc command-line spell checker
 */
int main(int argc, char *argv[])
{
    // Open the dictionary file
    FILE *fp; 
    if (!(fp = fopen("dictionary", "r"))) {
        fprintf(stderr, "E: Couldn't open words file\n");
        fflush (stderr);
        return 1;
    }

    // Create a bloom filter
    bloom_t *filter = bloom_filter_new(2500000);

    // Add all dictionary words to the filter
    char *p;
    char line[1024];
    while (fgets(line, 1024, fp)) {
        strip(line);
        bloom_filter_add(filter, line);
    }
    fclose(fp);
    printf("bloom filter count : %u\n", bloom_filter_count(filter));
    printf("bloom filter size  : %u\n", bloom_filter_size(filter));

    // Read words from stdin and print those words not in the bloom filter
    while (fgets(line, 1024, stdin)) {
        strip(line);
        p = strtok(line, " \t,.;:\r\n?!-/()");
        while (p) {
            if (!bloom_filter_contains(filter, p)) {
                printf("%s\n", p);
            }
            p = strtok(NULL, " \t,.;:\r\n?!-/()");
        }
    }

    // Cleanup
    bloom_filter_free(filter);
    return 0;
}

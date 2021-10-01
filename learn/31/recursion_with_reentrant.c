#include <stdio.h>

/* Input : 3
 * Function needs to do below:
 *
 * 1+1+1 = 3
 * 1+1 = 2
 * 1 = 1
 * 0 = 0
 *
 * 3 + 2 + 1 = 6
 */

int add(int n)
{
    int count;
    if (!n) {
        return 0;
    }

    count = 0;
    for (int i=1; i<=n; i++) {
        count++;
    }

    count = count + add(n-1);
    return count;
}

int main(void)
{
    printf("Output : %d\n", add(3));
    return 0;
}

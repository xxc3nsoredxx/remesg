#include <ctype.h>
#include <linux/kernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/klog.h>

#define SYSLOG_ACTION_SIZE_BUFFER   10
#define SYSLOG_ACTION_READ_ALL      3

/*
 * Potentially useful manpages
 * asctime(3)
 * bootparam(7)
 * clock(3)
 * difftime(3)
 * getdate(3)
 * getrusage(2)
 * gettimeofday(2)
 * localtime(3)
 * rtc(4)
 * time(2)
 * time(3p)
 * times(2)
 *
 * Potentially useful files
 * /proc/stat
 * /proc/uptime
 */

int main () {
    int error = 0;
    int size_buf = 0;
    int size_log = 0;
    int bytes_read = 0;
    char *buf = 0;
    char *temp = 0;
    int cx = 0;

    /* Get initial size of kernel ring buffer */
    size_log = klogctl(SYSLOG_ACTION_SIZE_BUFFER, NULL, 0);
    size_buf = size_log;
    /* Allocate initial space in buffer */
    buf = calloc(size_buf + 1, sizeof(*buf));

    /* Read the kernel ring buffer */
    bytes_read = klogctl(SYSLOG_ACTION_READ_ALL, buf, size_buf);

    /* Resize */
    temp = realloc(buf, (bytes_read + 1) * sizeof(*buf));
    if (!temp) {
        goto exit;
    }
    buf = temp;
    temp = 0;
    size_buf = bytes_read;

    /* Filter non-ASCII */
    for (cx = 0; cx < size_buf; cx++) {
        if (*(buf + cx) && !(isprint(*(buf + cx)) || isspace(*(buf + cx)))) {
            *(buf + cx) = ' ';
        }
    }

    /* Print the kernel messages */
    printf("%s", buf);

exit:
    free(buf);
    return error;
}

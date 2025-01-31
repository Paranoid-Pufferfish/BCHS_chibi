#include <sys/types.h> /* size_t, ssize_t */
#include <stdarg.h> /* va_list */
#include <stdint.h> /* int64_t */
#include <stdio.h> /* sprintf */
#include <unistd.h>
#include <err.h>
#include <kcgi.h>
#include <kcgihtml.h>
#include <sqlbox.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    struct sqlbox *p;
    struct sqlbox_src srcs[] = {
        { .fname = (char *)":memory:",
          .mode = SQLBOX_SRC_RWC },
      };
    struct sqlbox_cfg cfg;
    size_t id;

    memset(&cfg, 0, sizeof(struct sqlbox_cfg));
    cfg.msg.func_short = warnx;
    cfg.srcs.srcsz = 1;
    cfg.srcs.srcs = srcs;
    if ((p = sqlbox_alloc(&cfg)) == NULL)
        errx(EXIT_FAILURE, "sqlbox_alloc");
    if (!(id = sqlbox_open(p, 0)))
        errx(EXIT_FAILURE, "sqlbox_open");

    sqlbox_free(p);
    return 0;
}

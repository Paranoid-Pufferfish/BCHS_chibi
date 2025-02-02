#include <sys/types.h> /* size_t, ssize_t */
#include <stdarg.h> /* va_list */
#include <stdint.h> /* int64_t */
#include <unistd.h> /* pledge() */
#include <err.h> /* err(), warnx() */
#include <stdlib.h> /* EXIT_FAILURE */
#include <string.h> /* memset() */
#include <sqlbox.h>


int main(void) {
    size_t dbid, stmtid;
    struct sqlbox *p;
    struct sqlbox_cfg cfg;
    struct sqlbox_src srcs[] = {
        { .fname = (char *)"db.db",
          .mode = SQLBOX_SRC_RW }
    };
    struct sqlbox_pstmt pstmts[] = {
        { .stmt = (char *)"INSERT INTO BCHS (NUM) VALUES (?)" },
      };
    struct sqlbox_parm parms[] = {
        { .iparm = 10,
          .type = SQLBOX_PARM_INT },
      };
    const struct sqlbox_parmset *res;

    memset(&cfg, 0, sizeof(struct sqlbox_cfg));
    cfg.msg.func_short = warnx;
    cfg.srcs.srcsz = 1;
    cfg.srcs.srcs = srcs;
    cfg.stmts.stmtsz = 1;
    cfg.stmts.stmts = pstmts;

    if ((p = sqlbox_alloc(&cfg)) == NULL)
        errx(EXIT_FAILURE, "sqlbox_alloc");
    if (!(dbid = sqlbox_open(p, 0)))
        errx(EXIT_FAILURE, "sqlbox_open");
    if (!(stmtid = sqlbox_prepare_bind(p, dbid, 0, 1, parms, 0)))
        errx(EXIT_FAILURE, "sqlbox_prepare_bind");
    if ((res = sqlbox_step(p, stmtid)) == NULL)
        errx(EXIT_FAILURE, "sqlbox_step");
    if (!sqlbox_finalise(p, stmtid))
        errx(EXIT_FAILURE, "sqlbox_finalise");

    sqlbox_free(p);
    return 0;
}

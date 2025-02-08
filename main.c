#include <sys/types.h> /* size_t, ssize_t */
#include <stdarg.h> /* va_list */
#include <stddef.h> /* NULL */
#include <stdint.h> /* int64_t */
#include <unistd.h> /* pledge() */
#include <err.h> /* err(), warnx() */
#include <stdlib.h> /* EXIT_FAILURE */
#include <string.h> /* memset() */
#include <kcgi.h>
#include <kcgihtml.h>
#include <sqlbox.h>

int main(void) {
    struct kreq r;
    struct kpair *p;
    struct khtmlreq req;

    size_t dbid, stmtid;
    struct sqlbox *p2;
    struct sqlbox_cfg cfg;
    struct sqlbox_src srcs[] = {
        {
            .fname = (char *) "db/db.db",
            .mode = SQLBOX_SRC_RWC
        }
    };
    struct sqlbox_pstmt pstmts[] = {
        {.stmt = (char *) "SELECT * FROM chibis"},
    };
    const struct sqlbox_parmset *res;

    memset(&cfg, 0, sizeof(struct sqlbox_cfg));
    cfg.msg.func_short = warnx;
    cfg.srcs.srcsz = 1;
    cfg.srcs.srcs = srcs;
    cfg.stmts.stmtsz = 1;
    cfg.stmts.stmts = pstmts;

    if (pledge("stdio rpath cpath "
               "wpath flock proc fattr", NULL) == -1)
        err(EXIT_FAILURE, "pledge");
    if ((p2 = sqlbox_alloc(&cfg)) == NULL)
        errx(EXIT_FAILURE, "sqlbox_alloc");
    if (pledge("stdio proc", NULL) == -1)
        err(EXIT_FAILURE, "pledge");
    if (!(dbid = sqlbox_open(p2, 0)))
        errx(EXIT_FAILURE, "sqlbox_open");
    if (!(stmtid = sqlbox_prepare_bind(p2, dbid, 0, 0, 0, 0)))
        errx(EXIT_FAILURE, "sqlbox_prepare_bind");
    if ((res = sqlbox_step(p2, stmtid)) == NULL)
        errx(EXIT_FAILURE, "sqlbox_step");

    enum khttp er = KHTTP_200;
    if (pledge("stdio proc", NULL) == -1)
        err(EXIT_FAILURE, "pledge");
    if (khttp_parse(&r, 0,0,0,0,0) != KCGI_OK)
        return 1;
    if (pledge("stdio", NULL) == -1)
        err(EXIT_FAILURE, "pledge");
    if (r.mime != KMIME_TEXT_HTML)
        er = KHTTP_404;
    khttp_head(&r, kresps[KRESP_STATUS],
               "%s", khttps[er]);
    khttp_head(&r, kresps[KRESP_CONTENT_TYPE],
               "%s", kmimetypes[KMIME_TEXT_HTML]);
    khttp_body(&r);
    khtml_open(&req, &r, 0);
    kcgi_writer_disable(&r);
    for (int i = 0; i < res->psz; ++i) {
        switch (res->ps[i].type) {
            case SQLBOX_PARM_BLOB:
                khtml_printf(&req, "Blob: %zu bytes\n", res->ps[i].sz);
            khtml_elem(&req, KELEM_BR);
            break;
            case SQLBOX_PARM_FLOAT:
                khtml_printf(&req, "Float: %f\n", res->ps[i].fparm);
            khtml_elem(&req, KELEM_BR);
            break;
            case SQLBOX_PARM_INT:
                khtml_printf(&req, "Blob: %lld\n", res->ps[i].iparm);
            khtml_elem(&req, KELEM_BR);
            break;
            case SQLBOX_PARM_NULL:
                khtml_printf(&req, "Null\n");
            khtml_elem(&req, KELEM_BR);
            break;
            case SQLBOX_PARM_STRING:
                khtml_printf(&req, "String: %s\n", res->ps[i].sparm);
            khtml_elem(&req, KELEM_BR);
            break;
        }
    }

        if (!sqlbox_finalise(p2, stmtid))
            errx(EXIT_FAILURE, "sqlbox_finalise");
        sqlbox_free(p2);
        khtml_close(&req);
        khttp_free(&r);
        return 0;
    }

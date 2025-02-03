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
    struct khtmlreq req;

    // size_t dbid, stmtid;
    // struct sqlbox *p;
    // struct sqlbox_cfg cfg;
    // struct sqlbox_src srcs[] = {
    //     { .fname = (char *)"db/db.db",
    //       .mode = SQLBOX_SRC_RWC }
    // };
    // struct sqlbox_pstmt pstmts[] = {
    //     { .stmt = (char *)"INSERT INTO BCHS (NUM) VALUES (?)" },
    //   };
    // struct sqlbox_parm parms[] = {
    //     { .iparm = 10,
    //       .type = SQLBOX_PARM_INT },
    //   };
    // const struct sqlbox_parmset *res;
    //
    // memset(&cfg, 0, sizeof(struct sqlbox_cfg));
    // cfg.msg.func_short = warnx;
    // cfg.srcs.srcsz = 1;
    // cfg.srcs.srcs = srcs;
    // cfg.stmts.stmtsz = 1;
    // cfg.stmts.stmts = pstmts;
    //
    // if (pledge("stdio rpath cpath "
    //     "wpath flock proc fattr", NULL) == -1)
    //     err(EXIT_FAILURE, "pledge");
    // if ((p = sqlbox_alloc(&cfg)) == NULL)
    //     errx(EXIT_FAILURE, "sqlbox_alloc");
    // if (pledge("stdio proc", NULL) == -1)
    //     err(EXIT_FAILURE, "pledge");
    // if (!(dbid = sqlbox_open(p, 0)))
    //     errx(EXIT_FAILURE, "sqlbox_open");
    // if (!(stmtid = sqlbox_prepare_bind(p, dbid, 0, 1, parms, 0)))
    //     errx(EXIT_FAILURE, "sqlbox_prepare_bind");
    // if ((res = sqlbox_step(p, stmtid)) == NULL)
    //     errx(EXIT_FAILURE, "sqlbox_step");
    // if (!sqlbox_finalise(p, stmtid))
    //     errx(EXIT_FAILURE, "sqlbox_finalise");
    // sqlbox_free(p);

    if (pledge("stdio proc", NULL) == -1)
        err(EXIT_FAILURE, "pledge");
    if (khttp_parse(&r, 0, 0, 0, 0, 0) != KCGI_OK)
        return 1;
    if (pledge("stdio", NULL) == -1)
        err(EXIT_FAILURE, "pledge");
    khttp_head(&r, kresps[KRESP_STATUS],
               "%s", khttps[KHTTP_200]);
    khttp_head(&r, kresps[KRESP_CONTENT_TYPE],
               "%s", kmimetypes[r.mime]);
    khttp_body(&r);
    khtml_open(&req, &r, 0);
    kcgi_writer_disable(&r);
    khtml_printf(&req, "path: %s", r.path);
    khtml_elem(&req, KELEM_BR);
    khtml_printf(&req, "suffix: %s", r.suffix);
    khtml_elem(&req, KELEM_BR);
    khtml_printf(&req, "fullpath: %s", r.fullpath);
    khtml_elem(&req, KELEM_BR);
    khtml_printf(&req, "pagename: %s", r.pagename);
    khtml_elem(&req, KELEM_BR);
    khtml_printf(&req, "host: %s", r.host);
    khtml_elem(&req, KELEM_BR);
    khtml_printf(&req, "port: %d", r.port);
    khtml_elem(&req, KELEM_BR);
    khtml_printf(&req, "mime: %s", kmimetypes[r.mime]);
    khtml_elem(&req, KELEM_BR);
    struct khead *h;
    for (int i = 0; i < KREQU__MAX; i++) {
        if ((h = r.reqmap[i]) != NULL) {
            khtml_printf(&req, "%s: %s", h->key, h->val);
            khtml_elem(&req, KELEM_BR);
        }
    }
    khtml_close(&req);
    khttp_free(&r);
    return 0;
}

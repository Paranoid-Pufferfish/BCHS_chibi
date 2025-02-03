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


enum key {
    KEY_VAL,
    KEY__MAX
};


static const struct kvalid keys[KEY__MAX] = {
    {kvalid_int, "val"}, /* KEY_INTEGER */
};


enum page {
    PAGE_INDEX,
    PAGE_ADD,
    PAGE_GET,
    PAGE__MAX
};

const char *const pages[PAGE__MAX] = {
    "index",
    "add",
    "get"
};


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
    enum khttp er = KHTTP_200;
    if (pledge("stdio proc", NULL) == -1)
        err(EXIT_FAILURE, "pledge");
    if (khttp_parse(&r, keys, KEY__MAX, pages, PAGE__MAX, PAGE_INDEX) != KCGI_OK)
        return 1;
    if (pledge("stdio", NULL) == -1)
        err(EXIT_FAILURE, "pledge");
    if (r.mime != KMIME_TEXT_HTML)
        er = KHTTP_404;
    if (r.method != KMETHOD_GET)
        er = KHTTP_405;
    khttp_head(&r, kresps[KRESP_STATUS],
               "%s", khttps[er]);
    khttp_head(&r, kresps[KRESP_CONTENT_TYPE],
               "%s", kmimetypes[KMIME_TEXT_HTML]);
    khttp_body(&r);
    khtml_open(&req, &r, 0);
    kcgi_writer_disable(&r);
    khtml_elem(&req, KELEM_H1);
    switch (r.page) {
        case PAGE_INDEX: khtml_printf(&req, "You are in INDEX");
            break;
        case PAGE_ADD: khtml_printf(&req, "You are in ADD");
            break;
        case PAGE_GET: khtml_printf(&req, "You are in GET");
            break;
        default: khtml_printf(&req, "Probably Elsewhere");
            break;
    }
    khtml_closeelem(&req, 0);
    khtml_close(&req);
    khttp_free(&r);
    return 0;
}

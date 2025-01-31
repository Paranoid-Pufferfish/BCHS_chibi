#include <sys/types.h> /* size_t, ssize_t */
#include <stdarg.h> /* va_list */
#include <stdint.h> /* int64_t */
#include <stdio.h> /* sprintf */
#include <kcgi.h>
#include <kcgihtml.h>

int main(void) {
    struct kreq r;
    struct khtmlreq req;
    if (khttp_parse(&r, 0, 0, 0, 0, 0) != KCGI_OK)
        return 1;

    khttp_head(&r, kresps[KRESP_STATUS],
        "%s", khttps[KHTTP_200]);
    khttp_head(&r, kresps[KRESP_CONTENT_TYPE],
        "%s", kmimetypes[r.mime]);
    khttp_body(&r);
    khtml_open(&req,&r,0);
    kcgi_writer_disable(&r);
    khtml_printf(&req, "path: %s",r.path);
    khtml_elem(&req, KELEM_BR);
    khtml_printf(&req,"suffix: %s",r.suffix);
    khtml_elem(&req, KELEM_BR);
    khtml_printf(&req,"fullpath: %s",r.fullpath);
    khtml_elem(&req, KELEM_BR);
    khtml_printf(&req,"pagename: %s",r.pagename);
    khtml_elem(&req, KELEM_BR);
    khtml_printf(&req,"host: %s",r.host);
    khtml_elem(&req, KELEM_BR);
    khtml_printf(&req,"port: %d",r.port);
    khtml_elem(&req, KELEM_BR);
    khtml_printf(&req,"mime: %s",kmimetypes[r.mime]);
    khtml_elem(&req, KELEM_BR);
    struct khead *h;
    for(int i = 0; i < KREQU__MAX; i++){
        if((h = r.reqmap[i]) != NULL){
            khtml_printf(&req,"%s: %s",h->key,h->val);
            khtml_elem(&req, KELEM_BR);
        }
    }
    khtml_close(&req);
    khttp_free(&r);
    return 0;
}
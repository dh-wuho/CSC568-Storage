#include "hdfs_fuse.h"

void *hdfs_init(struct fuse_conn_info *conn, struct fuse_config *cfg) {
    hdfs_context *ctx = (hdfs_context *) fuse_get_context()->private_data;

    return (void *) ctx;
}

#include "hdfs_fuse.h"

void hdfs_destroy(void *private_data) {
    hdfs_context *ctx = (hdfs_context *) private_data;

    hdfsDisconnect(ctx->fs);
}

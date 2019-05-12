#include "hdfs_fuse.h"

int hdfs_rename(const char *from, const char *to, unsigned int flags) {
    hdfs_context *ctx = (hdfs_context *) fuse_get_context()->private_data;
    if (hdfsMove(ctx->fs, from, ctx->fs, to) != 0) {
        return EIO;
    }
    return 0;
}

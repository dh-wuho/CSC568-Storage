#include "hdfs_fuse.h"

int hdfs_unlink(const char *path) {
    hdfs_context *ctx = (hdfs_context *) fuse_get_context()->private_data;
    if (hdfsDelete(ctx->fs, path, 0) != 0) {
        return EIO;
    }
    return 0;
}

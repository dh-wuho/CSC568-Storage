#include "hdfs_fuse.h"

int hdfs_rmdir(const char *path) {
    hdfs_context *ctx = (hdfs_context *) fuse_get_context()->private_data;
    if (hdfsDelete(ctx->fs, path, 1) != 0) {
        return EIO;
    }

    return 0;
}

#include "hdfs_fuse.h"

int hdfs_mkdir(const char *path, mode_t mode) {
    hdfs_context *ctx = (hdfs_context *) fuse_get_context()->private_data;

    if (hdfsCreateDirectory(ctx->fs, path) != 0) {
        return EIO;
    }

    return 0;
}

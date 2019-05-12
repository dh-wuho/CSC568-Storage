#include "hdfs_fuse.h"

int hdfs_chmod(const char *path, mode_t mode, struct fuse_file_info *fi) {

    hdfs_context *ctx = (hdfs_context *) fuse_get_context()->private_data;

    if(hdfsChmod(ctx->fs, path, mode) != 0) {
        return -EACCES;
    }

    return 0;
}

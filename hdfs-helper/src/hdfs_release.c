#include "hdfs_fuse.h"

int hdfs_release(const char *path, struct fuse_file_info *fi) {
    hdfs_context *ctx = (hdfs_context *) fuse_get_context()->private_data;
    hdfs_file_handle *fh = (hdfs_file_handle *) fi->fh;
    int ret = 0;

    if (hdfsCloseFile(ctx->fs, fh->file) != 0) {
        ret = -EIO;
    }

    pthread_mutex_destroy(&fh->mutex);
    fi->fh = 0;

    return ret;
}

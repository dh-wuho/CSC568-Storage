#include "hdfs_fuse.h"

int hdfs_statfs(const char *path, struct statvfs * st) {
    hdfs_context *ctx = (hdfs_context *) fuse_get_context()->private_data;

    tOffset capacity = hdfsGetCapacity(ctx->fs);
    tOffset used = hdfsGetUsed(ctx->fs);
    tOffset blockSize = hdfsGetDefaultBlockSize(ctx->fs);

    st->f_bsize = (unsigned long) blockSize;
    st->f_frsize = (unsigned long) blockSize;
    st->f_blocks = (fsblkcnt_t) capacity / blockSize;
    st->f_bfree = (fsblkcnt_t) (capacity - used) / blockSize;
    st->f_bavail = (fsblkcnt_t) (capacity - used) / blockSize;

    return 0;
}

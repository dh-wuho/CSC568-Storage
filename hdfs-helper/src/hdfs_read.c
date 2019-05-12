#include "hdfs_fuse.h"

int hdfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    hdfs_context *ctx = (hdfs_context *) fuse_get_context()->private_data;
    hdfs_file_handle *fh = (hdfs_file_handle *) fi->fh;

    // if the caller needs 0 byte, it is not necessary to go interact with hdfs
    if (size == 0) {
        return 0;
    }

    int readSize = hdfsPread(ctx->fs, fh->file, offset, buf, size);

    if (readSize < 0) {
        return -EIO;
    }

    return readSize;
}

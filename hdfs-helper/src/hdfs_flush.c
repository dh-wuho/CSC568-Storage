#include "hdfs_fuse.h"

int hdfs_flush(const char *path, struct fuse_file_info *fi) {
    hdfs_context *ctx = (hdfs_context *) fuse_get_context()->private_data;
    hdfs_file_handle *fh = (hdfs_file_handle *) fi->fh;
    
    if (fi->flags & O_WRONLY) {
        if (hdfsFlush(ctx->fs, fh->file) != 0) {
            return -EIO;
        }
    }
    return 0;
}

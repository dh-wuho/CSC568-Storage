#include "hdfs_fuse.h"

int hdfs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    hdfs_context *ctx = (hdfs_context *) fuse_get_context()->private_data;
    hdfs_file_handle *fh = (hdfs_file_handle *) fi->fh;
    tOffset cur_offset = hdfsTell(ctx->fs, fh->file);
    tSize length = 0;
    int ret = 0;
    
    pthread_mutex_lock(&fh->mutex);

    if (cur_offset != offset) {
        ret = -ENOTSUP;
    } else {
        length = hdfsWrite(ctx->fs, fh->file, buf, size);
        if (length <= 0) {
            ret = -EIO;
        }
    } 

    pthread_mutex_unlock(&fh->mutex);

    return ret == 0 ? length : ret;
}

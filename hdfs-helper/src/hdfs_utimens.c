#include "hdfs_fuse.h"

int hdfs_utimens(const char *path, const struct timespec tv[2], struct fuse_file_info *fi) {

    hdfs_context *ctx = (hdfs_context *) fuse_get_context()->private_data;
    
    if(hdfsUtime(ctx->fs, path, tv[1].tv_sec, tv[0].tv_sec) != 0) {
        return -EACCES;
    }
    return 0;
}

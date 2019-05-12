#include "hdfs_fuse.h"
#include <stdlib.h>

int hdfs_open(const char *path, struct fuse_file_info *fi) {
    hdfs_context *ctx = (hdfs_context *) fuse_get_context()->private_data;
    hdfs_file_handle *fh = NULL;
    hdfsFileInfo *info;
    struct hdfsStreamBuilder *bld;
    int flags;

    if ((fi->flags & O_ACCMODE) == O_RDONLY) {
        flags = O_RDONLY;
    } else if (fi->flags & O_TRUNC) {
        flags = O_WRONLY;
    } else if ((info = hdfsGetPathInfo(ctx->fs, path))) {
        if (info->mSize == 0) {
            flags = O_WRONLY;
        } else if ((fi->flags & O_ACCMODE) == O_RDWR) {
            flags = O_RDONLY;
        } else {
            flags = O_WRONLY | O_APPEND;
        }
        hdfsFreeFileInfo(info, 1);
    } else {
        if (fi->flags & O_CREAT) {
            flags = O_WRONLY;
        } else {
            return -ENOENT;
        }
    }

    fh = (hdfs_file_handle *) malloc(sizeof(hdfs_file_handle));

    if ((bld = hdfsStreamBuilderAlloc(ctx->fs, path, flags)) == NULL) {
        free(fh);
        return -EIO;
    }

    if (pthread_mutex_init(&fh->mutex, NULL) != 0) {
        free(fh);
        hdfsStreamBuilderFree(bld);
        return -EIO;
    }

    fh->file = hdfsStreamBuilderBuild(bld);
    fi->fh = (uint64_t) fh;

    return 0;
}

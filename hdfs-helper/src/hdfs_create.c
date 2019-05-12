#include "hdfs_fuse.h"

int hdfs_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    fi->flags |= mode;
    return hdfs_open(path, fi);
}

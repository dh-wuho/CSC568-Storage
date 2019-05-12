#include "hdfs_fuse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    hdfs_context ctx;
    struct hdfsBuilder *bld = hdfsNewBuilder();
    struct fuse_operations hdfs_op = {
        .getattr = hdfs_getattr,
        .readlink = hdfs_readlink,
        .mknod = hdfs_mknod,
        .mkdir = hdfs_mkdir,
        .unlink = hdfs_unlink,
        .rmdir = hdfs_rmdir,
        .symlink = hdfs_symlink,
        .rename = hdfs_rename,
        .link = hdfs_link,
        .chmod = hdfs_chmod,
        .chown = hdfs_chown,
        .truncate = hdfs_truncate,
        .open = hdfs_open,
        .read = hdfs_read,
        .write = hdfs_write,
        .statfs = hdfs_statfs,
        .flush = hdfs_flush,
        .release = hdfs_release,
        .fsync = hdfs_fsync,
        .setxattr = hdfs_setxattr,
        .getxattr = hdfs_getxattr,
        .listxattr = hdfs_listxattr,
        .removexattr = hdfs_removexattr,
        .opendir = hdfs_opendir,
        .readdir = hdfs_readdir,
        .releasedir = hdfs_releasedir,
        .fsyncdir = hdfs_fsyncdir,
        .init = hdfs_init,
        .destroy = hdfs_destroy,
        .access = hdfs_access,
        .create = hdfs_create,
        .lock = hdfs_lock,
        .utimens = hdfs_utimens,
        .bmap = hdfs_bmap,
        .ioctl = hdfs_ioctl,
        .poll = hdfs_poll,
        //.write_buf = hdfs_write_buf,
        .read_buf = hdfs_read_buf,
        .flock = hdfs_flock,
        .fallocate = hdfs_fallocate,
        .copy_file_range = hdfs_copy_file_range,
    };

    if (argc < 3 || strncmp(argv[1], "hdfs://", 7) != 0) {
        printf("Usage: hdfs-mount hdfs://<hostname>:<port> <mountpoint>\n");
        return 1;
    }

    hdfsBuilderSetNameNode(bld, argv[1]);

    if ((ctx.fs = hdfsBuilderConnect(bld)) == NULL) {
        printf("Cannot connect to %s\n", argv[1]);
    }

    memmove(argv + 1, argv + 2, (argc - 2) * sizeof(char *));

    return fuse_main(argc - 1, argv, &hdfs_op, &ctx);
}

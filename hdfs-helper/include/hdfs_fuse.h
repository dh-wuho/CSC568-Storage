#ifndef HDFS_FUSE_H
#define HDFS_FUSE_H

#define FUSE_USE_VERSION 34

#include <fuse3/fuse.h>
#include <hdfs.h>
#include <pthread.h>

typedef struct hdfs_context {
    hdfsFS fs;
} hdfs_context;

typedef struct hdfs_file_handle {
    hdfsFile file;
    pthread_mutex_t mutex;
} hdfs_file_handle;

int hdfs_getattr(const char *path, struct stat *st, struct fuse_file_info *fi);
int hdfs_readlink(const char *path, char *buf, size_t size);
int hdfs_mknod(const char *path, mode_t mode, dev_t dev);
int hdfs_mkdir(const char *path, mode_t mode);
int hdfs_unlink(const char *path);
int hdfs_rmdir(const char *path);
int hdfs_symlink(const char *from, const char *to);
int hdfs_rename(const char *from, const char *to, unsigned int flags);
int hdfs_link(const char *from, const char *to);
int hdfs_chmod(const char *path, mode_t mode, struct fuse_file_info *fi);
int hdfs_chown(const char *path, uid_t uid, gid_t gid, struct fuse_file_info *fi);
int hdfs_truncate(const char *path, off_t size, struct fuse_file_info *fi);
int hdfs_open(const char *path, struct fuse_file_info *fi);
int hdfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
int hdfs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
int hdfs_statfs(const char *path, struct statvfs * st);
int hdfs_flush(const char *path, struct fuse_file_info *fi);
int hdfs_release(const char *path, struct fuse_file_info *fi);
int hdfs_fsync(const char *path, int isdatasync, struct fuse_file_info *fi);
int hdfs_setxattr(const char *path, const char *name, const char *value, size_t size, int flags);
int hdfs_getxattr(const char *path, const char *name, char *value, size_t size);
int hdfs_listxattr(const char *path, char *list, size_t size);
int hdfs_removexattr(const char *path, const char *name);
int hdfs_opendir(const char *path, struct fuse_file_info *fi);
int hdfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags);
int hdfs_releasedir(const char *path, struct fuse_file_info *fi);
int hdfs_fsyncdir(const char *path, int datasync, struct fuse_file_info *fi);
void *hdfs_init(struct fuse_conn_info *conn, struct fuse_config *cfg);
void hdfs_destroy(void *private_data);
int hdfs_access(const char *path, int mask);
int hdfs_create(const char *path, mode_t mode, struct fuse_file_info *fi);
int hdfs_lock(const char *path, struct fuse_file_info *fi, int cmd, struct flock *lock);
int hdfs_utimens(const char *path, const struct timespec tv[2], struct fuse_file_info *fi);
int hdfs_bmap(const char *path, size_t blocksize, uint64_t *idx);
int hdfs_ioctl(const char *path, int cmd, void *arg, struct fuse_file_info *fi, unsigned int flags, void *data);
int hdfs_poll(const char *path, struct fuse_file_info *fi, struct fuse_pollhandle *ph, unsigned *reventsp);
int hdfs_write_buf(const char *path, struct fuse_bufvec *buf, off_t off, struct fuse_file_info *fi);
int hdfs_read_buf(const char *path, struct fuse_bufvec **bufp, size_t size, off_t off, struct fuse_file_info *fi);
int hdfs_flock(const char *path, struct fuse_file_info *fi, int op);
int hdfs_fallocate(const char *path, int mode, off_t offset, off_t length, struct fuse_file_info *fi);
ssize_t hdfs_copy_file_range(const char *path_in, struct fuse_file_info *fi_in, off_t off_in, const char *path_out, struct fuse_file_info *fi_out, off_t off_out, size_t len, int flags);

#endif

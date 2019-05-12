#include "hdfs_fuse.h"
#include <grp.h>
#include <math.h>
#include <pwd.h>
#include <string.h>

int hdfs_getattr(const char *path, struct stat *st, struct fuse_file_info *fi) {
    hdfs_context *ctx = (hdfs_context *) fuse_get_context()->private_data;
    hdfsFileInfo *info;
    hdfsFileInfo *entries;
    int numEntries;
    struct passwd *u;
    struct group *g;
    int i;

    if ((info = hdfsGetPathInfo(ctx->fs, path)) == NULL) {
        return -ENOENT;
    }

    memset(st, 0, sizeof(struct stat));

    if (info->mKind == kObjectKindFile) {
        st->st_size = info->mSize;
        st->st_mode = S_IFREG;
        st->st_nlink = 1;
    } else {
        st->st_mode = S_IFDIR;
        st->st_size = 4096;
        st->st_nlink = 2;
        if ((entries = hdfsListDirectory(ctx->fs, path, &numEntries))) {
            for (i = 0; i < numEntries; i++) {
                if (entries[i].mKind == kObjectKindDirectory) {
                    st->st_nlink++;
                }
            }
            hdfsFreeFileInfo(entries, numEntries);
        }
    }

    st->st_mode |= info->mPermissions > 0 ? info->mPermissions : fuse_get_context()->umask;
    st->st_uid = fuse_get_context()->uid;
    st->st_gid = fuse_get_context()->gid;
    st->st_blksize = 512;
    st->st_blocks = (blkcnt_t) ceil((double) st->st_size / st->st_blksize);
    st->st_ctim.tv_sec = info->mLastMod;
    st->st_mtim.tv_sec = info->mLastMod;
    st->st_atim.tv_sec = info->mLastAccess;

    if (info->mOwner) {
        u = getpwnam(info->mOwner);
        st->st_uid = u? u->pw_uid : st->st_uid;
    }

    if (info->mGroup) {
        g = getgrnam(info->mGroup);
        st->st_gid = g? g->gr_gid : st->st_gid;
    }

    hdfsFreeFileInfo(info, 1);

    return 0;
}

#include "hdfs_fuse.h"
#include <stdlib.h>
#include <string.h>

int hdfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags) {
    hdfs_context *ctx = (hdfs_context *) fuse_get_context()->private_data;
    hdfsFileInfo *info;
    hdfsFileInfo *entries;
    int numEntries;
    int i;

    if ((info = hdfsGetPathInfo(ctx->fs, path)) == NULL) {
        return -ENOENT;
    }

    if (info->mKind == kObjectKindFile) {
        return -ENOENT;
    } else {
        filler(buf, ".", NULL, 0, 0);
        filler(buf, "..", NULL, 0, 0);
        if ((entries = hdfsListDirectory(ctx->fs, path, &numEntries))) {
            for (i = 0; i < numEntries; i++) {
                char *relPath = strrchr(entries[i].mName, '/');
                filler(buf, relPath+1, NULL, 0, 0);
            }
            hdfsFreeFileInfo(entries, numEntries);
        }
    }

    hdfsFreeFileInfo(info, 1);
    return 0;
}

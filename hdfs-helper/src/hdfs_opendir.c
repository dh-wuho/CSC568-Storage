#include "hdfs_fuse.h"

int hdfs_opendir(const char *path, struct fuse_file_info *fi) {
    
    // TODO
    // Not sure if hdfsListDirectory will check permission itself.
    
    int nEntries;

    hdfs_context *ctx = (hdfs_context *) fuse_get_context()->private_data;
    hdfsFileInfo *info = hdfsListDirectory(ctx->fs, path, &nEntries);

    if(info == NULL) {
        return -ENOENT; 
    }

    fi = (struct fuse_file_info*) info;
    hdfsFreeFileInfo(info, nEntries);
    return 0;
}
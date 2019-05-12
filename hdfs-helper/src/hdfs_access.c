#include "hdfs_fuse.h"
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

int hdfs_access(const char *path, int mask) {

    // mask = F_OK | R_OK | W_OK | X_OK
    hdfs_context *ctx = (hdfs_context *) fuse_get_context()->private_data;
    hdfsFileInfo *info;

    if ((info = hdfsGetPathInfo(ctx->fs, path)) == NULL) {
        hdfsFreeFileInfo(info, 1);
        return -ENOENT;
    }

    // if F_OK, return OK.
    if(mask == 0) {
        hdfsFreeFileInfo(info, 1);
        return 0;
    }
    
    struct passwd* pwd;
    struct group* grp;
    
    pwd = getpwuid(getuid());
    
    if(strcmp(pwd->pw_name, info->mOwner) == 0) {
        int ownerPermission = info->mPermissions >> 6;
        if(ownerPermission & mask) {
            hdfsFreeFileInfo(info, 1);
            return 0;
        }
        else {
            hdfsFreeFileInfo(info, 1);
            return -EACCES;
        }
    }

    grp = getgrgid(getgid());

    if(strcmp(grp->gr_name, info->mGroup) == 0) {
        int groupPermission = (info->mPermissions >> 3) % 8;
        if(groupPermission & mask) {
            hdfsFreeFileInfo(info, 1);
            return 0;
        }
        else {
            hdfsFreeFileInfo(info, 1);
            return -EACCES;
        }
    }
    
    int otherPermission = info->mPermissions % 8;

    if(otherPermission & mask) {
        hdfsFreeFileInfo(info, 1);
        return 0;
    } 

    hdfsFreeFileInfo(info, 1);
    return -EACCES;
}

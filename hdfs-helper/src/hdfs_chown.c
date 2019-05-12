#include "hdfs_fuse.h"
#include <pwd.h>
#include <grp.h>

int hdfs_chown(const char *path, uid_t uid, gid_t gid, struct fuse_file_info *fi) {
    
    struct passwd* pwd;
    struct group* grp;
    
    pwd = getpwuid(uid);
    if(pwd == NULL) {
        // User doesn't exist
        return -EACCES;
    }

    if(pwd->pw_gid == gid) {
        grp = getgrgid(gid);
        hdfs_context *ctx = (hdfs_context *) fuse_get_context()->private_data;
        if(hdfsChown(ctx->fs, path, pwd->pw_name, grp->gr_name) != 0) {
            return -EIO;
        }
    }
    else {
        // User and group don't match
        return -EACCES;
    }

    return 0;
}

#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include <zip.h>

zip_t* zArchive;

/* here, getattr is like stat();
   http://man7.org/linux/man-pages/man2/fstat.2.html */
static int zipfs_getattr(const char *path, struct stat *stbuf, 
                        struct fuse_file_info *fi) {

    (void) fi;
    int res = 0;

    if(strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        stbuf->st_size = 1;
        return res;
    }

    zip_stat_t* sb = malloc(sizeof(zip_stat_t));
    zip_stat_init(sb);

    // process path without a slash, append a slash to it.
    int pathLength = strlen(path);
    char pathSlashed[pathLength + 1];
    strcpy(pathSlashed, path + 1);
    pathSlashed[pathLength - 1] = '/';
    pathSlashed[pathLength] = '\0';

    //printf("pathSlashed: %s\n", pathSlashed);

    // ZIP_FL_ENC_GUESS is 0u in zip.h;
    // if path points to a directory
    if(zip_name_locate(zArchive, pathSlashed, ZIP_FL_ENC_GUESS) != -1) {
        zip_stat(zArchive, pathSlashed, ZIP_FL_ENC_GUESS, sb);
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        stbuf->st_size = 0;
        stbuf->st_mtime = sb->mtime;
        //printf("%s, this is a directory\n", pathSlashed);
    }
    else {
        // if path points to a file
        if(zip_name_locate(zArchive, path + 1, ZIP_FL_ENC_GUESS) != -1) {
            zip_stat(zArchive, path + 1, ZIP_FL_ENC_GUESS, sb);
            stbuf->st_mode = S_IFREG | 0777;
            stbuf->st_nlink = 1;
            stbuf->st_size = sb->size;
            stbuf->st_mtime = sb->mtime;
            //printf("%s, this is a file\n", path + 1);
        }
        // invalid
        else {
            res = -ENOENT;
        }
    } 
    
    free(sb);
    return res;
}

static int zipfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			            off_t offset, struct fuse_file_info *fi,
			            enum fuse_readdir_flags flags) {

    (void) offset;
    (void) fi;

    // current directory and upper directory
    filler(buf, ".", NULL, 0, ZIP_FL_ENC_GUESS);
    filler(buf, "..", NULL, 0, ZIP_FL_ENC_GUESS);

    int i;
    for(i = 0; i < zip_get_num_entries(zArchive, ZIP_FL_ENC_GUESS); i++) {
        struct stat st;
        memset(&st, 0, sizeof(stat));
        zip_stat_t* sb = malloc(sizeof(zip_stat_t));
        zip_stat_init(sb);
        zip_stat_index(zArchive, i, ZIP_FL_ENC_GUESS, sb);
        
        int sbLength = strlen(sb->name);
        char zipfs_path[sbLength + 2];
        zipfs_path[0] = '/';
        zipfs_path[sbLength + 1] = '\0';
        strcpy(zipfs_path + 1, sb->name);
        char* dir_path = strdup(zipfs_path);
              
        //https://linux.die.net/man/3/dirname  
        if(strcmp(path, dirname(dir_path)) == 0) {
            if(zipfs_path[strlen(zipfs_path) - 1] == '/') {
                zipfs_path[strlen(zipfs_path) - 1] = '\0';
            }
            zipfs_getattr(zipfs_path, &st, fi);
            char* base_name = basename(zipfs_path);
            if(filler(buf, base_name, &st, 0, ZIP_FL_ENC_GUESS)) {
                free(sb);
                free(dir_path);
                break;
            }
        }  
        free(dir_path);
        free(sb);
    }

    return 0;
}

static int zipfs_open(const char *path, struct fuse_file_info *fi) {
    (void) fi;

    if(zip_name_locate(zArchive, path + 1, ZIP_FL_ENC_GUESS) == -1) {
        return -ENOENT;
    }

    return 0;
}

static int zipfs_read(const char *path, char *buf, size_t size, off_t offset,
		            struct fuse_file_info *fi) {
    (void) fi;

    if(zip_name_locate(zArchive, path + 1, ZIP_FL_ENC_GUESS) == -1) {
        return -ENOENT;
    }

    zip_stat_t* sb = malloc(sizeof(zip_stat_t));
    zip_stat_init(sb);
    zip_stat(zArchive, path + 1, ZIP_FL_ENC_GUESS, sb);
    zip_file_t* zip_file = zip_fopen(zArchive, path + 1, 0);
    char* zip_contents = malloc(sb->size);
    memset(zip_contents, 0, sb->size);

    if(zip_fread(zip_file, zip_contents, sb->size) == -1) {
        printf("Zip_fread failed.\n");
        return -ENOENT;
    }

    if(offset < sb->size) {
        if(offset + size > sb->size) {
            size = sb->size - offset;
        }
        memcpy(buf, zip_contents + offset, size);
    }
    else {
        size = 0;
    }

    zip_fclose(zip_file);
    free(zip_contents);
    free(sb);

    return size;
}

static int zipfs_access(const char* path, int mask) {
    (void) mask;

    return 0;
}

static int zipfs_flush(const char* path, struct fuse_file_info* fi) {
    
    return 0;
}

static void zipfs_destroy(void* private_data) {
    (void) private_data;

    zip_close(zArchive);
}

static int zipfs_write(const char* path, const char *buf, size_t size, off_t offset, struct fuse_file_info* fi){
    printf("This is a read-only filesystem. write() is not suported.\n");
    return -EROFS;
}

static int zipfs_mknod(const char* path, mode_t mode, dev_t rdev){
    printf("This is a read-only filesystem. mknod() is not suported.\n");
    return -EROFS;
}

static int zipfs_rmdir(const char *path){
    printf("This is a read-only filesystem. rmdir() is not suported.\n");
    return -EROFS;
}

static int zipfs_mkdir(const char *path, mode_t mode){
    printf("This is a read-only filesystem. mkdir() is not suported.\n");
    return -EROFS;
}

static int zipfs_unlink(const char *path){
    printf("This is a read-only filesystem. unlink() is not suported.\n");
    return -EROFS;
}

static int zipfs_symlink(const char *path, const char *lnPath){
    printf("This is a read-only filesystem. symlink() is not suported.\n");
    return -EROFS;
}
static int zipfs_rename(const char *path, const char *newPath, unsigned int i){
    printf("This is a read-only filesystem. rename() is not suported.\n");
    return -EROFS;
}


static struct fuse_operations zipfs_oper = {
	.getattr	= zipfs_getattr,
	.readdir	= zipfs_readdir,
	.open		= zipfs_open,
	.read		= zipfs_read,
    .access     = zipfs_access,
    .flush      = zipfs_flush,
    .destroy    = zipfs_destroy,
    .write      = zipfs_write,
    .mknod      = zipfs_mknod,
    .rmdir      = zipfs_rmdir,
    .mkdir      = zipfs_mkdir,
    .unlink      = zipfs_unlink,
    .symlink      = zipfs_symlink,
    .rename      = zipfs_rename,
};


int main(int argc, char *argv[]) {
    
    int err;
    zArchive = zip_open(argv[1], 0, &err);
    if(zArchive == NULL){
        printf("Can't open zip file!\n");
        printf("Error: %d\n", err);
        return 1;
    }
	
    char* fuseArgv[argc - 1];
    fuseArgv[0] = argv[0];
    for (int i = 1; i < argc - 1; i++){
        fuseArgv[i] = argv[i + 1];
    }

    return fuse_main(argc-1, fuseArgv, &zipfs_oper, NULL);
}
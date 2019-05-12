#### zipfs
A read-only filesystem, support:
```bash
    cat
    cp (from ZIP FS)
    ls
    stat
    find
    du (partially)
    file
    less
    grep
```


#### Dependency Packages
Fuse 3.4.1
libzip 4.0
```bash
sudo apt-get install libzip-dev
```

#### How to use
- complie
```bash
gcc -Wall zipfs.c -lzip -o zipfs -I /usr/include/fuse3/ -L /lib/libfuse3.so.3*
```
- run
```bash
./zipfs <file.zip> [options] <mount_point>
```

#### Some tricks on debugging:
- Fuse debug
```bash
./zipfs <file.zip> -f -s -d <mount_point>
  
  -s runs in single threaded mode  
  -f displays debugging output to stdout 
  -d debug model 
```
- debug by gdb
```bash
gcc -Wall -ggdb zipfs.c -lzip -o zipfs -I /usr/include/fuse3/ -L /lib/libfuse3.so.3*
gdb zipfs
[set break points]
run [file.zip] -f -s -d [mount_point]
```

####Reference
https://engineering.facile.it/blog/eng/write-filesystem-fuse/
https://www.cs.nmsu.edu/~pfeiffer/fuse-tutorial/html/unclear.html
https://www.cs.hmc.edu/~geoff/classes/hmc.cs135.201109/homework/fuse/fuse_doc.html
https://github.com/Ninja3047/Fuse-Zip
https://libfuse.github.io/doxygen/index.html
https://libzip.org/documentation/

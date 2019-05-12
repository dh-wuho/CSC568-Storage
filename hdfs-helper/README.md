# HDFS-FUSE

## Requirement

- JDK 8
- FUSE 3.4
- Hadoop 3.1.2

## Build

1. Set environment variable `$JAVA_HOME` and `$HADOOP_HOME`

2. Compile

    ```
    cmake .
    make
    ```

## Run

1. Generate classpath as manifest in jar

    ```
    $HADOOP_HOME/bin/hdfs classpath --jar <path>
    ```

2. Set environment variable `$CLASSPATH=<path>`

3. Run

    ```
    hdfs-mount hdfs://<hostname>:<port> <mountpoint>
    ```

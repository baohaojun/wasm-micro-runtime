# 运行

将 package.tgz 文件拷贝到板子上，解压之后，在解压目录下运行如下命令。

## 启动 runtime app server

```sh
./usr/aarch64-linux-gnu/lib/ld-linux-aarch64.so.1 \
    --library-path ./usr/aarch64-linux-gnu/lib \
    cmake_build/simple-arm64-aot/simple -s -p 8888
```

## 上传并启动 timer app

打开另外一个终端连接到板子，在解压目录下运行：

```sh
./usr/aarch64-linux-gnu/lib/ld-linux-aarch64.so.1 \
    --library-path ./usr/aarch64-linux-gnu/lib \
    ./out/host_tool -i timer -f ./out/wasm-apps/timer.aot -P 8888
```

# 终止运行、数据分析

Timer App 会执行一个死循环，所以要终止的话，必须手动 Ctrl-C。

另外，每个 timer 会调用 alarm 函数等待 10ms，然后会取系统时间，看实际等待了多久。

比如如下数据，`Timer update NNN` 代表第几次调用 alarm； `tick: 10`代表其实际等待的时间是 10ms（与调用参数一致）。

最终我们需要计算一下准确的等待次数与不准确的等待次数的一个占比，以得出此 runtime 系统的一个实时性报告。

        Timer update 1
        ...
        Timer update 4069
        tick: 11
        Timer update 4070
        tick: 10
        Timer update 4071
        tick: 10
        Timer update 4072


# 编译

## 编译 core runtime

在 product-mini/platforms/linux 目录下，用 cmake，交叉编译的 toolchain.cmake 文件已经放在此目录下

## 编译 aot compiler

在 wamr-compiler 目录下，先运行 ./build_llvm.sh，编译出一个项目专用的 llvm 版本，然后再运行 cmake，不需要交叉编译，直接编就可以（编出来的 wamrc 程序本身有交叉编译 aot 的能力）。

## 编译 sample 程序

在 samples/simple 目录下，运行./build.sh，会提示编译的类型，输入 arm64-aot 并回车就可以。

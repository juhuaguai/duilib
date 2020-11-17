主要参考：https://blog.csdn.net/kiss_xq/article/details/105494238
libpng    1.6.37
libpng patch 1.6.37
zlib    1.2.11

说明：
原始的1.6.37+patch代码完全可以编译通过并且正常解析apng；
本仓库做如下调整：
*移除无用的文件，仅保留vs编译时用到的文件；
*移除了除静态库之外的编译配置和选项；
*调整zlib的导出函数名，统一增加dui_前缀，比如adler32调整为dui_adler32，用来解决其他库也使用zlib时的冲突问题;
*其他为了顺利编译和运行所作的微调；
*在vs中设置附加依赖库，以便于把生成的zlib给合并到libpng.lib中；

libpng的sln文件在apng/libpng/projects/visualc71目录下；
libpng生成的测试程序在apng/ibpng/projects/visualc71/bin目录下；
libpng.lib静态库生成到了apng/static目录下；
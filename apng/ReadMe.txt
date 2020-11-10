主要参考：https://blog.csdn.net/kiss_xq/article/details/105494238
libpng    1.6.37
libpng patch 1.6.37
zlib    1.2.11

说明：
原始的1.6.37+patch代码完全可以编译通过并且正常解析apng；
本仓库做如下调整：
*移除无用的文件，仅保留vs编译时用到的文件；
*增加了MT DLL Release编译配置选项；
*移除了VB和LIB Debug编译配置选项；
*其他为了顺利编译和运行所作的微调；

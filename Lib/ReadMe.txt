libpng.lib使用的是静态库，链接的是apng/static里面libpng.lib。
可以看到duilib使用libpng.lib时是添加到附加依赖项里，而不是使用#pragma comment(lib,"libpng.lib")，两者的区别是：
附加依赖项会合并libpng.lib到duilib.lib里，使用duilib时仅链接duilib.lib即可；
使用#pragma comment(lib,"libpng.lib")，使用duilib时需要链接duilib.lib和libpng.lib。
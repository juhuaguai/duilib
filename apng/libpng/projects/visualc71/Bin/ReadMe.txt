测试时可以将生成的pngtest.exe拖到cmd.exe里，然后 参数1表示输入png，参数表示输出png。比如
c:/pngtest.exe c:/pngtest.png c:/pngout.png

MT DLL Release编译选项屏蔽了stdio相关，避免文件指针跨模块时崩溃。
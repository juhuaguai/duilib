======================
Duilib界面文件预览工具
======================

-使用时将界面文件(*.xml)拖到DuilibPreview.exe上即可预览，按"Esc"或者"Alt+F4"退出。

-还可以把预览工具嵌入到你所使用的XML编辑工具

**嵌入到editplus：
    工具->配置用户工具->添加
        菜单文本=DuilibPreview
        命令=DuilibPreview.exe的完整路径
        参数=$(FilePath)
    确定
    现在编辑xml的时候按"Ctrl+1"快捷键就可以快速预览界面了

**嵌入到notepad++：
	打开notpad++
    按F5运行
        程序名=DuilibPreview.exe的完整路径 $(FULL_CURRENT_PATH)			//例如【c:/DuilibPreview.exe $(FULL_CURRENT_PATH)】
    保存
        Name=DuilibPreview
        Alt+F10
	OK
    现在编辑xml的时候按"Alt+F10"快捷键就可以快速预览界面了

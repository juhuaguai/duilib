# duilib
由官方的duilib( https://github.com/duilib/duilib ) 复制过来的，仅为了个人学习使用。<br />
可以使用vs2008打开，新增一些学习测试用的demo，之后也可能会做很少的修改，但代码尽量会与官方保持一致。<br />
有任何问题请联系我，邮箱juhuaguai@yeah.net<br />
<br />
本仓库改动日志：<br />
1.将解决方案sln切换为vs2008打开<br />
2.添加webbrowser控件对js方法window.close的响应<br />
3.给combo控件增加selectedid属性，以便于在xml中可以直接配置默认选中的item<br />
4.给combo控件增加textcolor/font/align属性，便于自由配置combo控件显示的文字的颜色/字体/水平对齐方式<br />
5.给combo控件的下拉窗口增加Notify过滤，将下拉窗口中的DUI_MSGTYPE_ITEMCLICK,DUI_MSGTYPE_ITEMACTIVATE,DUI_MSGTYPE_LINK ,DUI_MSGTYPE_CLICK异步转发到combo控件所在的窗口的notify中<br />
6.修复listElement listContainerElement标签的selected属性false不生效的bug<br />
7.修复combo下拉框的滚动条不能使用default标签中定义的滚动条样式的问题<br />
8.修复容器的滚动条不能正确取消hot状态的问题<br />
9.修复Combo下拉框滚动条相关标签vscrollbar/hscrollbar/vscrollbarstyle/hscrollbarstyle无效问题；<br />
10.修改所有容器设置vscrollbarstyle属性就默认显示vscrollbar(即便vscrollbar属性值为false)、设置hscrollbarstyle属性就默认显示hscrollbar的处理，使滚动条的显示由vscrollbar/hscrollbar的属性来控制<br />
11.给Button、Edit、combo、option控件增加gdi+的文字绘制，仅仅是为了在半透明窗口中抗锯齿，一些文字阴影渐变发光等特效并没有引入<br />
12.修复Label控件采用gdi+绘制时单行限制失效的问题<br />
13.给button控件增加指定光标样式的属性 cursor ,值类型为字符串。如cursor="arrow"指定hot状态光标样式为箭头样式<br />
14.给richedit补充maxchar属性<br />
15.给CTextUI控件类增加获取links数量和对应文字区域(RECT)的接口，以便于在标题栏使用时，能够更加准确的进行事件响应和拖动过滤<br />
16.修复edit获得输入焦点时，控件高度大于文本高度过多时，会显示异常的bug；<br />
17.修复header="false"不能正常隐藏list表头的bug,原版用header="hidden"也能够隐藏，但是个人认为不够统一，就补充个header="false"<br />
18.对半透明窗口的绘制做了调整，修复半透明窗口中背景色的显示可能不是我们预期效果的bug<br />
19.修复VerLayout中padding的一处bug<br />
20.修复当bordersize的left为0时，其余3边的边框也不绘制的bug<br />
21.修复了某些情况下控件的右边框和下边框绘制不出来的bug；当边框宽度为奇数时保证边框是绘制在控件内部的.<br />
22.对子控件的remove默认改为同步销毁。<br />
23.修正第一焦点是Edit时,窗口创建后edit没有默认处理焦点状态的问题；修复当SetFocusNeeded参数为NULL时，窗口初始仍然会给控件设置焦点的bug<br />
24.修复在DUI_MSGTYPE_WINDOWINIT消息中去同时设置窗口大小和layered时可能不生效的bug<br />
25.对Label动态文本计算所需宽度的结果进行修正，当gdi+绘制文本时宽度能够更加准确<br />
26.修复richedit控件动态创建时某些属性可能不生效(需要先设置属性再add到布局才会生效)的bug，涉及的属性包括滚动条，自动滚动等。<br />
27.richedit的光标颜色调整为跟随文本颜色，避免在与背景色接近导致看不出来光标<br />
28.修复当richedit控件的父容器滚动条滚动后，richedit文本位置不正确的bug<br />
29.给CContainerUI增加了DoPostPaint调用，用来修复EndDown不能及时滚动到最底部(总是差最后一行)的bug<br />
30.修改UIlib.h以便于兼容cef和duilib结合使用<br />
31.修复layeredimage绘制可能不正确的bug;修复开启LayeredImage但是不设置图片时文字穿透的问题。<br />
32.修复duilib在使用list添加表头后异常的bug,例如duipreview在预览包含ListHeader的xml时会无法预览.<br />
33.修复多字节工程编译时一处笔误错误，建议最好还是用unicode版的<br />
34.richedit增加setcaret接口来控制是否显示输入光标<br />
35.给richedit添加个DUI_MSGTYPE_LINK的notify消息<br />
36.给richedit增加FocusImage和disabledImage的绘制<br />
37.给Label和Button控件增加focus背景色和disabled背景色<br />
38.调整透明窗口的绘制，不再采用内容与背景混合的方式，改为先绘制背景，再在背景上绘制内容。除图片和颜色绘制外，文本，线条，矩形，圆角矩形等的黑色全部修正为0xFF000001(原本应该是0xFF000000).<br />
39.修复绘制ARGB背景颜色时，透明度无法正确被绘制的bug（比如0x1EFFFFFF）<br />
40.修复文本颜色值为RGB（而非ARGB）时文本完全透明的bug<br />
41.给字体增加删除线属性支持，Font标签中的strikeout="true"表示该字体带删除线<br />
42.button增加DUI_MSGTYPE_MOUSEENTER和DUI_MSGTYPE_MOUSELEAVE的notify发送<br />
43.修复richedit没有textchanged消息的问题<br />
44.给richedit增加一个SetPasswordChar的导出，用以设置密码密文字符<br />
45.combo控件下拉框样式进行调整。使用Item的背景色，并且移除下拉框的边框<br />
46.移除combo中selectedItem中对于下拉框子项visible的判断<br />
47.修改webbrowser代码，给webbrowser增加JS调用c++的处理。增加了JS与C++互相调用的注释，并在CustomControlDemo中增加了相关示例<br />
48.将duilib的zip解压更换掉.支持了zip加密。<br />
49.增加一个计算文本区域大小的函数SIZE CRenderEngine::EstimateTextSize<br />
50.修复treenode删除子控件时没有完全删除的一个bug<br />
51.修复gif控件一处可能潜在的句柄泄漏<br />
52.完善TileLayout，增加固定行的属性，当设置固定行之后，子控件将自动向右摆放，顺序是自上而下，自左而右，超出可以使用水平滚动条。<br />
53.修复鼠标在gif控件上滑动鼠标滚轮滚动条不会滚动的bug<br />
54.给Control增加ForeImage属性。结合Png图片可以方便的做出圆形头像等效果<br />
55.移除了CDuiString对于operator LPCTSTR() const;的重载，用来支持对操作符==的更多的重载，修改了部分源码和部分demo的源码以便于正常编译<br />
56.补充设置滚动条位置时是否传递事件的参数。手动调用接口改变滚动条位置时，可以设置参数来决定是否传递事件通知<br />
57.CProgressUI控件，文本初始化设置为垂直居中并且单行<br />
58.升级了duilib的图片解码库stb_image由原来的2.05升级到2.19<br />
59.引入官方库其他人提交的PR,用于修复Edit输入状态下的右键菜单<br />
60.简单的给richedit增加个系统的复制粘贴右键弹出菜单<br />
61.修复richedit里面的两处笔误；给richedit中CTxtWinHost类的成员变量名增加m_前缀<br />
62.引入官方库的关于List排序的修复<br />
63.引入官方库的关于List排序后选中错误的修复,同步关于SelectItem参数传递错误的修复，见https://github.com/duilib/duilib/pull/182<br />
64.引入官方库的修复UIRender某些情况下占用文件不释放的bug，见https://github.com/duilib/duilib/pull/186<br />
65.滚动条滚动后位置优化。<br />
66.同步官方库的一个pr，关于CDuiStringPtrMap Remove函数问题修复<br />
67.修复win10系统上，存在Showmodal子窗口时，主窗口仍然能够响应鼠标滚轮消息的问题<br />
68.修复画边框border时，指定为虚线风格不生效的bug.(虚线风格要求border的宽度只能为1)<br />
69.修改showhtml绘制时，<n>标签还需要配合multiline="true"才能生效的问题<br />
70.修改容器的setpos代码,当大小可能变化时，重置容器的滚动条滚动范围。用来修复自动布局时，自动大小的子控件，大小计算不对的问题。（比如简单的自动居中布局，点击显示桌面让窗口最小化，再点击窗口显示时，可能会出现滚动条）<br />
71.修复滚动条不在起始点时，调整List的大小，item与header位置不对应的bug<br />
72.修复Option按钮为选中且禁用状态时，不显示禁用状态的背景图，显示的是选中状态的背景图的问题<br />
73.新增了一个菊花样子的loading控件，gid+绘制。来自于网友提供的loading菊花源码中提取修改<br />
74.新增了一个图片旋转控件，gid+绘制，可以自由控制图片的动态旋转。来自于网友提供的圆环旋转源码中提取修改<br />
75.新增了一个支持gif的按钮控件，参考了官方的gif控件<br />
76.支持设置combo的下拉框的边框大小和颜色<br />
77.调整Button按钮的hotbkcolor和hotforeimage的绘制，让他们能够正常叠加<br />
78.给button按钮增加hotbordercolor属性，能够设置鼠标悬停时按钮的边框颜色<br />
79.调整button按钮的hot背景色与背景图的叠加，当处于鼠标悬停状态并且设置的有Hotbkcolor时，先绘制Hotbkcolor，再在上层绘制背景图<br />
80.调整option控件的选中背景色与背景图的叠加，当处于选中状态时，先绘制SelectedBkColor，再在上层绘制背景图<br />
81.给richedit增加tipvalue，通过tipvalue和tipcolor可分别设置提示文本的内容和颜色。主要参考redrain的duilib库中的实现<br />


<br />
其他改动：<br />
1.修改了几处官方库中的笔误。<br />
2.会同步官方库的修复，并且一些提交到官方库的pr如果我认为没问题也会抄过来，一些issue如果我能够优化的，也会尝试优化并提交，Q群里有反馈的bug或者改进我也会抄过来<br />
3.增加个编译静态的配置。自己的工程引入了静态库报一些无法解析之类的，尤其是dllimport之类的，这说明需要将UILIB_STATIC也加入到自己的工程中。因为dllimport之类的显然是动态库用的东西。<br />
4.引入了DuilibPreview工具和源码，可以方便的在editplus和notepad++中预览xml文件，可以一边修改xml一边预览啦，具体参看DuilibPreview工程目录下的说明<br />
5.combo的itemfont建议都是用 shared="true"的字体，因为非共享资源的字体并没有加载到下拉框的PM中。其实最好所有的字体都用shared="true"，这样可以避免多个窗口重复的添加同样的字体，也方便管理。<br />
6.增加了一个gifbutton控件，放到了CustomControlDemo中<br />
7.收集了一些来自网络的duilib相关的代码，分享出来供大家参考使用<br />
8.提交个扩展容器，支出鼠标移入移出。移入移出时容器背景图和背景色可以自动切换，同时可以指定子控件是否跟随移入移出来显示隐藏。<br />
9.备注下，滚动条可以指定每次滚动的幅度，例如：hscrollbarstyle="height=&quot;0&quot; scrollunit=&quot;726&quot;" 指定了滚动条宽度为0，每次滚轮滚动幅度为726像素<br />
10.给两个demo引入系统自带的CS_DROPSHADOW的边框阴影测试，需要系统开启默认阴影效果，代码：SystemParametersInfo(SPI_SETDROPSHADOW,0, (PVOID)TRUE, 0);<br />
11.增加了窗口阴影属性，不使用额外窗口，而是在当前窗口边框直接绘制。bin/Res目录下有shadowdemo.xml可以看。<br />
    详细说明：<br />
    1).需要layeredimage属性支持，即需要启用分层窗口(异形窗口)。<br />
    2).通过shadowsize来控制阴影的宽度，暂时没有做四边阴影分开设置的处理，四边的阴影宽度都是相同的。<br />
    3).阴影部分在caption区域也不会响应拖动，如有需要可自行修改源码。<br />
    4).窗口的尺寸是包含了阴影区域的。<br />
    5).通过shadowcolor来设置阴影渐变终止色。即阴影是从0x00000000渐变到shadowcolor的值，具体代码可以查看paintshadow函数。<br />
    6).通过shadowfocusscales来设置窗口阴影渐变效果缩放。0-1之间,值越大,阴影颜色越深。当为默认值-1时,将自动计算合适的值(计算方法:(100-shadowsize)/100,结果小于0时取0,大于1时取1)。<br />
    7).因为有些窗口原先不是分层窗口，上面的文字,border等都是gdi绘制，其像素没有alpha通道，启用分层窗口后会变透明，鼠标也会穿透。为了便于使用阴影窗口，引入了nolayeredpadding属性。<br />
    8)通过nolayeredpadding来设置非分层区域的外边距。意思是，窗口大小减去外边距之后的矩形区域，其像素将被强制修复alpha为255。这样就避免透明和鼠标穿透了。<br />

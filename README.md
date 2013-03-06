AutoUpdate
==========

自动在线升级源码，暂时只支持http方式升级，可自动先执行升级程序后再执行应用程序。

-Automatic online upgrade code, can automatically perform the upgrade process after the implementation of applications.

1.在你程序开始运行的代码中加入：

WinExec(PChar('"AutoUpdate.exe" /silence'), SW_SHOW);

2.在你程序菜单加一项<自动升级>，其代码为:

WinExec(PChar('"AutoUpdate.exe"'), SW_SHOW);

AutoUpdate.ini为自动升级的配置文件

[AutoUpdate]

URL=http://www.yourweb/update/AutoUpdate.xml	 #指定下载的新版本说明文件

updateAppCaption=smartit IM	 #要更新的更新界面的程序名称

checkVerApp=smartitIM.exe	 #要更新的主程序，以此程序的版本与AutoUpdate.xml中版本对比

checkVerAppRegRootKey=\Software\smartitIM	 #若主程序中将当前版本号写入的注册表区，可不配置

[copyright]

homepage=	 #更新界面的连接网址

licensekey=	 #注册后可设置你自己的版权信息

#多语言配置

[Locale_en]

[Locale_zh-CN]

AutoUpdate.xml为批明新版本的下载文件和新版本说明等，大家可参考此格式写自己的更新文件，有两种格式：

1.以一个安装程序来更新:

<update version="新版本号" datetime="新版本日期" url="新版本的安装程序下载地址">

新版本说明

</update>

2.只更新几个文件的：

<update version="新版本号" datetime="新版本日期">

<desc>

新版本说明

</desc>

<file1 url="新版本的文件下载地址"/>

<file2 url="新版本的文件下载地址"/>

</update>
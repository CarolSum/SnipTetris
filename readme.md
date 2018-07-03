# SnipTetris（俄罗斯方块切割版）

### Dependencies
python2.7
cocos2d-x-3.17
Windows SDK 10.0.16299.0

打开将要存放项目的目录地址并执行命令

`> cocos new SnipTetris -l cpp`

在当前目录下创建项目成功后，打开项目文件夹

`> cd SnipTetris`

将此Repo克隆下来

`git clone git@github.com:owtotwo/SnipTetris.git`

先删除原有的Classes和Resources文件夹

`rm -r Classes Resources`

并且将其中的内容迁移出来根目录，形成git的仓库

`mv SnipTetris/.[!.]* . && rm -r SnipTetris`

这样就成功地将本Repo的Classes和Resources给替换进去了

接下来打开proj.win32文件夹中的SnipTetris.sln启动解决方案

将解决方案中每一个项目的Windows SDK版本都设置为10.0.16299.0

将SnipTetris中（共4个项目）的src文件夹里的所有项移除，并添加现有项，全选Classes文件夹中的代码文件并添加。

最后生成项目，并执行即可。
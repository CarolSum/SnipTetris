# SnipTetris（俄罗斯方块切割版）

### 玩法
- 双人协作游戏（双手操作也行），同时操作两个方块。
- 当两个方块在空中出现重叠时，进入**重叠状态**，此时任意一个方块可以使用剪切
功能，将另一方的重叠部分进行消除（自身的不消除，非即相消）。
- 若出现只有一个格子独立存在的时候（一般原生为四格、五格或六格），如一个五格横条
被切分成分离的一格和两格（中间两格被消除了），此时即算做出现独立的一格，那么此
格将被消除。也就是说，只有存在相邻格子的格子才允许存在。
- 重叠状态下落地面并与地面结合时，判定为游戏结束。
- 双方中任意一方没有格子存在时，判定为游戏结束。（也就是不能剪没了）

### 依赖
python2.7
cocos2d-x-3.17
Windows SDK 10.0.16299.0

### 部署开发
打开将要存放项目的目录地址并执行命令

`> cocos new SnipTetris -l cpp`

在当前目录下创建项目成功后，打开项目文件夹

`> cd SnipTetris`

将此Repo克隆下来

`git clone git@github.com:owtotwo/SnipTetris.git`

先删除原有的Classes和Resources文件夹

`rm -rf Classes/ Resources/ .git/`

并且将其中的内容迁移出来根目录，形成git的仓库

`mv SnipTetris/{.,}[!.]* . && rm -r SnipTetris`

这样就成功地将本Repo的Classes和Resources给替换进去了

接下来打开proj.win32文件夹中的SnipTetris.sln启动解决方案

将解决方案中每一个项目的Windows SDK版本都设置为10.0.16299.0

将SnipTetris中（共4个项目）的src文件夹里的所有项移除，并添加现有项，全选Classes文件夹中的代码文件并添加。

最后生成项目，并执行即可。

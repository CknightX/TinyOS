## 2018-12-25
考研回来了，发现过去的代码完全看不懂了:( 
把bochs换成了qemu，相比于bochs的一大坨脚本，qemu一行命令就搞定了。
目前存在的问题是，通过在link脚本中定义的kernel_start和kernel_end来计算得到的内核占用的内存大小完全不靠谱。不知道是怎个情况。

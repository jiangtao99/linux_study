v2版本是服务端使用mmap一次性发送
循环次数少
在文件内容传递得时候没有使用小火车
怎么保证文件内容对的呢？依靠TCP的可靠性
提前告知了文件的大小，所以确定了文件的边界
TCP是可靠连续的

但是一次性发送一个大文件有一个缺陷，消耗很大的内存
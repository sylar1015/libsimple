# libsimple
frequently used C functions based on linux system .

it was a sub project of a large distributed conmunication project .

wrapper for common functions, such as memory, string, regex, etc ...

wrapper for 3rd party functions, such as reactor, xml, etc ...

reactor is based on libevent, to use reactor api, you need to install libevent yourself;

Or "make libevent" before "make" to make libevent locally;

log is based on zlog, you need to install zlog yourself;

Or "make zlog" before "make" to make zlog locally;

xml is based on libxml2/xpath, to use xml api, you need to install libxml2 yourself;

task is based on tipc, to use task api, you need to install tipc yourself

To Be Continued ...

Linux下常用的C函数;

这是一个分布式集群通讯系统的子模块;

因为一些商用模块不能提供代码,这里重写了一部分;

Linux下常用C函数的简单封装, Memory/String等;;

常用第三方库的封装, Reactor/XML等;

持续添加中 ...

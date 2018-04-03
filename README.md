# libsimple
frequently used C functions based on linux system .

it was a sub project of a large distributed conmunication project .

wrapper for common functions, such as memory, string, regex, etc ...

wrapper for 3rd party functions, such as reactor, xml, etc ...

sp_http is for restful client

sp_jsonrpc is for jsonrpc

git submodule update --init to install submodules

To Be Continued ...

Linux下常用的C函数;

这是一个分布式集群通讯系统的子模块;

因为一些商用模块不能提供代码,这里重写了一部分;

组件包括, Reactor/XML/String/Memory/Log/Task 等;

Reactor 依赖 libevent
XML/XPATH 依赖 libxml2
Log 依赖 zlog
Task 依赖 tipc
Http 依赖 curl
持续添加中 ...

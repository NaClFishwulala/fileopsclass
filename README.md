一、任务：编写一个带缓存的文件操作类
    从执行体程序库中的CLLogger类可知，通过缓存要写入文件中的数据，能够提高读写磁盘的性能
    其要求如下：
    1.1 需要提供open/read/write/lseek/close等函数的封装函数
    1.2 该类要提供数据缓存服务。
        1.2.1 调用该类的写操作接口时，数据要首先写到缓存，然后再根据策略写到文件中。
        1.2.2 调用该类的读操作接口时，该类能根据策略缓存读出的数据

二、框架说明
    本项目定义了两个类，分别为文件操作类CFileOps以及用于存储文件操作后的状态的类CFileOps。
    其中，文件操作类CFileOps使用了单例设计模式，确保在程序运行的过程中，该类只被定义一次

三、项目进度
Finished:
1. 完成了状态类CFileOps的编写
2. 完成了对open函数的封装
3. 完成了对close函数的封装
4. 完成了对read函数进行封装
5. 完成了对write函数进行封装
6. 完成了对lseek函数进行封装

TODO:
1. 提供文件写的缓存操作
2. 提供文件读的缓存操作
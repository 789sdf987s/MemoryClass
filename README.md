# MemoryClass
 This is a very simple memory class that allows you to read memory of another process and its modules. It can be used to make for example external cheats. This class was created and tested for 32-bit games.

# Compilation
 To compile this class please use Visual Studio 2019 or greater. In solution you will recieve one project, compile it as Release | x86 and use Multi-byte character set.
 
# Usage
 Put Memory.h and Memory.cpp in your C++ project and create a instance (ex. cMemory Memory). Then you can use FindProcess() to find process that you want to read from (ex. FindProcess("csgo.exe")). This will automaticly open handle to the process and get its ID (Memory.ProcessHandle and Memory.ProcessID). Read and write memory functions will automaticly read from handle that you opened with FindProcess().
 
# License 
 > Copyright (c) 2020 Nexo

This project is licensed under the [MIT License](https://opensource.org/licenses/mit-license.php) - see the [LICENSE](LICENSE) file for details.

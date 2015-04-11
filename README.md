Max TCP Client
==============

a basic TCP client for *nix systems (tested on Mac OSX 10.7)
--------------

*Project Dependencies*

- [Max SKD:](https://cycling74.com/downloads/sdk/) tested with version 6.1.4 / Max 7
- [MaxCpp:](https://github.com/grrrwaaa/maxcpp) a great C++ wrapper for the Max SDK
- socket.h: should be present on your *nix system 

*Build Instructions*

1. set up MaxCpp per the instructions on the repository readme
2. place the TcpClient folder in the directory "maxcpp/examples"
3. add .mxo build path (defaults to "Library/Developer/Xcode/DerivedData" in the xCode project) in your Max File Preferences
4. add "TcpClient/js" to Max File Preferences
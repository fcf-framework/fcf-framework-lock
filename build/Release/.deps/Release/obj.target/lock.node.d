cmd_Release/obj.target/lock.node := g++ -o Release/obj.target/lock.node -shared -pthread -rdynamic -m64  -Wl,-soname=lock.node -Wl,--start-group Release/obj.target/lock/lock.o -Wl,--end-group 

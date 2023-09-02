cmd_Release/lock.node := ln -f "Release/obj.target/lock.node" "Release/lock.node" 2>/dev/null || (rm -rf "Release/lock.node" && cp -af "Release/obj.target/lock.node" "Release/lock.node")

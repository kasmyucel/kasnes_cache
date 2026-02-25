**LRU CACHE** 

This project is a **cache for servers** that aims to provide **low latency** under **high-density network traffic**. It implements **least recently used structure** with **multithreading** , **doubly linked list** and **asynchronous I/O**.

The system starts listening to clients through the **TCP connection** and manages many connection asynchronously via the **epoll mechanism**  which ensures efficiency. It provides **concurrency mechanism** with **ThreadPool** and **ConnectionPool**. Every client request that comes through the **TCP connection** is assigned to an empty **ClientConnection** via **ConnectionPool** and while processing the request a thread is assigned via the **ThreadPool**. This strategy prevents the cost that comes from `new` and `delete` operations  and ensures stability. Also it provides a **graceful shutdown** via the **eventfd mechanism**.

The strategy that includes the aim of the project is provided via the **DataService layer**. It manages the operations from the **database** and the **LRUCache**. The request that comes from the client is firstly directed to the **LRUCache**. **LRUCache** provides **O(1) time complexity** for getting the data from the server by storing the **least recently used data** in a **lock-guarded std::unordered_map** and **doubly linked list**. If the data is not found on the **LRUCache** , **DataService** makes a **SQL query** on database by using **DBPool mechanism**  to ensure efficiency. After getting the data on database , **DataService** puts the data on the **LRUCache** by using **ConnectionGuard structure** to ensure releasing the connections safely.

Also the project has a `settings.h` file to provide easy configuration options and **Data class**  as an example data.

**Contributors:**

Abdullah Kasım Yücel: Implemented LRUCache, DataService and DBPool mechanism.
Bekir Enes Göğebakan: Implemented TCPServer and ClientPool mechanism.

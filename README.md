

MultiThreadedTCPserver: 
Implementation of Multi-thread TCP server to handle multiple client request in parallel. Current version of implementaion spwan seaparate thread to handle each Client. Once all the clients send EOM massge , Server shut down itself as no more service required.

Build instructions:
Ensure g++ compiler along with support for linux socket reletd lib and c++11 standard thread library is present. Then run the below commands.

to generate Server and Client executabe: make all

to clean executable: make clean

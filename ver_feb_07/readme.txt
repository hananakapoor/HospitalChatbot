Prerequisites

sudo apt update
sudo apt install libboost-system-dev libboost-filesystem-dev
sudo apt install libmysqlclient-dev
sudo apt install build-essential
sudo apt install g++

 Compilation and Execution
to run 
1 .g++ -std=c++17 -o server voice_chat2.cpp -lboost_system -lboost_filesystem -lpthread -lmysqlclient

2. sudo service mysql start


3. ./server


## Database Configuration

manager_ = new CallManager("newpassword", "newuser", "127.0.0.1", "3306", "ksfe_db", 10, 5);



change name to your root  and password   ;change db to your db name table name  shoud be same States and Transitions





sudo service mysql start



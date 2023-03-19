# Video_Streaming_App_Project
Project môn lập trình mạng HEDSPI 

Project này được làm bởi nhóm 4 người học kì 20221 HUST. 

Do trưởng nhóm không public repository nen mình up lại trên đây.

# Thông tin về project 
Server: 
Sử dụng server C và giao thưc là TCP đẻ giao tiếp với client 

Sử dụng 1 server khác là python để stream (server này sẽ chạy thông qua hamg engine trong server C)

Client: Reactjs (mình làm bên phía server nên không rõ lắm : D ) 

Database: Postgres SQL 

# Chạy 

Bạn phải cài đặt thư viện libpq-fe để kết nối database 

Phía server: 
cc -c -I/usr/include/postgresql server.c

cc -pthread -o server server.o -L/usr/bin -lpq

./server 

Phía Client: 

npm install

npm start 

# LƯU Ý: Chương trình chỉ thử nghiệm chạy trên Ubuntu. Các trình duyệt chạy ddược là Mozilla và Chrome. 

# Sẽ update comment code bên phía server sau 

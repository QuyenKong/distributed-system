import socket

def send_request(req):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect(("127.0.0.1", 5050))  # Kết nối tới server
        s.sendall((req + "\n").encode("utf-8"))  # Gửi request
        response = s.recv(1024).decode("utf-8")  # Nhận response
        print("Server:", response)

# Test thử các lệnh
send_request("KV/1.0 PUT user42 Alice")
send_request("KV/1.0 GET user42")
send_request("KV/1.0 DEL user42")
send_request("KV/1.0 STATS")
send_request("KV/1.0 QUIT")
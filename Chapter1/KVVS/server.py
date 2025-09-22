import socket   # Thư viện để làm việc với socket TCP
import threading  # Để xử lý nhiều client song song

# Tạo storage (dictionary) để lưu key-value
store = {}

# Hàm xử lý request từ client
def process_request(request):
    try:
        # request dạng: KV/1.0 COMMAND key value
        parts = request.strip().split()
        if len(parts) < 2:
            return "ERR Invalid request"

        version, command = parts[0], parts[1]
        if version != "KV/1.0":
            return "ERR Unsupported version"

        # Lệnh PUT
        if command == "PUT" and len(parts) == 4:
            key, value = parts[2], parts[3]
            store[key] = value
            return f"OK PUT {key}"

        # Lệnh GET
        elif command == "GET" and len(parts) == 3:
            key = parts[2]
            return f"OK {key} {store.get(key, 'NULL')}"

        # Lệnh DEL
        elif command == "DEL" and len(parts) == 3:
            key = parts[2]
            if key in store:
                del store[key]
                return f"OK DEL {key}"
            return "ERR Key not found"

        # Lệnh STATS
        elif command == "STATS":
            return f"OK STATS {len(store)} entries"

        # Lệnh QUIT
        elif command == "QUIT":
            return "BYE"

        else:
            return "ERR Invalid command"
    except Exception as e:
        return f"ERR Exception {e}"

# Hàm thread để xử lý client
def handle_client(conn, addr):
    print(f"Client connected: {addr}")
    with conn:
        while True:
            data = conn.recv(1024).decode("utf-8")
            if not data:
                break
            requests = data.strip().split("\n")
            for request in requests:
                response = process_request(request)
                conn.sendall((response + "\n").encode("utf-8"))

# Hàm main server
def start_server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind(("127.0.0.1", 5050))  # Gắn server vào localhost:5050
        s.listen()
        print("Server listening on 127.0.0.1:5050")
        while True:
            conn, addr = s.accept()
            threading.Thread(target=handle_client, args=(conn, addr)).start()

if __name__ == "__main__":
    start_server()
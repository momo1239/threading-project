import socket
import sys

class CommandClient:
    def __init__(self, server_address, server_port):
        self.server_address = server_address
        self.server_port = server_port
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.buffer_size = 4096

    def send_command(self, command):
        try:
            self.client_socket.sendto(command.encode(), (self.server_address, self.server_port))
            data, _ = self.client_socket.recvfrom(self.buffer_size)
            return data.decode()
        except socket.error as e:
            return f"Socket error: {e}"
        except Exception as e:
            return f"An unexpected error occurred: {e}"

    def query(self, packet_id):
        command = f"query {packet_id}"
        response = self.send_command(command)
        print(response)

    def status(self):
        command = "status"
        response = self.send_command(command)
        print(response)

    def list_packets(self):
        command = "list"
        response = self.send_command(command)
        print(response)

    def pause(self):
        command = "pause"
        response = self.send_command(command)
        print(response)

    def shutdown(self):
        command = "shutdown"
        response = self.send_command(command)
        print(response)
        self.client_socket.close()
        sys.exit(0)

    def help_menu(self):
        print("\nAvailable commands:")
        print("1. QUERY <packet ID> - Query a specific packet by ID")
        print("2. STATUS - Check the status of the arrays")
        print("3. LIST - List all packet IDs currently in the result array")
        print("4. PAUSE - Pause or resume the processor threads")
        print("5. SHUTDOWN - Gracefully shutdown the server")
        print("6. HELP - Show this help menu")
        print("7. QUIT - Exit the client\n")

    def quit(self):
        self.client_socket.close()
        print("Exiting...")
        sys.exit(0)

def main():
    if len(sys.argv) != 3:
        print("Usage: python3 command_client.py <server_address> <server_port>")
        sys.exit(1)

    server_address = sys.argv[1]
    server_port = int(sys.argv[2])

    client = CommandClient(server_address, server_port)

    print("Type HELP for available commands")

    while True:
        user_input = input("command> ").strip().split()
        if not user_input:
            continue

        command = user_input[0].upper()

        if command == "QUERY":
            if len(user_input) == 2:
                client.query(user_input[1])
            else:
                print("Usage: QUERY <packet ID>")
        elif command == "STATUS":
            client.status()
        elif command == "LIST":
            client.list_packets()
        elif command == "PAUSE":
            client.pause()
        elif command == "SHUTDOWN":
            client.shutdown()
        elif command == "HELP":
            client.help_menu()
        elif command == "QUIT":
            client.quit()
            break
        else:
            print("Invalid command! Type HELP for the list of available commands.")

if __name__ == "__main__":
    main()

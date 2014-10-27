#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "chmod_lcd.h"
#include "kbd_hw.h"


#define DEV_ENABLE "/sys/bus/pci/devices/0000:03:00.0/enable"


#define h_addr h_addr_list[0]
#define CTRL 0x8080
#define ADDR 0x8060
#define DATAOUT 0x8020
#define WR 0x8060
#define RD 0x8040

unsigned int DEV_ADDRESS;
unsigned int DELKA;
int gateID = 1;
int gateSize = 1;


int readed = 0;
int r = -1;

int isPressed[14];

int username[10];
int password[10];

int led[10];

void findBoard() {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("/proc/bus/pci/devices", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);



    while ((read = getline(&line, &len, fp)) != -1) {

        if (line[5] == '1' && line[6] == '1' && line[7] == '7'
                && line[8] == '2' && line[9] == '1' && line[10] == 'f' && line[11] == '3' && line[12] == '2'
                ) {
            line[22] = '0';
            line[23] = 'x';
            int start = 22, count = 10;
            char string2[11] = {0};
            char *ptr1 = &line[start];
            char *stop = ptr1 + count;
            char *ptr2 = string2;
            while ((ptr1 < stop) && (*ptr2++ = *ptr1++));
            line[144] = '0';
            line[145] = 'x';
            start = 144;
            count = 7;
            char string3[8] = {0};
            ptr1 = &line[start];
            stop = ptr1 + count;
            ptr2 = string3;
            while ((ptr1 < stop) && (*ptr2++ = *ptr1++));
            DELKA = (int) strtol(string3, NULL, 16);
            DEV_ADDRESS = (int) strtol(string2, NULL, 16);
        }

    }
}

/* Power operation */
int power(int base, int exp) {
    int result = 1;
    while (exp) {
        result *= base;
        exp--;
    }
    return result;
}

/* Creates integer from bin array */
int getInt(int * array, int size) {
    int result = 0;

    int i = 0;
    for (i = 1; i < size + 1; i++) {
        int pow1 = (int) power(10, (size - i));
        result = result + (array[i - 1] * pow1);

    }
    return result;
}

/*  Write to board  memory */
void writeW(unsigned char Adress, unsigned char Data, unsigned char* base, int wait) {
    *(base + CTRL) = 0xff;
    *(base + ADDR) = Adress;
    *(base + DATAOUT) = Data;
    *(base + CTRL) = 0xfd;
    *(base + CTRL) = 0xbd;
    usleep(wait);
    *(base + CTRL) = 0xfd;
    *(base + CTRL) = 0xff;
}

/* Write to board memory and update LED state */
void write1(unsigned char Adress, unsigned char Data, unsigned char * base) {
    writeW(Adress, Data, base, 1000);
    updateLED(base);
}

/* Read from board memory */
int read1(unsigned int adress, unsigned char * base) {
    *(base + ADDR) = adress;
    *(base + CTRL) = 0xFE;
    *(base + CTRL) = 0xBE;
    usleep(100);
    int readed = *(base + RD);
    *(base + CTRL) = 0xFF;
    return readed;
}

/* Read keyboard line 1 */
int readKB1(unsigned char * base) {
    write1(BUS_KBD_WR_o, 0x03, base);
    int col = read1(BUS_KBD_RD_o, base);
    if (col != 0xFF) {
        if (col == 0xFE) {
            beep(base);
            readed = 1;
            isPressed[1] = 1;
        }
        if (col == 0xFD) {
            beep(base);
            readed = 1;
            isPressed[4] = 1;
        }
        if (col == 0xFB) {
            beep(base);
            readed = 1;
            isPressed[7] = 1;
        }

        if (col == 0xF7) {
            beep(base);
            readed = 1;
            isPressed[10] = 1;
        }
        if (col == 0xEF) {
            beep(base);
            readed = 1;
            isPressed[13] = 1;
        }
    }
}

/* Read keyboard line 2 */
int readKB2(unsigned char * base) {
    write1(BUS_KBD_WR_o, 0x05, base);
    int col = read1(BUS_KBD_RD_o, base);
    if (col != 0xFF) {
        if (col == 0xFE) {
            beep(base);
            readed = 1;
            isPressed[2] = 1;
        }

        if (col == 0xFD) {
            beep(base);
            readed = 1;
            isPressed[5] = 1;
        }
        if (col == 0xFB) {
            beep(base);
            readed = 1;
            isPressed[8] = 1;
        }
        if (col == 0xF7) {
            beep(base);
            readed = 1;
            isPressed[0] = 1;
        }
        if (col == 0xEF) {
            beep(base);
            readed = 1;
            isPressed[12] = 1;
        }
    }
}

/* Read keyboard line 3 */
int readKB3(unsigned char * base) {
    write1(BUS_KBD_WR_o, 0x06, base);
    int col = read1(BUS_KBD_RD_o, base);
    if (col != 0xFF) {
        if (col == 0xFE) {
            beep(base);
            readed = 1;
            isPressed[3] = 1;
        }
        if (col == 0xFD) {
            beep(base);
            readed = 1;
            isPressed[6] = 1;
        }
        if (col == 0xFB) {
            beep(base);
            readed = 1;
            isPressed[9] = 1;
        }
        if (col == 0xF7) {
            beep(base);
            readed = 1;
            isPressed[11] = 1;
        }

    }
}

/* Beep */
void short_beep(unsigned char * base) {
    write1(3, 0xFF, base);
    usleep(400000);
    write1(3, 0x00, base);
}

/* Update LED state */
void updateLED(char unsigned * base) {
    int i = 0;
    int num = 0;
    for (i = 0; i < 8; i++) {
        int b = led[i];
        num = (num << 1) | b;
    };
    writeW(1, num, base, 1000);
}

/* Turn specific LED on or off */
void turnLED(int ledc, int value, char unsigned * base) {
    led[ledc] = value;
    updateLED(base);
}

/* Turn all LED on */
void turnLEDON(unsigned char * base) {
    int t = 0;
    for (t = 0; t < 11; t++) {
        turnLED(t, 1, base);
    }

}

/*Turn all LED off*/
void turnLEDOFF(unsigned char * base) {
    int t = 0;
    for (t = 0; t < 11; t++) {
        turnLED(t, 0, base);
    }

}

/* KITT LED animation https://www.youtube.com/watch?v=j4iPih1AgLk */
void kitt(char unsigned * base) {
    int t = 50000;
    int i = 0;
    for (i = 0; i < 8; i++) {
        turnLED(i, 1, base);
        usleep(t);
        turnLED(i, 0, base);

    }
    turnLED(9, 1, base);
    usleep(t);
    for (i = 9; i > -1; i--) {
        turnLED(i, 1, base);
        usleep(t);
        turnLED(i, 0, base);
    }
}

/* Tree long beeps */
void three_long_beeps(unsigned char * base) {
    turnLEDON(base);
    write1(3, 0x80, base);
    usleep(400000);
    turnLEDOFF(base);
    write1(3, 0x00, base);

    turnLEDON(base);
    usleep(400000);

    write1(3, 0x80, base);
    usleep(400000);
    turnLEDOFF(base);
    write1(3, 0x00, base);

    turnLEDON(base);
    usleep(400000);

    write1(3, 0x80, base);
    usleep(400000);
    turnLEDOFF(base);
    write1(3, 0x00, base);
}

/* One keyboard standart beep*/
void beep(unsigned char * base) {
    write1(3, 0x80, base);
    usleep(40000);
    write1(3, 0x00, base);
}

/* Write given char value to LCD */
void displayLCDCharW(char c, int pos, unsigned char * base, char off) {
    write1(WR, CHMOD_LCD_POS + pos + off, base);
    write1(2, c, base);
}

/* Write  char on 1 line of LCD */
void displayLCDChar(char c, int pos, unsigned char * base) {
    displayLCDCharW(c, pos, base, 0x00);
}

/* Write  char on 2 line of LCD */
void displayLCDCharD(char c, int pos, unsigned char * base) {
    displayLCDCharW(c, pos, base, 0x40);
}

/* Write  string on  LCD */
void displatLCDSrtingW(char *str, unsigned char * base, char off) {
    int len = strlen(str);
    int i = 0;
    if (off > 0) for (i = 0; i < len; i++) displayLCDCharD(str[i], i, base);
    if (off == 0) for (i = 0; i < len; i++) displayLCDChar(str[i], i, base);
}

/* Write  string on 1 line of LCD */
void displayLCDString(char *str, unsigned char * base) {
    displatLCDSrtingW(str, base, 0);
}

/* Write  string on 2 line of LCD */
void displayLCDStringD(char *str, unsigned char * base) {
    displatLCDSrtingW(str, base, 1);
}

/* Clear 1 line of LCD slow way */
void clearLCDSlow(unsigned char * base) {
    displayLCDString("                ", base);
}

/* Clear 2 line of LCD slow way */
void clearLCDSlowD(unsigned char * base) {
    displayLCDStringD("                ", base);
}

/* Clear LCD fast way */
void clearLCD(unsigned char * base) {
    write1(WR, CHMOD_LCD_CLR, base);
    usleep(10000);
}

/* Init LCD*/
void initLCD(unsigned char * base) {
    write1(WR, CHMOD_LCD_MOD, base);
    usleep(10000);
    write1(WR, CHMOD_LCD_MOD, base);
    usleep(10000);
    clearLCD(base);
    write1(WR, CHMOD_LCD_DON, base);
    usleep(10000);
}

/* Server communication */
int check_cardinals(int username_pos, int password_pos, unsigned char * base) {
    int sock;
    struct sockaddr_in server;
    char server_reply[2000];

    //Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        //perror("Could not create socket");
        return -1;
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(55556);

    //Connect to rdo1()emote server
    if (connect(sock, (struct sockaddr *) &server, sizeof (server)) < 0) {
        //perror("connect failed. Error");
        return -1;
    }

    //puts("Connected\n");
    char message [100];

    snprintf(message, sizeof message, "%s %d %d %d\n", "checkaccess", gateID, getInt(username, username_pos), getInt(password, password_pos));
    if (send(sock, message, strlen(message), 0) < 0) {
        //perror("Send failed");
        return -1;
    }

    if (recv(sock, server_reply, 2000, 0) < 0) {
        //perror("recv failed");
        return -1;
    }

    if (server_reply[10 + 1 + gateSize + 1 + username_pos + 1 + 1] == 'O') {
        printf("Access granted\n\n");
        return 1;
    } else {
        printf("Access denied\n\n");
        return 0;
    }
    close(sock);
}

/* Clear array with pressed keys */
void clearInput() {
    int i = 0;
    for (i; i < 14; i++) {
        isPressed[i] = 0;
    }
}

/* Read username */
int read_username(unsigned char * base) {
    clearLCDSlow(base);
    clearLCDSlowD(base);
    displayLCDString("ENTER USERID:", base);
    int i, result;
    int username_pos = 0;
    int pos = 0;
    while (1) {

        readKB1(base);
        usleep(100);
        readKB2(base);
        usleep(100);
        readKB3(base);
        usleep(100);
        if (readed == 1) {
            for (i = 0; i < 14; i++) {
                if (isPressed[i]) {
                    if (i >= 0 && i <= 9) {
                        username[username_pos] = i;
                        username_pos++;
                        displayLCDCharD((char) (((int) '0') + i), pos, base);
                        pos++;
                        if (username_pos > 10) {
                            displayLCDString("Wrong Username", base);
                            usleep(1000000);
                            return -1;
                        }
                    } else {
                        if (i == 10)//star is pressed
                        {
                            printf("* \n");
                        } else if (i == 11)//hash is pressed
                        {
                            printf("# \n");
                        } else if (i == 12)//cancel is pressed
                        {
                            clearLCD(base);
                            initLCD(base);
                            pos = 0;
                        } else if (i = 13)//enter is pressed
                        {
                            return username_pos;
                        }
                    }

                }

            }
            clearInput();
            readed = 0;
            usleep(150000);
        }
    }
}

/* Read password */
int read_password(unsigned char * base) {
    clearLCDSlow(base);
    clearLCDSlowD(base);
    displayLCDString("ENTER PASSWORD:", base);
    int i;
    int password_pos = 0;
    int pos = 0;
    while (1) {
        readKB1(base);
        usleep(1000);
        readKB2(base);
        usleep(1000);
        readKB3(base);
        usleep(1000);
        if (readed == 1) {
            for (i = 0; i < 14; i++) {
                if (isPressed[i]) {
                    if (i >= 0 && i <= 9) {
                        password[password_pos] = i;
                        password_pos++;
                        displayLCDCharD('*', pos, base);
                        pos++;
                        if (password_pos > 10) {
                            displayLCDString("Wrong password", base);
                            usleep(1000000);
                            return -1;
                        }
                    } else {
                        if (i == 10)//star is pressed
                        {
                            printf("* \n");
                        }
                        if (i == 11)//hash is pressed
                        {
                            printf("# \n");
                        }
                        if (i == 12)//cancel is pressed
                        {
                            clearLCD(base);
                            initLCD(base);
                            pos = 0;
                        }
                        if (i = 13)//enter is pressed
                        {
                            return password_pos;
                        }
                    }

                }

            }
            clearInput();
            readed = 0;
            usleep(150000);
        }
    }
}

/* Main loop : read login and password, check it and start over */
int scanKeyboard(unsigned char * base) {
    int i = 0;
    int username_pos, password_pos;
    while (1) {
        username_pos = read_username(base);
        clearInput();
        if (username_pos > 0) break;
    }

    printf("Username: ");

    for (i = 0; i < username_pos; i++) {
        printf("%d", username[i]);
    }
    clearInput();
    printf("\n");

    while (1) {
        password_pos = read_password(base);
        clearInput();
        if (password_pos > 0) break;
    }

    printf("Password: ");

    for (i = 0; i < password_pos; i++) {
        printf("%d", password[i]);
    }
    clearInput();
    printf("\n");
    int check = check_cardinals(username_pos, password_pos, base);
    if (check == 1) {
        clearLCDSlow(base);
        clearLCDSlowD(base);
        turnLEDON(base);
        displayLCDString("Access granted!", base);
        clearLCDSlowD(base);
        short_beep(base);
        usleep(800000);
        turnLEDOFF(base);
    } else if (check == 0) {
        clearLCDSlow(base);
        clearLCDSlowD(base);
        turnLEDON(base);
        displayLCDString("Access denied! ", base);
        clearLCDSlowD(base);
        three_long_beeps(base);
        usleep(800000);
        turnLEDOFF(base);
    } else {
        clearLCDSlow(base);
        clearLCDSlowD(base);
        turnLEDON(base);
        displayLCDString("Access denied! ", base);
        displayLCDStringD("Network issues! ", base);
        three_long_beeps(base);
        usleep(800000);
        turnLEDOFF(base);
    }
    scanKeyboard(base);
}

/* Show init screen on start */
void initLoad(unsigned char * base) {
    displayLCDString("PROGRAM: PCIe s.", base);
    displayLCDStringD("STATUS : init", base);
}

/* Main program. Finds board, start it, init display and LED, start main loop*/
int main() {
    findBoard();
    printf("DELKA: %x \n", DELKA);
    printf("DEV_ADDRESS: %x \n", DEV_ADDRESS);
    int soubor = open("/dev/mem", O_RDWR | O_SYNC);
    if (soubor == -1) return 1;

    unsigned char * base = mmap(NULL, DELKA, PROT_WRITE | PROT_READ, MAP_SHARED,
            soubor, DEV_ADDRESS);
    if (base == MAP_FAILED) return 2;

    if (1) {
        *(base + CTRL) = 0x00;
        write1(1, 0x00, base);
        usleep(100000);
        initLCD(base);
        usleep(100000);
        initLoad(base);
        usleep(100000);
        write1(0, 3, base);
        usleep(100000);

        kitt(base);
        kitt(base);
        kitt(base);
        usleep(100000);
        scanKeyboard(base);
        *(base + CTRL) = 0x00;
    }
    return 0;
}

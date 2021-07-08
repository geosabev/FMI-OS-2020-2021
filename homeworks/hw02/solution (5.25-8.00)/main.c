#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

void validateOpen(int r)
{
    if (r == -1)
    {
        err(3, "Something went wrong while trying to open the given file!");
        exit(3);
    }
}
void validateRead(int r1, int r2, int r3, int fileDesc)
{
    if (r1 == -1 || r2 == -1 || r3 == -1)
    {
        close(fileDesc);

        err(4, "Read failed!");
        exit(4);
    }
}
void validateWrite(size_t ws1, size_t s1, size_t ws2, size_t s2, size_t ws3, size_t s3, size_t ws4, size_t s4, int fileDesc)
{
    if (ws1 != s1 || ws2 != s2 || ws3 != s3 || ws4 != s4)
    {
        close(fileDesc);

        err(5, "Write failed!");
        exit(5);
    }
}
void printState(double t, double tC, uint16_t slots)
{
    fprintf(stderr, "[%.3f]<state> temp: %.2f °C, slots:", t, tC);

    for (int i = 0; i <= 15; i++)
    {
        uint8_t bit = slots % 2;

        slots = slots / 2;

        if (bit == 0)
        {
            fprintf(stderr, " %d[ ]", i);
        }
        else
        {
            fprintf(stderr, " %d[X]", i);
        }
    }

    fprintf(stderr, "\n");
}
void printSlotText(double t, uint8_t id, char text[13])
{
    fprintf(stderr, "[%.3f]<slot text> slot %d: %s\n", t, id, text);
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        errx(1, "Wrong number of arguments!");
        exit(1);
    }

    struct timeval start, stop;
    gettimeofday(&start, NULL);

    if (strcmp(argv[1], "record") == 0)
    {
        int fileDescriptor = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC);
        validateOpen(fileDescriptor);

        uint16_t type;
        int rMain;

        while ((rMain=read(0, &type, sizeof(type))))
        {
            validateRead(rMain, 1, 1, fileDescriptor);

            if (type == 0x0001)
            {
                uint16_t bit_mask;
                int tempK;

                int r1 = read(0, &bit_mask, sizeof(bit_mask));
                int r2 = read(0, &tempK, sizeof(tempK));

                gettimeofday(&stop, NULL);

                validateRead(r1, r2, 1, fileDescriptor);

                double timePassed = stop.tv_sec - start.tv_sec + (stop.tv_usec - start.tv_usec) / 1000000.0;
                double ms = (stop.tv_sec - start.tv_sec) * 1000.0 + (stop.tv_usec - start.tv_usec) / 1000.0;

                int w1 = write(fileDescriptor, &type, sizeof(type));
                int w2 = write(fileDescriptor, &ms, sizeof(ms));
                int w3 = write(fileDescriptor, &bit_mask, sizeof(bit_mask));
                int w4 = write(fileDescriptor, &tempK, sizeof(tempK));
                validateWrite(w1, sizeof(type), w2, sizeof(ms), w3, sizeof(bit_mask), w4, sizeof(tempK), fileDescriptor);

                double tempC = tempK / 100.00 - 273.15;

                printState(timePassed, tempC, bit_mask);
            }
            else if (type == 0x0002)
            {
                uint8_t id;
                char text[13];

                int r1 = read(0, &id, sizeof(id));
                int r2 = read(0, text, sizeof(text));

                gettimeofday(&stop, NULL);

                validateRead(r1, r2, 1, fileDescriptor);

                double timePassed = stop.tv_sec - start.tv_sec + (stop.tv_usec - start.tv_usec) / 1000000.0;
                double ms = (stop.tv_sec - start.tv_sec) * 1000.0 + (stop.tv_usec - start.tv_usec) / 1000.0;

                int w1 = write(fileDescriptor, &type, sizeof(type));
                int w2 = write(fileDescriptor, &ms, sizeof(ms));
                int w3 = write(fileDescriptor, &id, sizeof(id));
                int w4 = write(fileDescriptor, &text, sizeof(text));
                validateWrite(w1, sizeof(type), w2, sizeof(ms), w3, sizeof(id), w4, sizeof(text), fileDescriptor);

                printSlotText(timePassed, id, text);
            }
        }

        close(fileDescriptor);
    }
    else if (strcmp(argv[1], "replay") == 0)
{
        int fileDescriptor = open(argv[2], O_RDONLY);
        validateOpen(fileDescriptor);

        uint16_t type;
        int rMain;

        double previous = 0.0;

        while ((rMain = read(fileDescriptor, &type, sizeof(type))))
        {
            validateRead(rMain, 1, 1, fileDescriptor);

            if (type == 0x0001)
            {
                uint16_t bit_mask;
                int tempK;
                double ms;

                int r1 = read(fileDescriptor, &ms, sizeof(ms));
                int r2 = read(fileDescriptor, &bit_mask, sizeof(bit_mask));
                int r3 = read(fileDescriptor, &tempK, sizeof(tempK));
                validateRead(r1, r2, r3, fileDescriptor);

                double currentTime = ms * 1000.0;
                double sleep_time = currentTime - previous;
                usleep(sleep_time);

                gettimeofday(&stop, NULL);
                double timePassed = stop.tv_sec - start.tv_sec + (stop.tv_usec - start.tv_usec) / 1000000.0;

                int w1 = write(1, &type, sizeof(type));
                int w2 = write(1, &bit_mask, sizeof(bit_mask));
                int w3 = write(1, &tempK, sizeof(tempK));
                validateWrite(w1, sizeof(type), w2, sizeof(bit_mask), w3, sizeof(tempK), 1, 1, fileDescriptor);

                previous = currentTime;

                double tempC = tempK / 100.00 - 273.15;

                printState(timePassed, tempC, bit_mask);
            }
            else if (type == 0x0002)
            {
                uint8_t id;
                char text[13];
                double ms;

                int r1 = read(fileDescriptor, &ms, sizeof(ms));
                int r2 = read(fileDescriptor, &id, sizeof(id));
                int r3 = read(fileDescriptor, text, sizeof(text));
                validateRead(r1, r2, r3, fileDescriptor);

                double currentTime = ms * 1000.0;
                double sleep_time = currentTime - previous;
                usleep(sleep_time);

                gettimeofday(&stop, NULL);
                double timePassed = stop.tv_sec - start.tv_sec + (stop.tv_usec - start.tv_usec) / 1000000.0;

                int w1 = write(1, &id, sizeof(id));
                int w2 = write(1, text, sizeof(text));
                validateWrite(w1, sizeof(id), w2, sizeof(text), 1, 1, 1, 1, fileDescriptor);

                previous = currentTime;

                printSlotText(timePassed, id, text);
            }
        }

        close(fileDescriptor);
    }
    else
    {
        err(3, "This is not a valid option! Use 'record' or 'replay' instead!");
        exit(3);
    }
}

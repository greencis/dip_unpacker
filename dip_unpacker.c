#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Definition of basic types */
typedef unsigned char byte;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;

inline byte readByte(FILE *file)
{
    byte b = 0;
    fread(&b, 1, 1, file);
    return b;
}

inline uint16 readUint16LE(FILE *file)
{
    uint16 x = readByte(file);
    return x | readByte(file) << 8;
}

inline uint32 readUint32LE(FILE *file)
{
    uint16 x = readUint16LE(file);
    return x | readUint16LE(file) << 16;
}

inline void writeByte(FILE *file, byte b)
{
    fwrite(&b, 1, 1, file);
}

inline void writeUint16LE(FILE *file, uint16 x)
{
    writeByte(file, x & 0xFF);
    writeByte(file, x >> 8);
}

inline void writeUint32LE(FILE *file, uint32 x)
{
    writeUint16LE(file, x & 0xFFFF);
    writeUint16LE(file, x >> 16);
}

inline void writeUint16BE(FILE *file, uint16 x)
{
    writeByte(file, x >> 8);
    writeByte(file, x & 0xFF);
}

inline void writeUint32BE(FILE *file, uint32 x)
{
    writeUint16BE(file, x >> 16);
    writeUint16BE(file, x & 0xFFFF);
}

int main(int argc, char **argv)
{
    FILE *fin, *fout;
    int i;
    uint32 signature, filesCount, archiveSize, type, size, offset;
    long oldOffset;
    char buf[80];
    void *dataPtr;

    fin = fopen("base.res", "rb");
    fseek(fin, 0L, SEEK_END);
    archiveSize = ftell(fin);
    rewind(fin);

    signature = readUint32LE(fin);
    if (signature != 0x36526150u)
    {
        exit(1);
    }

    filesCount = readUint32LE(fin) + 1;
    for (i = 0; i < filesCount; ++i)
    {
        type = readByte(fin);
        size = readUint32LE(fin);
        offset = readUint32LE(fin);
        sprintf(buf, "%02X_%05d_%08X_%08X", type, i, offset, size);

        if (offset + size > archiveSize) /* debug suffix "_warning" */
        {
            strcat(buf, "_warning");
            size = archiveSize - offset;
        }

        /* TODO: other types */
        if (type == 0x01u)
        {
            strcat(buf, ".txt");
        }
        else if (type == 0x08u)
        {
            strcat(buf, ".png");
        }
        else if (type == 0x09u || type == 0x0Cu)
        {
            strcat(buf, ".ogg");
        }
        else if (type == 0x0Bu)
        {
            strcat(buf, ".ogv");
        }

        if ((int32)size < 0)
        {
            fprintf(stderr, "WARNING: %d, file %d\n", (int32)size, i);
            size = 0;
        }

        oldOffset = ftell(fin);
        fseek(fin, offset, SEEK_SET);

        fout = fopen(buf, "wb");
        dataPtr = malloc(size);
        fread(dataPtr, 1, size, fin);
        fwrite(dataPtr, 1, size, fout);
        free(dataPtr);
        fclose(fout);

        fseek(fin, oldOffset, SEEK_SET);
    }

    fclose(fin);

    return 0;
}

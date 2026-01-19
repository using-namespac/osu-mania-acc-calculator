#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


struct NoteStat
{
    int16_t S300;
    int16_t S100;
    int16_t S50;
    int16_t S300g;
    int16_t S200;
    int16_t S0;
};

struct Score
{
    float V1;
    float V2;
};

bool ReadULEB128(FILE *file, __uint128_t *result)
{
    uint8_t buffer;

    if (1 != fread(&buffer, 1, 1, file))
        return true;

    if (buffer != 0x0b)
        return true;

    *result = 0;
    uint8_t offset = 0;

    while (true)
    {
        if (offset == 19)
            return true;

        if (1 != fread(&buffer, 1, 1, file))
            return true;

        *result |= (0b01111111 & buffer) << (offset * 7);

        const bool finished = (0b10000000 & buffer) == 0;

        if (finished)
            break;

        offset += 1;
    }

    return false;
}

bool ReadNoteStat(FILE *file, struct NoteStat *result)
{
    if (0 != fseek(file, 39, SEEK_CUR))
        return true;

    __uint128_t usernameSize;

    if (ReadULEB128(file, &usernameSize))
        return true;

    if (0 != fseek(file, (long) usernameSize, SEEK_CUR))
        return true;

    if (0 != fseek(file, 34, SEEK_CUR))
        return true;

    if (1 != fread(result, sizeof(struct NoteStat), 1, file))
        return true;

    return false;
}

bool ReadNoteStatFromPath(const char *path, struct NoteStat *result)
{
    FILE *file = fopen(path, "rb");

    if (file == (void*)0)
        return false;

    if (ReadNoteStat(file, result))
        return true;

    fclose(file);

    return false;
}

struct Score CalculateScore(const struct NoteStat x)
{
    return (struct Score)
    {
        .V1 = (float) (300 * (x.S300g + x.S300) + 200 * x.S200 + 100 * x.S100 + 50 * x.S50) /
              (float) (300 * (x.S300g + x.S300 + x.S200 + x.S100 + x.S50 + x.S0)),
        .V2 = (float) (305 * x.S300g + 300 * x.S300 + 200 * x.S200 + 100 * x.S100 + 50 * x.S50) /
              (float) (305 * (x.S300g + x.S300 + x.S200 + x.S100 + x.S50 + x.S0)),
    };
}

bool EndsWith(const char *str, const char *suffix)
{
    const size_t strLen = strlen(str);
    const size_t suffixLen = strlen(suffix);

    if (suffixLen > strLen)
        return false;

    const int comparison = strcmp(suffix, str + strLen - suffixLen);

    return comparison == 0;
}

int main(const int argc, char * argv[])
{
    DIR *dir = opendir(".");   //argv[1]

    if (dir == (void*)0)
    {
        puts("Not a directory.");
        return 2;
    }

    while (1)
    {
        const struct dirent *dirent = readdir(dir);

        if (dirent == (void*)0)
            break;

        if (!EndsWith(dirent->d_name, ".osr"))
            continue;

        struct NoteStat noteStat;

        if (ReadNoteStatFromPath(dirent->d_name, &noteStat))
        {
            printf("Error when reading %s", dirent->d_name);
            continue;
        }

        const struct Score score = CalculateScore(noteStat);

        puts(dirent->d_name);
        printf("V1: %.4f, V2: %.4f\n", score.V1 * 100, score.V2 * 100);
    }
    closedir(dir);
}

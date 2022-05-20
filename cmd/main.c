/*__attribute__((dllexport)) void RVExtension(char *output, int outputSize, const char *function);
__attribute__((dllexport)) int RVExtensionArgs(char *output, int outputSize, const char *function, const char **argv, int argc);
__attribute__((dllexport)) void RVExtensionVersion(char *output, int outputSize);*/

int strncpy_safe(char *output, const char *src, int size)
{
    int i;
    size--;
    for (i = 0; i < size && src[i] != '\0'; i++)
    {
        output[i] = src[i];
    }
    output[i] = '\0';
    return i;
}

extern void RVExtension(char *output, int outputSize, const char *function)
{
    strncpy_safe(output, function, outputSize);
}

extern int RVExtensionArgs(char *output, int outputSize, const char *function, const char **argv, int argc)
{
    int index = 0;
    int i;
    for (i = 0; i < argc && index < outputSize; i++)
    {
        index += strncpy_safe(output + index, argv[i], outputSize - 1 - index);
    }
    return 0;
}

extern void RVExtensionVersion(char *output, int outputSize)
{
    strncpy_safe(output, "Test-Extension v1.0", outputSize);
}

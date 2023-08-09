void
debug_log(const char *log, ...)
{
    va_list args;
    va_start(args, log);
    vfprintf(stderr, log, args);
    fputc('\n', stderr);
    va_end(args);
}

void
warning(const char *warning, ...)
{
    va_list args;
    va_start(args, warning);
    fprintf(stderr, "warning: ");
    vfprintf(stderr, warning, args);
    fputc('\n', stderr);
    va_end(args);
}

void
error(const char *error, ...)
{
    va_list args;
    va_start(args, error);
    fprintf(stderr, "error: ");
    vfprintf(stderr, error, args);
    fputc('\n', stderr);
    va_end(args);
}

void
important_warning(const char *warning, ...)
{
    va_list args;
    va_start(args, warning);
    vsnprintf(mbText, sizeof(mbText), warning, args);
    va_end(args);
    if (isalpha(*mbText))
        *mbText = toupper(*mbText);
    MessageBox(NULL, mbText, TEXT("Warning"),
               MB_ICONWARNING | MB_OK);
}

void
fatal_error(const char *error, ...)
{
    va_list args;
    va_start(args, error);
    vsnprintf(mbText, sizeof(mbText), error, args);
    va_end(args);
    if (isalpha(*mbText))
        *mbText = toupper(*mbText);
    MessageBox(NULL, mbText, TEXT("Fatal Error"),
               MB_ICONERROR | MB_OK);
    ExitProcess(EXIT_FAILURE);
}

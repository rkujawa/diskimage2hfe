char *strdup(const char *s);
short simpleswab16(short from);
#ifdef NEED_SWAB
void swab(const void *from, void *to, ssize_t n);
#endif

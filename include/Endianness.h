#ifndef CPU_ENDIANNESS_CHECK_H_
#define CPU_ENDIANNESS_CHECK_H_ 1

#define LittleEndian() !BigEndian()

inline static unsigned char BigEndian(void)
{
    const unsigned int value = 1;
    return (unsigned char)(*(char *)&value) == 0;
}

#endif

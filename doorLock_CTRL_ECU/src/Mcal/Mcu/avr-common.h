#ifndef __AVR_COMMON_H__
#define __AVR_COMMON_H__

#ifndef _VECTOR
#define _VECTOR(N) __vector_ ## N
#endif

#if (__GNUC__ == 4 && __GNUC_MINOR__ >= 1) || (__GNUC__ > 4)
#  define __INTR_ATTRS used, externally_visible
#else /* GCC < 4.1 */
#  define __INTR_ATTRS used
#endif

#ifdef __cplusplus
#  define ISR(vector, ...)            \
    extern "C" void vector (void) __attribute__ ((signal,__INTR_ATTRS)) __VA_ARGS__; \
    void vector (void)
#else
#  define ISR(vector, ...)            \
    void vector (void) __attribute__ ((signal,__INTR_ATTRS)) __VA_ARGS__; \
    void vector (void)
#endif

#ifdef __cplusplus
#  define SIGNAL(vector)					\
    extern "C" void vector(void) __attribute__ ((signal, __INTR_ATTRS));	\
    void vector (void)
#else
#  define SIGNAL(vector)					\
    void vector (void) __attribute__ ((signal, __INTR_ATTRS));		\
    void vector (void)
#endif

#ifdef __cplusplus
#  define EMPTY_INTERRUPT(vector)                \
    extern "C" void vector(void) __attribute__ ((signal,naked,__INTR_ATTRS));    \
    void vector (void) {  __asm__ __volatile__ ("reti" ::); }
#else
#  define EMPTY_INTERRUPT(vector)                \
    void vector (void) __attribute__ ((signal,naked,__INTR_ATTRS));    \
    void vector (void) { __asm__ __volatile__ ("reti" ::); }
#endif

#ifdef __cplusplus
#  if defined(__AVR_MEGA__) && __AVR_MEGA__
#    define ISR_ALIAS(vector, tgt) extern "C" void vector (void) \
	__attribute__((signal, naked, __INTR_ATTRS)); \
	void vector (void) { asm volatile ("jmp " __STRINGIFY(tgt) ::); }
#  else /* !__AVR_MEGA */
#    define ISR_ALIAS(vector, tgt) extern "C" void vector (void) \
	__attribute__((signal, naked, __INTR_ATTRS)); \
	void vector (void) { asm volatile ("rjmp " __STRINGIFY(tgt) ::); }
#  endif  /* __AVR_MEGA__ */
#else	  /* !__cplusplus */
#  if defined(__AVR_MEGA__) && __AVR_MEGA__
#  define ISR_ALIAS(vector, tgt) void vector (void) \
	__attribute__((signal, naked, __INTR_ATTRS)); \
	void vector (void) { asm volatile ("jmp " __STRINGIFY(tgt) ::); }
#  else /* !__AVR_MEGA */
#  define ISR_ALIAS(vector, tgt) void vector (void) \
	__attribute__((signal, naked, __INTR_ATTRS)); \
	void vector (void) { asm volatile ("rjmp " __STRINGIFY(tgt) ::); }
#  endif  /* __AVR_MEGA__ */
#endif	/* __cplusplus */

#  define reti()  __asm__ __volatile__ ("reti" ::)

#  define BADISR_vect __vector_default

#  define ISR_BLOCK
#  define ISR_NOBLOCK    __attribute__((interrupt))
#  define ISR_NAKED      __attribute__((naked))
#  define ISR_ALIASOF(v) __attribute__((alias(__STRINGIFY(v))))


#endif /* __AVR_COMMON_H__ */

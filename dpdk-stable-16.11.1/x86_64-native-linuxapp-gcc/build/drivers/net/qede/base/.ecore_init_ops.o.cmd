cmd_base/ecore_init_ops.o = gcc -Wp,-MD,base/.ecore_init_ops.o.d.tmp  -m64 -pthread  -march=native -DRTE_MACHINE_CPUFLAG_SSE -DRTE_MACHINE_CPUFLAG_SSE2 -DRTE_MACHINE_CPUFLAG_SSE3 -DRTE_MACHINE_CPUFLAG_SSSE3 -DRTE_MACHINE_CPUFLAG_SSE4_1 -DRTE_MACHINE_CPUFLAG_SSE4_2 -DRTE_MACHINE_CPUFLAG_AES -DRTE_MACHINE_CPUFLAG_PCLMULQDQ -DRTE_MACHINE_CPUFLAG_AVX -DRTE_MACHINE_CPUFLAG_RDRAND -DRTE_MACHINE_CPUFLAG_FSGSBASE -DRTE_MACHINE_CPUFLAG_F16C -DRTE_MACHINE_CPUFLAG_AVX2  -I/home/felix/projects/edition/dpdk-stable-16.11.1/x86_64-native-linuxapp-gcc/include -include /home/felix/projects/edition/dpdk-stable-16.11.1/x86_64-native-linuxapp-gcc/include/rte_config.h -O3 -W -Wall -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wold-style-definition -Wpointer-arith -Wcast-align -Wnested-externs -Wcast-qual -Wformat-nonliteral -Wformat-security -Wundef -Wwrite-strings -Wno-unused-parameter -Wno-sign-compare -Wno-missing-prototypes -Wno-cast-qual -Wno-unused-function -Wno-unused-variable -Wno-strict-aliasing -Wno-missing-prototypes -Wno-unused-value -Wno-format-nonliteral -Wno-unused-but-set-variable -Wno-missing-declarations -Wno-maybe-uninitialized -Wno-strict-prototypes -Wno-unused-parameter -Wno-sign-compare -Wno-missing-prototypes -Wno-cast-qual -Wno-unused-function -Wno-unused-variable -Wno-strict-aliasing -Wno-missing-prototypes -Wno-unused-value -Wno-format-nonliteral -Wno-unused-but-set-variable -Wno-missing-declarations -Wno-maybe-uninitialized -Wno-strict-prototypes -Wno-unused-parameter -Wno-sign-compare -Wno-missing-prototypes -Wno-cast-qual -Wno-unused-function -Wno-unused-variable -Wno-strict-aliasing -Wno-missing-prototypes -Wno-unused-value -Wno-format-nonliteral -Wno-unused-but-set-variable -Wno-missing-declarations -Wno-maybe-uninitialized -Wno-strict-prototypes -Wno-unused-parameter -Wno-sign-compare -Wno-missing-prototypes -Wno-cast-qual -Wno-unused-function -Wno-unused-variable -Wno-strict-aliasing -Wno-missing-prototypes -Wno-unused-value -Wno-format-nonliteral -Wno-unused-but-set-variable -Wno-missing-declarations -Wno-maybe-uninitialized -Wno-strict-prototypes -Wno-unused-parameter -Wno-sign-compare -Wno-missing-prototypes -Wno-cast-qual -Wno-unused-function -Wno-unused-variable -Wno-strict-aliasing -Wno-missing-prototypes -Wno-unused-value -Wno-format-nonliteral -Wno-unused-but-set-variable -Wno-missing-declarations -Wno-maybe-uninitialized -Wno-strict-prototypes -Wno-unused-parameter -Wno-sign-compare -Wno-missing-prototypes -Wno-cast-qual -Wno-unused-function -Wno-unused-variable -Wno-strict-aliasing -Wno-missing-prototypes -Wno-unused-value -Wno-format-nonliteral -Wno-unused-but-set-variable -Wno-missing-declarations -Wno-maybe-uninitialized -Wno-strict-prototypes -Wno-unused-parameter -Wno-sign-compare -Wno-missing-prototypes -Wno-cast-qual -Wno-unused-function -Wno-unused-variable -Wno-strict-aliasing -Wno-missing-prototypes -Wno-unused-value -Wno-format-nonliteral -Wno-unused-but-set-variable -Wno-missing-declarations -Wno-maybe-uninitialized -Wno-strict-prototypes -Wno-unused-parameter -Wno-sign-compare -Wno-missing-prototypes -Wno-cast-qual -Wno-unused-function -Wno-unused-variable -Wno-strict-aliasing -Wno-missing-prototypes -Wno-unused-value -Wno-format-nonliteral -Wno-unused-but-set-variable -Wno-missing-declarations -Wno-maybe-uninitialized -Wno-strict-prototypes -Wno-unused-parameter -Wno-sign-compare -Wno-missing-prototypes -Wno-cast-qual -Wno-unused-function -Wno-unused-variable -Wno-strict-aliasing -Wno-missing-prototypes -Wno-unused-value -Wno-format-nonliteral -Wno-unused-but-set-variable -Wno-missing-declarations -Wno-maybe-uninitialized -Wno-strict-prototypes -Wno-unused-parameter -Wno-sign-compare -Wno-missing-prototypes -Wno-cast-qual -Wno-unused-function -Wno-unused-variable -Wno-strict-aliasing -Wno-missing-prototypes -Wno-unused-value -Wno-format-nonliteral -Wno-unused-but-set-variable -Wno-missing-declarations -Wno-maybe-uninitialized -Wno-strict-prototypes -Wno-unused-parameter -Wno-sign-compare -Wno-missing-prototypes -Wno-cast-qual -Wno-unused-function -Wno-unused-variable -Wno-strict-aliasing -Wno-missing-prototypes -Wno-unused-value -Wno-format-nonliteral -Wno-unused-but-set-variable -Wno-missing-declarations -Wno-maybe-uninitialized -Wno-strict-prototypes -Wno-unused-parameter -Wno-sign-compare -Wno-missing-prototypes -Wno-cast-qual -Wno-unused-function -Wno-unused-variable -Wno-strict-aliasing -Wno-missing-prototypes -Wno-unused-value -Wno-format-nonliteral -Wno-unused-but-set-variable -Wno-missing-declarations -Wno-maybe-uninitialized -Wno-strict-prototypes -Wno-unused-parameter -Wno-sign-compare -Wno-missing-prototypes -Wno-cast-qual -Wno-unused-function -Wno-unused-variable -Wno-strict-aliasing -Wno-missing-prototypes -Wno-unused-value -Wno-format-nonliteral -Wno-unused-but-set-variable -Wno-missing-declarations -Wno-maybe-uninitialized -Wno-strict-prototypes -Wno-unused-parameter -Wno-sign-compare -Wno-missing-prototypes -Wno-cast-qual -Wno-unused-function -Wno-unused-variable -Wno-strict-aliasing -Wno-missing-prototypes -Wno-unused-value -Wno-format-nonliteral -Wno-unused-but-set-variable -Wno-missing-declarations -Wno-maybe-uninitialized -Wno-strict-prototypes    -o base/ecore_init_ops.o -c /home/felix/projects/edition/dpdk-stable-16.11.1/drivers/net/qede/base/ecore_init_ops.c 

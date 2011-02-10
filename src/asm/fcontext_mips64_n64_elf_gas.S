/*
            Copyright Oliver Kowalke 2009.
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
          http://www.boost.org/LICENSE_1_0.txt)
*/

/*******************************************************************
 *                                                                 *
 *  -------------------------------------------------------------  *
 *  |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  *
 *  -------------------------------------------------------------  *
 *  |  0  |  8  |  16 |  24 |  32 |  40 |  48 |  56 |  64 |  72 |  *
 *  -------------------------------------------------------------  *
 *  |  S0 |  S1 |  S2 |  S3 |  S4 |  S5 |  S6 |  S7 |  GP |  SP |  *
 *  -------------------------------------------------------------  *
 *  -------------------------------------------------------------  *
 *  |  10 |  11 |  12 |  13 |                                   |  *
 *  -------------------------------------------------------------  *
 *  |  80 |  88 |  96 | 104 |                                   |  *
 *  -------------------------------------------------------------  *
 *  |  S8 |  RA |  PC |  A0 |                                   |  *
 *  -------------------------------------------------------------  *
 *  -------------------------------------------------------------  *
 *  |  14 |  15 |  16 |  17 |  18 |  19 |  20 |  21 |           |  *
 *  -------------------------------------------------------------  *
 *  | 112 | 120 | 128 | 136 | 144 | 152 | 160 | 168 |           |  *
 *  -------------------------------------------------------------  *
 *  | F24 | F25 | F26 | F27 | F28 | F29 | F30 | F31 |           |  *
 *  -------------------------------------------------------------  *
 *  -------------------------------------------------------------  *
 *  |  22 |  23 |  24 |                                         |  *
 *  -------------------------------------------------------------  *
 *  | 176 | 184 | 192 |                                         |  *
 *  -------------------------------------------------------------  *
 *  |sbase|slimt|flink|                                         |  *
 *  -------------------------------------------------------------  *
 *                                                                 *
 * *****************************************************************/

.text
.globl get_fcontext
.align 2
.type get_fcontext,@function
.ent get_fcontext
get_fcontext:
    sd      $s0, ($a0)      # save S0
    sd      $s1, 8($a0)     # save S1
    sd      $s2, 16($a0)    # save S2
    sd      $s3, 24($a0)    # save S3
    sd      $s4, 32($a0)    # save S4
    sd      $s5, 40($a0)    # save S5
    sd      $s6, 48($a0)    # save S6
    sd      $s7, 56($a0)    # save S7
    sd      $gp, 64($a0)    # save GP
    sd      $sp, 72($a0)    # save SP
    sd      $s8, 80($a0)    # save S8
    sd      $ra, 88($a0)    # save RA
    sd      $ra, 96($a0)    # save RA as PC

#if defined(__mips_hard_float)
    s.d     $f24, 112($a0)  # save F24
    s.d     $f25, 120($a0)  # save F25
    s.d     $f26, 128($a0)  # save F26
    s.d     $f27, 136($a0)  # save F27
    s.d     $f28, 144($a0)  # save F28
    s.d     $f29, 152($a0)  # save F29
    s.d     $f30, 160($a0)  # save F30
    s.d     $f31, 168($a0)  # save F31
#endif

    move    $v0, $zero      # return value set to zero
    j       $ra             # return to caller
    nop
.end get_fcontext
.size get_fcontext, .-get_fcontext

.text
.globl set_fcontext
.align 2
.type set_fcontext,@function
.ent set_fcontext
set_fcontext:
    move    $t0, $a0        # let T0 point to fcontext
    ld      $s0, ($t0)      # restore S0
    ld      $s1, 8($t0)     # restore S1
    ld      $s2, 16($t0)    # restore S2
    ld      $s3, 24($t0)    # restore S3
    ld      $s4, 32($t0)    # restore S4
    ld      $s5, 40($t0)    # restore S5
    ld      $s6, 48($t0)    # restore S6
    ld      $s7, 56($t0)    # restore S7
    ld      $gp, 64($t0)    # restore GP
    ld      $sp, 72($t0)    # restore SP
    ld      $s8, 80($t0)    # restore S8
    ld      $ra, 88($t0)    # restore RA
    ld      $a0, 96($t0)    # restore void pointer as argument

#if defined(__mips_hard_float)
    l.d     $f24, 112($t0)  # restore F24
    l.d     $f25, 120($t0)  # restore F25
    l.d     $f26, 128($t0)  # restore F26
    l.d     $f27, 136($t0)  # restore F27
    l.d     $f28, 144($t0)  # restore F28
    l.d     $f29, 152($t0)  # restore F29
    l.d     $f30, 160($t0)  # restore F30
    l.d     $f31, 168($t0)  # restore F31
#endif

    ld      $t9, 96($t0)    # restore PC
    jr      $t9             # jump to context
    nop
.end set_fcontext
.size set_fcontext, .-set_fcontext

.text
.globl swap_fcontext
.align 2
.type swap_fcontext,@function
.ent swap_fcontext
swap_fcontext:
    sd      $s0, ($a0)      # save S0
    sd      $s1, 8($a0)     # save S1
    sd      $s2, 16($a0)    # save S2
    sd      $s3, 24($a0)    # save S3
    sd      $s4, 32($a0)    # save S4
    sd      $s5, 40($a0)    # save S5
    sd      $s6, 48($a0)    # save S6
    sd      $s7, 56($a0)    # save S7
    sd      $gp, 64($a0)    # save GP
    sd      $sp, 72($a0)    # save SP
    sd      $s8, 80($a0)    # save S8
    sd      $ra, 88($a0)    # save RA
    sd      $ra, 96($a0)    # save RA as PC

#if defined(__mips_hard_float)
    s.d     $f24, 112($a0)  # save F24
    s.d     $f25, 120($a0)  # save F25
    s.d     $f26, 128($a0)  # save F26
    s.d     $f27, 136($a0)  # save F27
    s.d     $f28, 144($a0)  # save F28
    s.d     $f29, 152($a0)  # save F29
    s.d     $f30, 160($a0)  # save F30
    s.d     $f31, 168($a0)  # save F31
#endif


    ld      $s0, ($a1)      # restore S0
    ld      $s1, 8($a1)     # restore S1
    ld      $s2, 16($a1)    # restore S2
    ld      $s3, 24($a1)    # restore S3
    ld      $s4, 32($a1)    # restore S4
    ld      $s5, 40($a1)    # restore S5
    ld      $s6, 48($a1)    # restore S6
    ld      $s7, 56($a1)    # restore S7
    ld      $gp, 64($a1)    # restore GP
    ld      $sp, 72($a1)    # restore SP
    ld      $s8, 80($a1)    # restore S8
    ld      $ra, 88($a1)    # restore RA
    ld      $a0, 96($a1)    # restore void pointer as argument

#if defined(__mips_hard_float)
    l.d     $f24, 112($a1)  # restore F24
    l.d     $f25, 120($a1)  # restore F25
    l.d     $f26, 128($a1)  # restore F26
    l.d     $f27, 136($a1)  # restore F27
    l.d     $f28, 144($a1)  # restore F28
    l.d     $f29, 152($a1)  # restore F29
    l.d     $f30, 160($a1)  # restore F30
    l.d     $f31, 168($a1)  # restore F31
#endif

    ld      $t9, 96($a1)    # restore PC
    jr      $t9             # jump to context
    nop
.end swap_fcontext
.size swap_fcontext, .-swap_fcontext

.text
.globl make_fcontext
.align 2
.type make_fcontext,@function
.ent make_fcontext
make_fcontext:
    sd      $a1, 96($a0)        # save the address of the function supposed to be run
    sd      $a2, 104($a0)       # save void pointer
    ld      $t0, 176($a0)       # load the stack base
    sd      $t0, 72($a0)        # save the stack base
    ld      $t0, 192($a0)       # load address of next context
    sd      $t0, ($a0)          # save the next context
    la      $t0, link_fcontext  # load address of link_fcontext
    sd      $t0, 88($a0)        # save address of link_fcontext

    move    $v0, $zero
    j       $ra
    nop

link_fcontext:
    move    $a0, $s0
    beq     $a0, $zero, finish  # test if a next context was given
    la      $t0, set_fcontext   # install next context
    jr      $t0
    nop

finish:
    la      $t0, _exit          # exit application
    jr      $t0
    nop
.end make_fcontext
.size make_fcontext, .-make_fcontext

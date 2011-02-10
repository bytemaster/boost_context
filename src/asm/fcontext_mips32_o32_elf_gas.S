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
 *  |  14 |  15 |  16 |  17 |  18 |  19 |                       |  *
 *  -------------------------------------------------------------  *
 *  | 112 | 120 | 128 | 136 | 144 | 152 |                       |  *
 *  -------------------------------------------------------------  *
 *  | F20 | F22 | F24 | F26 | F28 | F30 |                       |  *
 *  -------------------------------------------------------------  *
 *  -------------------------------------------------------------  *
 *  |  20 |  21 |  22 |                                         |  *
 *  -------------------------------------------------------------  *
 *  | 160 | 164 | 168 |                                         |  *
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
    sw      $s0, ($a0)      # save S0
    sw      $s1, 8($a0)     # save S1
    sw      $s2, 16($a0)    # save S2
    sw      $s3, 24($a0)    # save S3
    sw      $s4, 32($a0)    # save S4
    sw      $s5, 40($a0)    # save S5
    sw      $s6, 48($a0)    # save S6
    sw      $s7, 56($a0)    # save S7
    sw      $gp, 64($a0)    # save GP
    sw      $sp, 72($a0)    # save SP
    sw      $s8, 80($a0)    # save S8
    sw      $ra, 88($a0)    # save RA
    sw      $ra, 96($a0)    # save RA as PC

#if defined(__mips_hard_float)
    s.d     $f20, 112($a0)  # save F20
    s.d     $f22, 120($a0)  # save F22
    s.d     $f24, 128($a0)  # save F24
    s.d     $f26, 136($a0)  # save F26
    s.d     $f28, 144($a0)  # save F28
    s.d     $f30, 152($a0)  # save F30
#endif

    move    $v0, $zero      # return value set to zero
    j       $ra             # return to caller
.end get_fcontext
.size get_fcontext, .-get_fcontext

.text
.globl set_fcontext
.align 2
.type set_fcontext,@function
.ent set_fcontext
set_fcontext:
    move    $t0, $a0        # let T0 point to fcontext
    lw      $s0, ($t0)      # restore S0
    lw      $s1, 8($t0)     # restore S1
    lw      $s2, 16($t0)    # restore S2
    lw      $s3, 24($t0)    # restore S3
    lw      $s4, 32($t0)    # restore S4
    lw      $s5, 40($t0)    # restore S5
    lw      $s6, 48($t0)    # restore S6
    lw      $s7, 56($t0)    # restore S7
    lw      $gp, 64($t0)    # restore GP
    lw      $sp, 72($t0)    # restore SP
    lw      $s8, 80($t0)    # restore S8
    lw      $ra, 88($t0)    # restore RA
    lw      $a0, 104($t0)   # restore void pointer as argument

#if defined(__mips_hard_float)
    l.d     $f20, 112($t0)  # restore F20
    l.d     $f22, 120($t0)  # restore F22
    l.d     $f24, 128($t0)  # restore F24
    l.d     $f26, 136($t0)  # restore F26
    l.d     $f28, 144($t0)  # restore F28
    l.d     $f30, 152($t0)  # restore F30
#endif

    lw      $t9, 96($t0)    # load PC
    jr      $t9             # jump to context
.end set_fcontext
.size set_fcontext, .-set_fcontext

.text
.globl swap_fcontext
.align 2
.type swap_fcontext,@function
.ent swap_fcontext
swap_fcontext:
    sw      $s0, ($a0)      # save S0
    sw      $s1, 8($a0)     # save S1
    sw      $s2, 16($a0)    # save S2
    sw      $s3, 24($a0)    # save S3
    sw      $s4, 32($a0)    # save S4
    sw      $s5, 40($a0)    # save S5
    sw      $s6, 48($a0)    # save S6
    sw      $s7, 56($a0)    # save S7
    sw      $gp, 64($a0)    # save GP
    sw      $sp, 72($a0)    # save SP
    sw      $s8, 80($a0)    # save S8
    sw      $ra, 88($a0)    # save RA
    sw      $ra, 96($a0)    # save RA as PC

#if defined(__mips_hard_float)
    s.d     $f20, 112($a0)  # save F20
    s.d     $f22, 120($a0)  # save F22
    s.d     $f24, 128($a0)  # save F24
    s.d     $f26, 136($a0)  # save F26
    s.d     $f28, 144($a0)  # save F28
    s.d     $f30, 152($a0)  # save F30
#endif


    lw      $s0, ($a1)      # restore S0
    lw      $s1, 8($a1)     # restore S1
    lw      $s2, 16($a1)    # restore S2
    lw      $s3, 24($a1)    # restore S3
    lw      $s4, 32($a1)    # restore S4
    lw      $s5, 40($a1)    # restore S5
    lw      $s6, 48($a1)    # restore S6
    lw      $s7, 56($a1)    # restore S7
    lw      $gp, 64($a1)    # restore GP
    lw      $sp, 72($a1)    # restore SP
    lw      $s8, 80($a1)    # restore S8
    lw      $ra, 88($a1)    # restore RA
    lw      $a0, 104($a1)   # restore void pointer as argument

#if defined(__mips_hard_float)
    l.d     $f20, 112($a1)  # restore F20
    l.d     $f22, 120($a1)  # restore F22
    l.d     $f24, 128($a1)  # restore F24
    l.d     $f26, 136($a1)  # restore F26
    l.d     $f28, 144($a1)  # restore F28
    l.d     $f30, 152($a1)  # restore F30
#endif

    lw      $t9, 96($a1)    # load PC
    jr      $t9             # jump to context
.end swap_fcontext
.size swap_fcontext, .-swap_fcontext

.text
.globl make_fcontext
.align 2
.type make_fcontext,@function
.ent make_fcontext
make_fcontext:
    sw      $a1, 96($a0)        # save the address of the function supposed to be run
    sw      $a2, 104($a0)       # save void pointer
    lw      $t0, 160($a0)       # load the stack base
    addiu   $t0, $t0, -16       # reserve 16 byte of argument space
    sw      $t0, 72($a0)        # save the stack base
    lw      $t0, 168($a0)       # load address of next user context
    sw      $t0, ($a0)          # save the next context
    la      $t0, link_fcontext  # get address of link_fcontext
    sw      $t0, 88($a0)        # save address of link_fcontext

    move    $v0, $zero
    j       $ra

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

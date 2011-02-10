/*
            Copyright Oliver Kowalke 2009.
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
*/

/****************************************************************************************
 *                                                                                      *
 *  ----------------------------------------------------------------------------------  *
 *  |    0    |    1    |    2    |    3    |    4     |    5    |    6    |    7    |  *
 *  ----------------------------------------------------------------------------------  *
 *  |   0x0   |   0x4   |   0x8   |   0xc   |   0x10   |   0x14  |   0x18  |   0x1c  |  *
 *  ----------------------------------------------------------------------------------  *
 *  |        RBX        |        R12        |         R13        |        R14        |  *
 *  ----------------------------------------------------------------------------------  *
 *  ----------------------------------------------------------------------------------  *
 *  |    8    |    9    |   10    |   11    |    12    |    13   |    14   |    15   |  *
 *  ----------------------------------------------------------------------------------  *
 *  |   0x20  |   0x24  |   0x28  |  0x2c   |   0x30   |   0x34  |   0x38  |   0x3c  |  *
 *  ----------------------------------------------------------------------------------  *
 *  |        R15        |        RBP        |         RSP        |        RIP        |  *
 *  ----------------------------------------------------------------------------------  *
 *  ----------------------------------------------------------------------------------  *
 *  |    16   |    17   |                                                            |  *
 *  ----------------------------------------------------------------------------------  *
 *  |   0x40  |   0x44  |                                                            |  *
 *  ----------------------------------------------------------------------------------  *
 *  |        RDI        |                                                            |  *
 *  ----------------------------------------------------------------------------------  *
 *  ----------------------------------------------------------------------------------  *
 *  |   18    |   19    |                                                            |  *
 *  ----------------------------------------------------------------------------------  *
 *  |  0x48   |  0x4c   |                                                            |  *
 *  ----------------------------------------------------------------------------------  *
 *  | fc_mxcsr|fc_x87_cw|                                                            |  *
 *  ----------------------------------------------------------------------------------  *
 *  ----------------------------------------------------------------------------------  *
 *  |   20    |    21   |    22    |   23    |    24   |    25   |                   |  *
 *  ----------------------------------------------------------------------------------  *
 *  |  0x50   |   0x54  |   0x58   |  0x5c   |   0x60  |   0x64  |                   |  *
 *  ----------------------------------------------------------------------------------  *
 *  |       sbase       |        slimit      |      fc_link      |                   |  *
 *  ----------------------------------------------------------------------------------  *
 *                                                                                      *
 * **************************************************************************************/

.globl get_fcontext
get_fcontext:
    movq     %rbx,       (%rdi)         /* save RBX */
    movq     %r12,       0x8(%rdi)      /* save R12 */
    movq     %r13,       0x10(%rdi)     /* save R13 */
    movq     %r14,       0x18(%rdi)     /* save R14 */
    movq     %r15,       0x20(%rdi)     /* save R15 */
    movq     %rbp,       0x28(%rdi)     /* save RBP */

    stmxcsr  0x48(%rdi)                 /* save SSE2 control and status word */
    fnstcw   0x4c(%rdi)                 /* save x87 control word */

    leaq     0x8(%rsp),  %rcx           /* exclude the return address */
    movq     %rcx,       0x30(%rdi)     /* save as stack pointer */
    movq     (%rsp),     %rcx           /* load return address */
    movq     %rcx,       0x38(%rdi)     /* save return address */

    xorq     %rax,       %rax           /* set RAX to zero */
    ret                                 /* return */

.globl set_fcontext
set_fcontext:
    movq     (%rdi),      %rbx      /* restore RBX */
    movq     0x8(%rdi),   %r12      /* restore R12 */
    movq     0x10(%rdi),  %r13      /* restore R13 */
    movq     0x18(%rdi),  %r14      /* restore R14 */
    movq     0x20(%rdi),  %r15      /* restore R15 */
    movq     0x28(%rdi),  %rbp      /* restore RBP */

    ldmxcsr  0x48(%rdi)             /* restore SSE2 control and status word */
    fldcw    0x4c(%rdi)             /* restore x87 control word */

    movq     0x30(%rdi),  %rsp      /* restore RSP */
    movq     0x38(%rdi),  %rcx      /* fetch the address to return to */
    pushq    %rcx                   /* push the return address on the new stack so we can return there */
    movq     0x40(%rdi),  %rdi      /* restore RDI first argument for calledcontext */

    xorq     %rax,        %rax      /* set RAX to zero */
    ret                             /* 'ret' will pop the return address of the code and jump to it */

.globl swap_fcontext
swap_fcontext:
    movq     %rbx,       (%rdi)         /* save RBX */
    movq     %r12,       0x8(%rdi)      /* save R12 */
    movq     %r13,       0x10(%rdi)     /* save R13 */
    movq     %r14,       0x18(%rdi)     /* save R14 */
    movq     %r15,       0x20(%rdi)     /* save R15 */
    movq     %rbp,       0x28(%rdi)     /* save RBP */

    stmxcsr  0x48(%rdi)                 /* save SSE2 control and status word */
    fnstcw   0x4c(%rdi)                 /* save x87 control word */

    leaq     0x8(%rsp),  %rcx           /* exclude the return address and save as stack pointer */
    movq     %rcx,       0x30(%rdi)     /* save as stack pointer */
    movq     (%rsp),     %rcx           /* save return address */
    movq     %rcx,       0x38(%rdi)     /* save return address as RIP */


    movq     (%rsi),      %rbx      /* restore RBX */
    movq     0x8(%rsi),   %r12      /* restore R12 */
    movq     0x10(%rsi),  %r13      /* restore R13 */
    movq     0x18(%rsi),  %r14      /* restore R14 */
    movq     0x20(%rsi),  %r15      /* restore R15 */
    movq     0x28(%rsi),  %rbp      /* restore RBP */

    ldmxcsr  0x48(%rsi)             /* restore SSE2 control and status word */
    fldcw    0x4c(%rsi)             /* restore x87 control word */

    movq     0x30(%rsi),  %rsp      /* restore RSP */
    movq     0x38(%rsi),  %rcx      /* fetch the address to return to */
    pushq    %rcx                   /* push the return address on the new stack so we can return there */
    movq     0x40(%rsi),  %rdi      /* restore RDI */

    xorq     %rax,        %rax      /* set RAX to zero */
    ret                             /* 'ret' will pop the return address of the code and jump to it */

.globl make_fcontext
make_fcontext:
    movq   %rsi,                 0x38(%rdi) /* save the address of the function supposed to run */
    movq   %rdx,                 0x40(%rdi) /* save the the void pointer */
    movq   0x50(%rdi),           %rdx       /* load the stack base */
    leaq   -0x10(%rdx),          %rdx       /* reserve space for return address, must be 16 byte border */
    movq   %rdx,                 0x30(%rdi) /* save the address */
    movq   0x60(%rdi),           %rcx       /* load the address of the next context */
    movq   %rcx,                 (%rdi)     /* save the address of next context */
    leaq   link_fcontext(%rip),  %rcx       /* helper code executed after fn() returns */
    movq   %rcx,                 (%rdx)
    xorq   %rax,                 %rax       /* set RAX to zero */
    ret                                     /* return 0 */

link_fcontext:
    movq   %rbx,               %rdi
    testq  %rdi,               %rdi         /* test if a next context was given */
    je     finish                           /* jump to finish */
    call   set_fcontext                     /* install next context */

finish:
    call   _exit                            /* exit application */

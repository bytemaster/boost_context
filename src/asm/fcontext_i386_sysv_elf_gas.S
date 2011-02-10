/*
            Copyright Oliver Kowalke 2009.
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
          http://www.boost.org/LICENSE_1_0.txt)
*/

/********************************************************************
 *                                                                  *
 *  --------------------------------------------------------------  *
 *  |    0    |    1    |    2    |    3    |    4     |    5    |  *
 *  --------------------------------------------------------------  *
 *  |   0x0   |   0x4   |   0x8   |   0xc   |   0x10   |   0x14  |  *
 *  --------------------------------------------------------------  *
 *  |   EBX   |   EBP   |   EDI   |   ESI   |   ESP    |   EIP   |  *
 *  --------------------------------------------------------------  *
 *  --------------------------------------------------------------  *
 *  |    6    |                                                  |  *
 *  -------------------i------------------------------------------  *
 *  |   0x18  |                                                  |  *
 *  --------------------------------------------------------------  *
 *  |fc_x87_cw|                                                  |  *
 *  --------------------------------------------------------------  *
 *  --------------------------------------------------------------  *
 *  |    7    |    8    |    9    |                              |  *
 *  --------------------------------------------------------------  *
 *  |  0x1c   |   0x20  |   0x24  |                              |  *
 *  --------------------------------------------------------------  *
 *  |  sbase  | slimit  | fc_link |                              |  *
 *  --------------------------------------------------------------  *
 *                                                                  *
 * *****************************************************************/

.text
.globl get_fcontext
.align 2
.type get_fcontext,@function
get_fcontext:
    movl    0x4(%esp),  %eax        /* load address of the fcontext_t arg */
    movl    %ebx,       (%eax)      /* save EBX */
    movl    %ebp,       0x4(%eax)   /* save EBP */
    movl    %edi,       0x8(%eax)   /* save EDI */
    movl    %esi,       0xc(%eax)   /* save ESI */

    fnstcw  0x18(%eax)              /* save x87 control word */

    leal    0x4(%esp),  %ecx        /* exclude the return address */
    movl    %ecx,       0x10(%eax)  /* save as stack pointer */
    movl    (%esp),     %ecx        /* load return address */
    movl    %ecx,       0x14(%eax)  /* save return address */

    xorl    %eax,       %eax        /* set return value to zero */
    ret                             /* return */
.size get_fcontext,.-get_fcontext

.text
.globl set_fcontext
.align 2
.type set_fcontext,@function
set_fcontext:
    movl   0x4(%esp),   %eax        /* load address of the fcontext_t arg */
    movl   (%eax),      %ebx        /* restore EBX */
    movl   0x4(%eax),   %ebp        /* restore EBP */
    movl   0x8(%eax),   %edi        /* restore EDI */
    movl   0xc(%eax),   %esi        /* restore ESI */

    fldcw  0x18(%eax)               /* restore x87 control word */

    movl   0x10(%eax),  %esp        /* restore ESP */
    movl   0x14(%eax),  %ecx        /* fetch the address to return to */
    pushl  %ecx                     /* push the return address on the new stack so we can return there */

    xorl   %eax,        %eax        /* set cwreturn value to zero */
    ret                             /* 'ret' will pop the return address of the code and jump to it */
.size set_fcontext,.-set_fcontext

.text
.globl swap_fcontext
.align 2
.type swap_fcontext,@function
swap_fcontext:
    movl    0x4(%esp), %eax         /* load address of the first fcontext_t arg */
    movl    %ebx,       (%eax)      /* save EBX */
    movl    %ebp,       0x4(%eax)   /* save EBP */
    movl    %edi,       0x8(%eax)   /* save EDI */
    movl    %esi,       0xc(%eax)   /* save ESI */

    fnstcw  0x18(%eax)              /* save x87 control word */

    leal    0x4(%esp),  %ecx        /* exclude the return address */
    movl    %ecx,       0x10(%eax)  /* save as stack pointer */
    movl    (%esp),     %ecx        /* load return address */
    movl    %ecx,       0x14(%eax)  /* save return address */


    movl    0x8(%esp),   %eax       /* load address of the second fcontext_t arg */
    movl    (%eax),      %ebx       /* restore EBX */
    movl    0x4(%eax),   %ebp       /* restore EBP */
    movl    0x8(%eax),   %edi       /* restore EDI */
    movl    0xc(%eax),   %esi       /* restore ESI */

    fldcw   0x18(%eax)              /* restore x87 control word */

    movl    0x10(%eax),  %esp       /* restore ESP */
    movl    0x14(%eax),  %ecx       /* fetch the address to return to */
    pushl   %ecx                    /* push the return address on the new stack so we can return there */

    xorl    %eax,        %eax       /* set return value to zero */
    ret                             /* 'ret' will pop the return address of the code and jump to it */
.size swap_fcontext,.-swap_fcontext

.text
.globl make_fcontext
.align 2
.type make_fcontext,@function
make_fcontext:
    movl   0x4(%esp),      %eax         /* load address of the fcontext_t */
    movl   0x8(%esp),      %ecx         /* load the address of the function supposed to run */
    movl   %ecx,           0x14(%eax)   /* save the address of the function supposed to run */
    movl   0x1c(%eax),     %edx         /* load the stack base */
    leal   -0x8(%edx),     %edx         /* reserve space for the last frame on stack */
    movl   %edx,           0x10(%eax)   /* save the address */
    movl   0x24(%eax),     %ecx         /* load the address of the next context */
    movl   %ecx,           (%eax)       /* save the address of the next context */
    movl   0xc(%esp),      %ecx         /* load the address of the void pointer */
    movl   %ecx,           0x4(%edx)    /* save the address of the void pointer */
    movl   $link_fcontext,  (%edx)      /* helper code executed after fn() returns */

    xorl   %eax,            %eax        /* set return value to zero */
    ret                                 /* return 0 */

link_fcontext:
    pushl  %ebx
    testl  %ebx,           %ebx         /* test if a next context was given */
    je     finish                       /* jump to finish */
#if defined(__PIC__) || defined(__pic__)
    call   set_fcontext@PLT             /* install next context */
#else
    call   set_fcontext                 /* install next context */
#endif

finish:
#if defined(__PIC__) || defined(__pic__)
    call   _exit@PLT                    /* exit application */
#else
    call   _exit                        /* exit application */
#endif
.size make_fcontext,.-make_fcontext

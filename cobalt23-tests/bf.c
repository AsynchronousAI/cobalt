#include "aot_header.c"

// source = @bf.cobalt
// main function
static
CallInfo *magic_implementation_00(lua_State *L, CallInfo *ci)
{
  LClosure *cl;
  TValue *k;
  StkId base;
  const Instruction *pc;
  int trap;

  trap = L->hookmask;
  cl = clLvalue(s2v(ci->func));
  k = cl->p->k;
  pc = ci->u.l.savedpc;
  if (l_unlikely(trap)) {
    if (pc == cl->p->code) {  /* first instruction (not resuming)? */
      if (cl->p->is_vararg)
        trap = 0;  /* hooks will start after VARARGPREP instruction */
      else  /* check 'call' hook */
        luaD_hookcall(L, ci);
    }
    ci->u.l.trap = 1;  /* assume trap is on, for now */
  }
  base = ci->func + 1;
  /* main loop of interpreter */
  Instruction *code = cl->p->code;
  Instruction i;
  StkId ra;

  switch (pc - code) {
    case 0: goto label_00;
    case 1: goto label_01;
    case 2: goto label_02;
    case 3: goto label_03;
    case 4: goto label_04;
    case 5: goto label_05;
  }

  // 0	[1]	VARARGPREP	0
  #undef  AOT_PC
  #define AOT_PC (code + 1)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_02
  label_00: {
    aot_vmfetch(0x00000051);
    ProtectNT(luaT_adjustvarargs(L, GETARG_A(i), ci, cl->p));
    if (l_unlikely(trap)) {  /* previous "Protect" updated trap */
      luaD_hookcall(L, ci);
      L->oldpc = 1;  /* next opcode will be seen as a "new" line */
    }
    updatebase(ci);  /* function has new base after adjustment */
  }

  // 1	[15]	CLOSURE  	0 0	; 0x7fccffa044d0
  #undef  AOT_PC
  #define AOT_PC (code + 2)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_03
  label_01: {
    aot_vmfetch(0x0000004f);
    Proto *p = cl->p->p[GETARG_Bx(i)];
    halfProtect(pushclosure(L, p, cl->upvals, base, ra));
    checkGC(L, ra + 1);
  }

  // 2	[1]	SETTABUP 	0 0 0	; _ENV "cleanbf"
  #undef  AOT_PC
  #define AOT_PC (code + 3)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_04
  label_02: {
    aot_vmfetch(0x0000000f);
    const TValue *slot;
    TValue *upval = cl->upvals[GETARG_A(i)]->v;
    TValue *rb = KB(i);
    TValue *rc = RKC(i);
    TString *key = tsvalue(rb);  /* key must be a string */
    if (luaV_fastget(L, upval, key, slot, luaH_getshortstr)) {
      luaV_finishfastset(L, upval, slot, rc);
    }
    else
      Protect(luaV_finishset(L, upval, rb, rc, slot));
  }

  // 3	[65]	CLOSURE  	0 1	; 0x7fccffa04fb0
  #undef  AOT_PC
  #define AOT_PC (code + 4)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_05
  label_03: {
    aot_vmfetch(0x0000804f);
    Proto *p = cl->p->p[GETARG_Bx(i)];
    halfProtect(pushclosure(L, p, cl->upvals, base, ra));
    checkGC(L, ra + 1);
  }

  // 4	[16]	SETTABUP 	0 1 0	; _ENV "interpret"
  #undef  AOT_PC
  #define AOT_PC (code + 5)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  label_04: {
    aot_vmfetch(0x0001000f);
    const TValue *slot;
    TValue *upval = cl->upvals[GETARG_A(i)]->v;
    TValue *rb = KB(i);
    TValue *rc = RKC(i);
    TString *key = tsvalue(rb);  /* key must be a string */
    if (luaV_fastget(L, upval, key, slot, luaH_getshortstr)) {
      luaV_finishfastset(L, upval, slot, rc);
    }
    else
      Protect(luaV_finishset(L, upval, rb, rc, slot));
  }

  // 5	[65]	RETURN   	0 1 1	; 0 out
  #undef  AOT_PC
  #define AOT_PC (code + 6)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  label_05: {
    aot_vmfetch(0x01010046);
    int n = GETARG_B(i) - 1;  /* number of results */
    int nparams1 = GETARG_C(i);
    if (n < 0)  /* not fixed? */
      n = cast_int(L->top - ra);  /* get what is available */
    savepc(ci);
    if (TESTARG_k(i)) {  /* may there be open upvalues? */
      if (L->top < ci->top)
        L->top = ci->top;
      luaF_close(L, base, CLOSEKTOP, 1);
      updatetrap(ci);
      updatestack(ci);
    }
    if (nparams1)  /* vararg function? */
      ci->func -= ci->u.l.nextraargs + nparams1;
    L->top = ra + n;  /* set call for 'luaD_poscall' */
    luaD_poscall(L, ci, n);
    updatetrap(ci);  /* 'luaD_poscall' can change hooks */
    if (ci->callstatus & CIST_FRESH)
        return NULL;  /* end this frame */
    else {
        ci = ci->previous;
        return ci;
    }
  }

}

// source = @bf.cobalt
// lines: 1 - 15
static
CallInfo *magic_implementation_01(lua_State *L, CallInfo *ci)
{
  LClosure *cl;
  TValue *k;
  StkId base;
  const Instruction *pc;
  int trap;

  trap = L->hookmask;
  cl = clLvalue(s2v(ci->func));
  k = cl->p->k;
  pc = ci->u.l.savedpc;
  if (l_unlikely(trap)) {
    if (pc == cl->p->code) {  /* first instruction (not resuming)? */
      if (cl->p->is_vararg)
        trap = 0;  /* hooks will start after VARARGPREP instruction */
      else  /* check 'call' hook */
        luaD_hookcall(L, ci);
    }
    ci->u.l.trap = 1;  /* assume trap is on, for now */
  }
  base = ci->func + 1;
  /* main loop of interpreter */
  Instruction *code = cl->p->code;
  Instruction i;
  StkId ra;

  switch (pc - code) {
    case 0: goto label_00;
    case 1: goto label_01;
    case 2: goto label_02;
    case 3: goto label_03;
    case 4: goto label_04;
    case 5: goto label_05;
    case 6: goto label_06;
    case 7: goto label_07;
    case 8: goto label_08;
    case 9: goto label_09;
    case 10: goto label_10;
    case 11: goto label_11;
    case 12: goto label_12;
    case 13: goto label_13;
    case 14: goto label_14;
    case 15: goto label_15;
    case 16: goto label_16;
    case 17: goto label_17;
    case 18: goto label_18;
    case 19: goto label_19;
    case 20: goto label_20;
    case 21: goto label_21;
    case 22: goto label_22;
    case 23: goto label_23;
    case 24: goto label_24;
    case 25: goto label_25;
    case 26: goto label_26;
    case 27: goto label_27;
    case 28: goto label_28;
    case 29: goto label_29;
    case 30: goto label_30;
    case 31: goto label_31;
    case 32: goto label_32;
    case 33: goto label_33;
  }

  // 0	[6]	LOADK    	1 0	; ""
  #undef  AOT_PC
  #define AOT_PC (code + 1)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_02
  label_00: {
    aot_vmfetch(0x00000083);
    TValue *rb = k + GETARG_Bx(i);
    setobj2s(L, ra, rb);
  }

  // 1	[7]	LOADI    	2 1
  #undef  AOT_PC
  #define AOT_PC (code + 2)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_03
  label_01: {
    aot_vmfetch(0x80000101);
    lua_Integer b = GETARG_sBx(i);
    setivalue(s2v(ra), b);
  }

  // 2	[7]	LEN      	3 0
  #undef  AOT_PC
  #define AOT_PC (code + 3)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_04
  label_02: {
    aot_vmfetch(0x000001b4);
    Protect(luaV_objlen(L, ra, vRB(i)));
  }

  // 3	[7]	LOADI    	4 1
  #undef  AOT_PC
  #define AOT_PC (code + 4)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_05
  label_03: {
    aot_vmfetch(0x80000201);
    lua_Integer b = GETARG_sBx(i);
    setivalue(s2v(ra), b);
  }

  // 4	[7]	FORPREP  	2 26	; to 32
  #undef  AOT_PC
  #define AOT_PC (code + 5)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_06
  label_04: {
    aot_vmfetch(0x000d014a);
    savestate(L, ci);  /* in case of errors */
    if (forprep(L, ra))
      goto label_32; /* skip the loop */
  }

  // 5	[8]	GETTABUP 	6 0 1	; _ENV "string"
  #undef  AOT_PC
  #define AOT_PC (code + 6)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_07
  label_05: {
    aot_vmfetch(0x0100030b);
    const TValue *slot;
    TValue *upval = cl->upvals[GETARG_B(i)]->v;
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, upval, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, upval, rc, ra, slot));
  }

  // 6	[8]	GETFIELD 	6 6 2	; "sub"
  #undef  AOT_PC
  #define AOT_PC (code + 7)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_08
  label_06: {
    aot_vmfetch(0x0206030e);
    const TValue *slot;
    TValue *rb = vRB(i);
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, rb, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, rb, rc, ra, slot));
  }

  // 7	[8]	MOVE     	7 0
  #undef  AOT_PC
  #define AOT_PC (code + 8)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_09
  label_07: {
    aot_vmfetch(0x00000380);
    setobjs2s(L, ra, RB(i));
  }

  // 8	[8]	MOVE     	8 5
  #undef  AOT_PC
  #define AOT_PC (code + 9)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_10
  label_08: {
    aot_vmfetch(0x00050400);
    setobjs2s(L, ra, RB(i));
  }

  // 9	[8]	MOVE     	9 5
  #undef  AOT_PC
  #define AOT_PC (code + 10)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_11
  label_09: {
    aot_vmfetch(0x00050480);
    setobjs2s(L, ra, RB(i));
  }

  // 10	[8]	CALL     	6 4 2	; 3 in 1 out
  #undef  AOT_PC
  #define AOT_PC (code + 11)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_12
  label_10: {
    aot_vmfetch(0x02040344);
    CallInfo *newci;
    int b = GETARG_B(i);
    int nresults = GETARG_C(i) - 1;
    if (b != 0)  /* fixed number of arguments? */
        L->top = ra + b;  /* top signals number of arguments */
    /* else previous instruction set top */
    savepc(L);  /* in case of errors */
    if ((newci = luaD_precall(L, ra, nresults)) == NULL)
        updatetrap(ci);  /* C call; nothing else to be done */
    else {
        ci = newci;
        ci->callstatus = 0;  /* call re-uses 'luaV_execute' */
        return ci;
    }
  }

  // 11	[9]	EQK      	6 3 1	; "-"
  #undef  AOT_PC
  #define AOT_PC (code + 12)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_27
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_13
  label_11: {
    aot_vmfetch(0x0003833c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 12	[9]	JMP      	14	; to 28
  #undef  AOT_PC
  #define AOT_PC (code + 13)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_14
  label_12: {
    aot_vmfetch(0x800006b8);
    updatetrap(ci);
    goto label_27;
  }

  // 13	[9]	EQK      	6 4 1	; "+"
  #undef  AOT_PC
  #define AOT_PC (code + 14)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_27
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_15
  label_13: {
    aot_vmfetch(0x0004833c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 14	[9]	JMP      	12	; to 28
  #undef  AOT_PC
  #define AOT_PC (code + 15)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_16
  label_14: {
    aot_vmfetch(0x800005b8);
    updatetrap(ci);
    goto label_27;
  }

  // 15	[9]	EQK      	6 5 1	; ">"
  #undef  AOT_PC
  #define AOT_PC (code + 16)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_27
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_17
  label_15: {
    aot_vmfetch(0x0005833c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 16	[9]	JMP      	10	; to 28
  #undef  AOT_PC
  #define AOT_PC (code + 17)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_18
  label_16: {
    aot_vmfetch(0x800004b8);
    updatetrap(ci);
    goto label_27;
  }

  // 17	[9]	EQK      	6 6 1	; "<"
  #undef  AOT_PC
  #define AOT_PC (code + 18)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_27
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_19
  label_17: {
    aot_vmfetch(0x0006833c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 18	[9]	JMP      	8	; to 28
  #undef  AOT_PC
  #define AOT_PC (code + 19)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_20
  label_18: {
    aot_vmfetch(0x800003b8);
    updatetrap(ci);
    goto label_27;
  }

  // 19	[9]	EQK      	6 7 1	; "."
  #undef  AOT_PC
  #define AOT_PC (code + 20)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_27
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_21
  label_19: {
    aot_vmfetch(0x0007833c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 20	[9]	JMP      	6	; to 28
  #undef  AOT_PC
  #define AOT_PC (code + 21)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_22
  label_20: {
    aot_vmfetch(0x800002b8);
    updatetrap(ci);
    goto label_27;
  }

  // 21	[9]	EQK      	6 8 1	; ","
  #undef  AOT_PC
  #define AOT_PC (code + 22)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_27
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_23
  label_21: {
    aot_vmfetch(0x0008833c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 22	[9]	JMP      	4	; to 28
  #undef  AOT_PC
  #define AOT_PC (code + 23)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_24
  label_22: {
    aot_vmfetch(0x800001b8);
    updatetrap(ci);
    goto label_27;
  }

  // 23	[9]	EQK      	6 9 1	; "["
  #undef  AOT_PC
  #define AOT_PC (code + 24)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_27
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_25
  label_23: {
    aot_vmfetch(0x0009833c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 24	[9]	JMP      	2	; to 28
  #undef  AOT_PC
  #define AOT_PC (code + 25)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_26
  label_24: {
    aot_vmfetch(0x800000b8);
    updatetrap(ci);
    goto label_27;
  }

  // 25	[9]	EQK      	6 10 0	; "]"
  #undef  AOT_PC
  #define AOT_PC (code + 26)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_31
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_27
  label_25: {
    aot_vmfetch(0x000a033c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 26	[9]	JMP      	4	; to 32
  #undef  AOT_PC
  #define AOT_PC (code + 27)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_28
  label_26: {
    aot_vmfetch(0x800001b8);
    updatetrap(ci);
    goto label_31;
  }

  // 27	[10]	MOVE     	7 1
  #undef  AOT_PC
  #define AOT_PC (code + 28)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_29
  label_27: {
    aot_vmfetch(0x00010380);
    setobjs2s(L, ra, RB(i));
  }

  // 28	[10]	MOVE     	8 6
  #undef  AOT_PC
  #define AOT_PC (code + 29)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_30
  label_28: {
    aot_vmfetch(0x00060400);
    setobjs2s(L, ra, RB(i));
  }

  // 29	[10]	CONCAT   	7 2
  #undef  AOT_PC
  #define AOT_PC (code + 30)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_31
  label_29: {
    aot_vmfetch(0x000203b5);
    int n = GETARG_B(i);  /* number of elements to concatenate */
    L->top = ra + n;  /* mark the end of concat operands */
    ProtectNT(luaV_concat(L, n));
    checkGC(L, L->top); /* 'luaV_concat' ensures correct top */
  }

  // 30	[10]	MOVE     	1 7
  #undef  AOT_PC
  #define AOT_PC (code + 31)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_32
  label_30: {
    aot_vmfetch(0x00070080);
    setobjs2s(L, ra, RB(i));
  }

  // 31	[7]	FORLOOP  	2 27	; to 6
  #undef  AOT_PC
  #define AOT_PC (code + 32)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_33
  label_31: {
    aot_vmfetch(0x000d8149);
    if (ttisinteger(s2v(ra + 2))) {  /* integer loop? */
      lua_Unsigned count = l_castS2U(ivalue(s2v(ra + 1)));
      if (count > 0) {  /* still more iterations? */
        lua_Integer step = ivalue(s2v(ra + 2));
        lua_Integer idx = ivalue(s2v(ra));  /* internal index */
        chgivalue(s2v(ra + 1), count - 1);  /* update counter */
        idx = intop(+, idx, step);  /* add step to index */
        chgivalue(s2v(ra), idx);  /* update internal index */
        setivalue(s2v(ra + 3), idx);  /* and control variable */
        goto label_05; /* jump back */
      }
    }
    else if (floatforloop(ra)) /* float loop */
      goto label_05; /* jump back */
    updatetrap(ci);  /* allows a signal to break the loop */
  }

  // 32	[14]	RETURN1  	1
  #undef  AOT_PC
  #define AOT_PC (code + 33)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  label_32: {
    aot_vmfetch(0x000200c8);
    if (l_unlikely(L->hookmask)) {
      L->top = ra + 1;
      savepc(ci);
      luaD_poscall(L, ci, 1);  /* no hurry... */
      trap = 1;
    }
    else {  /* do the 'poscall' here */
      int nres = ci->nresults;
      L->ci = ci->previous;  /* back to caller */
      if (nres == 0)
        L->top = base - 1;  /* asked for no results */
      else {
        setobjs2s(L, base - 1, ra);  /* at least this result */
        L->top = base;
        for (; l_unlikely(nres > 1); nres--)
          setnilvalue(s2v(L->top++));
      }
    }
    if (ci->callstatus & CIST_FRESH)
        return NULL;  /* end this frame */
    else {
        ci = ci->previous;
        return ci;
    }
  }

  // 33	[15]	RETURN0  	
  #undef  AOT_PC
  #define AOT_PC (code + 34)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  label_33: {
    aot_vmfetch(0x00010147);
    if (l_unlikely(L->hookmask)) {
      L->top = ra;
      savepc(ci);
      luaD_poscall(L, ci, 0);  /* no hurry... */
      trap = 1;
    }
    else {  /* do the 'poscall' here */
      int nres;
      L->ci = ci->previous;  /* back to caller */
      L->top = base - 1;
      for (nres = ci->nresults; l_unlikely(nres > 0); nres--)
        setnilvalue(s2v(L->top++));  /* all results are nil */
    }
    if (ci->callstatus & CIST_FRESH)
        return NULL;  /* end this frame */
    else {
        ci = ci->previous;
        return ci;
    }
  }

}

// source = @bf.cobalt
// lines: 16 - 65
static
CallInfo *magic_implementation_02(lua_State *L, CallInfo *ci)
{
  LClosure *cl;
  TValue *k;
  StkId base;
  const Instruction *pc;
  int trap;

  trap = L->hookmask;
  cl = clLvalue(s2v(ci->func));
  k = cl->p->k;
  pc = ci->u.l.savedpc;
  if (l_unlikely(trap)) {
    if (pc == cl->p->code) {  /* first instruction (not resuming)? */
      if (cl->p->is_vararg)
        trap = 0;  /* hooks will start after VARARGPREP instruction */
      else  /* check 'call' hook */
        luaD_hookcall(L, ci);
    }
    ci->u.l.trap = 1;  /* assume trap is on, for now */
  }
  base = ci->func + 1;
  /* main loop of interpreter */
  Instruction *code = cl->p->code;
  Instruction i;
  StkId ra;

  switch (pc - code) {
    case 0: goto label_00;
    case 1: goto label_01;
    case 2: goto label_02;
    case 3: goto label_03;
    case 4: goto label_04;
    case 5: goto label_05;
    case 6: goto label_06;
    case 7: goto label_07;
    case 8: goto label_08;
    case 9: goto label_09;
    case 10: goto label_10;
    case 11: goto label_11;
    case 12: goto label_12;
    case 13: goto label_13;
    case 14: goto label_14;
    case 15: goto label_15;
    case 16: goto label_16;
    case 17: goto label_17;
    case 18: goto label_18;
    case 19: goto label_19;
    case 20: goto label_20;
    case 21: goto label_21;
    case 22: goto label_22;
    case 23: goto label_23;
    case 24: goto label_24;
    case 25: goto label_25;
    case 26: goto label_26;
    case 27: goto label_27;
    case 28: goto label_28;
    case 29: goto label_29;
    case 30: goto label_30;
    case 31: goto label_31;
    case 32: goto label_32;
    case 33: goto label_33;
    case 34: goto label_34;
    case 35: goto label_35;
    case 36: goto label_36;
    case 37: goto label_37;
    case 38: goto label_38;
    case 39: goto label_39;
    case 40: goto label_40;
    case 41: goto label_41;
    case 42: goto label_42;
    case 43: goto label_43;
    case 44: goto label_44;
    case 45: goto label_45;
    case 46: goto label_46;
    case 47: goto label_47;
    case 48: goto label_48;
    case 49: goto label_49;
    case 50: goto label_50;
    case 51: goto label_51;
    case 52: goto label_52;
    case 53: goto label_53;
    case 54: goto label_54;
    case 55: goto label_55;
    case 56: goto label_56;
    case 57: goto label_57;
    case 58: goto label_58;
    case 59: goto label_59;
    case 60: goto label_60;
    case 61: goto label_61;
    case 62: goto label_62;
    case 63: goto label_63;
    case 64: goto label_64;
    case 65: goto label_65;
    case 66: goto label_66;
    case 67: goto label_67;
    case 68: goto label_68;
    case 69: goto label_69;
    case 70: goto label_70;
    case 71: goto label_71;
    case 72: goto label_72;
    case 73: goto label_73;
    case 74: goto label_74;
    case 75: goto label_75;
    case 76: goto label_76;
    case 77: goto label_77;
    case 78: goto label_78;
    case 79: goto label_79;
    case 80: goto label_80;
    case 81: goto label_81;
    case 82: goto label_82;
    case 83: goto label_83;
    case 84: goto label_84;
    case 85: goto label_85;
    case 86: goto label_86;
    case 87: goto label_87;
    case 88: goto label_88;
    case 89: goto label_89;
    case 90: goto label_90;
    case 91: goto label_91;
    case 92: goto label_92;
    case 93: goto label_93;
    case 94: goto label_94;
    case 95: goto label_95;
    case 96: goto label_96;
    case 97: goto label_97;
    case 98: goto label_98;
    case 99: goto label_99;
    case 100: goto label_100;
    case 101: goto label_101;
    case 102: goto label_102;
    case 103: goto label_103;
    case 104: goto label_104;
    case 105: goto label_105;
    case 106: goto label_106;
    case 107: goto label_107;
    case 108: goto label_108;
    case 109: goto label_109;
    case 110: goto label_110;
    case 111: goto label_111;
    case 112: goto label_112;
    case 113: goto label_113;
    case 114: goto label_114;
    case 115: goto label_115;
    case 116: goto label_116;
    case 117: goto label_117;
    case 118: goto label_118;
    case 119: goto label_119;
    case 120: goto label_120;
    case 121: goto label_121;
    case 122: goto label_122;
    case 123: goto label_123;
    case 124: goto label_124;
    case 125: goto label_125;
    case 126: goto label_126;
    case 127: goto label_127;
    case 128: goto label_128;
    case 129: goto label_129;
    case 130: goto label_130;
    case 131: goto label_131;
    case 132: goto label_132;
    case 133: goto label_133;
    case 134: goto label_134;
    case 135: goto label_135;
    case 136: goto label_136;
    case 137: goto label_137;
    case 138: goto label_138;
    case 139: goto label_139;
    case 140: goto label_140;
    case 141: goto label_141;
    case 142: goto label_142;
    case 143: goto label_143;
    case 144: goto label_144;
  }

  // 0	[17]	GETTABUP 	1 0 0	; _ENV "cleanbf"
  #undef  AOT_PC
  #define AOT_PC (code + 1)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_02
  label_00: {
    aot_vmfetch(0x0000008b);
    const TValue *slot;
    TValue *upval = cl->upvals[GETARG_B(i)]->v;
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, upval, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, upval, rc, ra, slot));
  }

  // 1	[17]	MOVE     	2 0
  #undef  AOT_PC
  #define AOT_PC (code + 2)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_03
  label_01: {
    aot_vmfetch(0x00000100);
    setobjs2s(L, ra, RB(i));
  }

  // 2	[17]	CALL     	1 2 2	; 1 in 1 out
  #undef  AOT_PC
  #define AOT_PC (code + 3)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_04
  label_02: {
    aot_vmfetch(0x020200c4);
    CallInfo *newci;
    int b = GETARG_B(i);
    int nresults = GETARG_C(i) - 1;
    if (b != 0)  /* fixed number of arguments? */
        L->top = ra + b;  /* top signals number of arguments */
    /* else previous instruction set top */
    savepc(L);  /* in case of errors */
    if ((newci = luaD_precall(L, ra, nresults)) == NULL)
        updatetrap(ci);  /* C call; nothing else to be done */
    else {
        ci = newci;
        ci->callstatus = 0;  /* call re-uses 'luaV_execute' */
        return ci;
    }
  }

  // 3	[17]	MOVE     	0 1
  #undef  AOT_PC
  #define AOT_PC (code + 4)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_05
  label_03: {
    aot_vmfetch(0x00010000);
    setobjs2s(L, ra, RB(i));
  }

  // 4	[18]	NEWTABLE 	1 0 0	; 0
  #undef  AOT_PC
  #define AOT_PC (code + 5)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_06
  label_04: {
    aot_vmfetch(0x00000093);
    int b = GETARG_B(i);  /* log2(hash size) + 1 */
    int c = GETARG_C(i);  /* array size */
    Table *t;
    if (b > 0)
      b = 1 << (b - 1);  /* size is 2^(b - 1) */
    lua_assert((!TESTARG_k(i)) == (GETARG_Ax(0x00000052) == 0));
    if (TESTARG_k(i))
      c += GETARG_Ax(0x00000052) * (MAXARG_C + 1);
    /* skip extra argument */
    L->top = ra + 1;  /* correct top in case of emergency GC */
    t = luaH_new(L);  /* memory allocation */
    sethvalue2s(L, ra, t);
    if (b != 0 || c != 0)
      luaH_resize(L, t, c, b);  /* idem */
    checkGC(L, ra + 1);
    goto AOT_SKIP1;
  }

  // 5	[18]	EXTRAARG 	0
  #undef  AOT_PC
  #define AOT_PC (code + 6)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_07
  label_05: {
    aot_vmfetch(0x00000052);
    lua_assert(0);
  }

  // 6	[19]	LOADI    	2 1
  #undef  AOT_PC
  #define AOT_PC (code + 7)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_08
  label_06: {
    aot_vmfetch(0x80000101);
    lua_Integer b = GETARG_sBx(i);
    setivalue(s2v(ra), b);
  }

  // 7	[20]	LOADI    	3 1
  #undef  AOT_PC
  #define AOT_PC (code + 8)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_09
  label_07: {
    aot_vmfetch(0x80000181);
    lua_Integer b = GETARG_sBx(i);
    setivalue(s2v(ra), b);
  }

  // 8	[22]	LEN      	4 0
  #undef  AOT_PC
  #define AOT_PC (code + 9)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_10
  label_08: {
    aot_vmfetch(0x00000234);
    Protect(luaV_objlen(L, ra, vRB(i)));
  }

  // 9	[22]	LE       	3 4 0
  #undef  AOT_PC
  #define AOT_PC (code + 10)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_144
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_11
  label_09: {
    aot_vmfetch(0x000401bb);
    op_order(L, l_lei, LEnum, lessequalothers);
  }

  // 10	[22]	JMP      	133	; to 145
  #undef  AOT_PC
  #define AOT_PC (code + 11)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_12
  label_10: {
    aot_vmfetch(0x80004238);
    updatetrap(ci);
    goto label_144;
  }

  // 11	[23]	GETTABUP 	4 0 1	; _ENV "string"
  #undef  AOT_PC
  #define AOT_PC (code + 12)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_13
  label_11: {
    aot_vmfetch(0x0100020b);
    const TValue *slot;
    TValue *upval = cl->upvals[GETARG_B(i)]->v;
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, upval, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, upval, rc, ra, slot));
  }

  // 12	[23]	GETFIELD 	4 4 2	; "sub"
  #undef  AOT_PC
  #define AOT_PC (code + 13)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_14
  label_12: {
    aot_vmfetch(0x0204020e);
    const TValue *slot;
    TValue *rb = vRB(i);
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, rb, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, rb, rc, ra, slot));
  }

  // 13	[23]	MOVE     	5 0
  #undef  AOT_PC
  #define AOT_PC (code + 14)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_15
  label_13: {
    aot_vmfetch(0x00000280);
    setobjs2s(L, ra, RB(i));
  }

  // 14	[23]	MOVE     	6 3
  #undef  AOT_PC
  #define AOT_PC (code + 15)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_16
  label_14: {
    aot_vmfetch(0x00030300);
    setobjs2s(L, ra, RB(i));
  }

  // 15	[23]	MOVE     	7 3
  #undef  AOT_PC
  #define AOT_PC (code + 16)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_17
  label_15: {
    aot_vmfetch(0x00030380);
    setobjs2s(L, ra, RB(i));
  }

  // 16	[23]	CALL     	4 4 2	; 3 in 1 out
  #undef  AOT_PC
  #define AOT_PC (code + 17)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_18
  label_16: {
    aot_vmfetch(0x02040244);
    CallInfo *newci;
    int b = GETARG_B(i);
    int nresults = GETARG_C(i) - 1;
    if (b != 0)  /* fixed number of arguments? */
        L->top = ra + b;  /* top signals number of arguments */
    /* else previous instruction set top */
    savepc(L);  /* in case of errors */
    if ((newci = luaD_precall(L, ra, nresults)) == NULL)
        updatetrap(ci);  /* C call; nothing else to be done */
    else {
        ci = newci;
        ci->callstatus = 0;  /* call re-uses 'luaV_execute' */
        return ci;
    }
  }

  // 17	[25]	EQK      	4 3 0	; ">"
  #undef  AOT_PC
  #define AOT_PC (code + 18)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_22
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_19
  label_17: {
    aot_vmfetch(0x0003023c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 18	[25]	JMP      	3	; to 23
  #undef  AOT_PC
  #define AOT_PC (code + 19)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_20
  label_18: {
    aot_vmfetch(0x80000138);
    updatetrap(ci);
    goto label_22;
  }

  // 19	[26]	ADDI     	2 2 1
  #undef  AOT_PC
  #define AOT_PC (code + 20)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_21
  label_19: {
    aot_vmfetch(0x80020115);
    op_arithI(L, l_addi, luai_numadd);
  }

  // 20	[26]	MMBINI   	2 1 6 0	; __add
  #undef  AOT_PC
  #define AOT_PC (code + 21)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_141
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_22
  label_20: {
    aot_vmfetch(0x0680012f);
    Instruction pi = 0x80020115;  /* original arith. expression */
    int imm = GETARG_sB(i);
    TMS tm = (TMS)GETARG_C(i);
    int flip = GETARG_k(i);
    StkId result = RA(pi);
    Protect(luaT_trybiniTM(L, s2v(ra), imm, flip, result, tm));
  }

  // 21	[27]	JMP      	119	; to 142
  #undef  AOT_PC
  #define AOT_PC (code + 22)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_23
  label_21: {
    aot_vmfetch(0x80003b38);
    updatetrap(ci);
    goto label_141;
  }

  // 22	[27]	EQK      	4 4 0	; "<"
  #undef  AOT_PC
  #define AOT_PC (code + 23)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_27
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_24
  label_22: {
    aot_vmfetch(0x0004023c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 23	[27]	JMP      	3	; to 28
  #undef  AOT_PC
  #define AOT_PC (code + 24)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_25
  label_23: {
    aot_vmfetch(0x80000138);
    updatetrap(ci);
    goto label_27;
  }

  // 24	[28]	ADDI     	2 2 -1
  #undef  AOT_PC
  #define AOT_PC (code + 25)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_26
  label_24: {
    aot_vmfetch(0x7e020115);
    op_arithI(L, l_addi, luai_numadd);
  }

  // 25	[28]	MMBINI   	2 1 7 0	; __sub
  #undef  AOT_PC
  #define AOT_PC (code + 26)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_141
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_27
  label_25: {
    aot_vmfetch(0x0780012f);
    Instruction pi = 0x7e020115;  /* original arith. expression */
    int imm = GETARG_sB(i);
    TMS tm = (TMS)GETARG_C(i);
    int flip = GETARG_k(i);
    StkId result = RA(pi);
    Protect(luaT_trybiniTM(L, s2v(ra), imm, flip, result, tm));
  }

  // 26	[29]	JMP      	114	; to 142
  #undef  AOT_PC
  #define AOT_PC (code + 27)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_28
  label_26: {
    aot_vmfetch(0x800038b8);
    updatetrap(ci);
    goto label_141;
  }

  // 27	[29]	EQK      	4 5 0	; "+"
  #undef  AOT_PC
  #define AOT_PC (code + 28)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_37
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_29
  label_27: {
    aot_vmfetch(0x0005023c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 28	[29]	JMP      	8	; to 38
  #undef  AOT_PC
  #define AOT_PC (code + 29)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_30
  label_28: {
    aot_vmfetch(0x800003b8);
    updatetrap(ci);
    goto label_37;
  }

  // 29	[30]	GETTABLE 	5 1 2
  #undef  AOT_PC
  #define AOT_PC (code + 30)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_31
  label_29: {
    aot_vmfetch(0x0201028c);
    const TValue *slot;
    TValue *rb = vRB(i);
    TValue *rc = vRC(i);
    lua_Unsigned n;
    if (ttisinteger(rc)  /* fast track for integers? */
        ? (cast_void(n = ivalue(rc)), luaV_fastgeti(L, rb, n, slot))
        : luaV_fastget(L, rb, rc, slot, luaH_get)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, rb, rc, ra, slot));
  }

  // 30	[30]	TEST     	5 1
  #undef  AOT_PC
  #define AOT_PC (code + 31)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_33
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_32
  label_30: {
    aot_vmfetch(0x000082c2);
    int cond = !l_isfalse(s2v(ra));
    docondjump();
  }

  // 31	[30]	JMP      	1	; to 34
  #undef  AOT_PC
  #define AOT_PC (code + 32)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_33
  label_31: {
    aot_vmfetch(0x80000038);
    updatetrap(ci);
    goto label_33;
  }

  // 32	[30]	LOADI    	5 0
  #undef  AOT_PC
  #define AOT_PC (code + 33)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_34
  label_32: {
    aot_vmfetch(0x7fff8281);
    lua_Integer b = GETARG_sBx(i);
    setivalue(s2v(ra), b);
  }

  // 33	[30]	ADDI     	5 5 1
  #undef  AOT_PC
  #define AOT_PC (code + 34)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_35
  label_33: {
    aot_vmfetch(0x80050295);
    op_arithI(L, l_addi, luai_numadd);
  }

  // 34	[30]	MMBINI   	5 1 6 0	; __add
  #undef  AOT_PC
  #define AOT_PC (code + 35)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_36
  label_34: {
    aot_vmfetch(0x068002af);
    Instruction pi = 0x80050295;  /* original arith. expression */
    int imm = GETARG_sB(i);
    TMS tm = (TMS)GETARG_C(i);
    int flip = GETARG_k(i);
    StkId result = RA(pi);
    Protect(luaT_trybiniTM(L, s2v(ra), imm, flip, result, tm));
  }

  // 35	[30]	SETTABLE 	1 2 5
  #undef  AOT_PC
  #define AOT_PC (code + 36)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_141
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_37
  label_35: {
    aot_vmfetch(0x05020090);
    const TValue *slot;
    TValue *rb = vRB(i);  /* key (table is in 'ra') */
    TValue *rc = RKC(i);  /* value */
    lua_Unsigned n;
    if (ttisinteger(rb)  /* fast track for integers? */
        ? (cast_void(n = ivalue(rb)), luaV_fastgeti(L, s2v(ra), n, slot))
        : luaV_fastget(L, s2v(ra), rb, slot, luaH_get)) {
      luaV_finishfastset(L, s2v(ra), slot, rc);
    }
    else
      Protect(luaV_finishset(L, s2v(ra), rb, rc, slot));
  }

  // 36	[31]	JMP      	104	; to 142
  #undef  AOT_PC
  #define AOT_PC (code + 37)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_38
  label_36: {
    aot_vmfetch(0x800033b8);
    updatetrap(ci);
    goto label_141;
  }

  // 37	[31]	EQK      	4 6 0	; "-"
  #undef  AOT_PC
  #define AOT_PC (code + 38)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_47
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_39
  label_37: {
    aot_vmfetch(0x0006023c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 38	[31]	JMP      	8	; to 48
  #undef  AOT_PC
  #define AOT_PC (code + 39)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_40
  label_38: {
    aot_vmfetch(0x800003b8);
    updatetrap(ci);
    goto label_47;
  }

  // 39	[32]	GETTABLE 	5 1 2
  #undef  AOT_PC
  #define AOT_PC (code + 40)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_41
  label_39: {
    aot_vmfetch(0x0201028c);
    const TValue *slot;
    TValue *rb = vRB(i);
    TValue *rc = vRC(i);
    lua_Unsigned n;
    if (ttisinteger(rc)  /* fast track for integers? */
        ? (cast_void(n = ivalue(rc)), luaV_fastgeti(L, rb, n, slot))
        : luaV_fastget(L, rb, rc, slot, luaH_get)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, rb, rc, ra, slot));
  }

  // 40	[32]	TEST     	5 1
  #undef  AOT_PC
  #define AOT_PC (code + 41)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_43
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_42
  label_40: {
    aot_vmfetch(0x000082c2);
    int cond = !l_isfalse(s2v(ra));
    docondjump();
  }

  // 41	[32]	JMP      	1	; to 44
  #undef  AOT_PC
  #define AOT_PC (code + 42)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_43
  label_41: {
    aot_vmfetch(0x80000038);
    updatetrap(ci);
    goto label_43;
  }

  // 42	[32]	LOADI    	5 0
  #undef  AOT_PC
  #define AOT_PC (code + 43)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_44
  label_42: {
    aot_vmfetch(0x7fff8281);
    lua_Integer b = GETARG_sBx(i);
    setivalue(s2v(ra), b);
  }

  // 43	[32]	ADDI     	5 5 -1
  #undef  AOT_PC
  #define AOT_PC (code + 44)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_45
  label_43: {
    aot_vmfetch(0x7e050295);
    op_arithI(L, l_addi, luai_numadd);
  }

  // 44	[32]	MMBINI   	5 1 7 0	; __sub
  #undef  AOT_PC
  #define AOT_PC (code + 45)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_46
  label_44: {
    aot_vmfetch(0x078002af);
    Instruction pi = 0x7e050295;  /* original arith. expression */
    int imm = GETARG_sB(i);
    TMS tm = (TMS)GETARG_C(i);
    int flip = GETARG_k(i);
    StkId result = RA(pi);
    Protect(luaT_trybiniTM(L, s2v(ra), imm, flip, result, tm));
  }

  // 45	[32]	SETTABLE 	1 2 5
  #undef  AOT_PC
  #define AOT_PC (code + 46)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_141
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_47
  label_45: {
    aot_vmfetch(0x05020090);
    const TValue *slot;
    TValue *rb = vRB(i);  /* key (table is in 'ra') */
    TValue *rc = RKC(i);  /* value */
    lua_Unsigned n;
    if (ttisinteger(rb)  /* fast track for integers? */
        ? (cast_void(n = ivalue(rb)), luaV_fastgeti(L, s2v(ra), n, slot))
        : luaV_fastget(L, s2v(ra), rb, slot, luaH_get)) {
      luaV_finishfastset(L, s2v(ra), slot, rc);
    }
    else
      Protect(luaV_finishset(L, s2v(ra), rb, rc, slot));
  }

  // 46	[33]	JMP      	94	; to 142
  #undef  AOT_PC
  #define AOT_PC (code + 47)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_48
  label_46: {
    aot_vmfetch(0x80002eb8);
    updatetrap(ci);
    goto label_141;
  }

  // 47	[33]	EQK      	4 7 0	; "."
  #undef  AOT_PC
  #define AOT_PC (code + 48)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_60
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_49
  label_47: {
    aot_vmfetch(0x0007023c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 48	[33]	JMP      	11	; to 61
  #undef  AOT_PC
  #define AOT_PC (code + 49)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_50
  label_48: {
    aot_vmfetch(0x80000538);
    updatetrap(ci);
    goto label_60;
  }

  // 49	[34]	GETTABUP 	5 0 8	; _ENV "io"
  #undef  AOT_PC
  #define AOT_PC (code + 50)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_51
  label_49: {
    aot_vmfetch(0x0800028b);
    const TValue *slot;
    TValue *upval = cl->upvals[GETARG_B(i)]->v;
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, upval, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, upval, rc, ra, slot));
  }

  // 50	[34]	GETFIELD 	5 5 9	; "write"
  #undef  AOT_PC
  #define AOT_PC (code + 51)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_52
  label_50: {
    aot_vmfetch(0x0905028e);
    const TValue *slot;
    TValue *rb = vRB(i);
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, rb, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, rb, rc, ra, slot));
  }

  // 51	[34]	GETTABUP 	6 0 1	; _ENV "string"
  #undef  AOT_PC
  #define AOT_PC (code + 52)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_53
  label_51: {
    aot_vmfetch(0x0100030b);
    const TValue *slot;
    TValue *upval = cl->upvals[GETARG_B(i)]->v;
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, upval, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, upval, rc, ra, slot));
  }

  // 52	[34]	GETFIELD 	6 6 10	; "char"
  #undef  AOT_PC
  #define AOT_PC (code + 53)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_54
  label_52: {
    aot_vmfetch(0x0a06030e);
    const TValue *slot;
    TValue *rb = vRB(i);
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, rb, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, rb, rc, ra, slot));
  }

  // 53	[34]	GETTABLE 	7 1 2
  #undef  AOT_PC
  #define AOT_PC (code + 54)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_55
  label_53: {
    aot_vmfetch(0x0201038c);
    const TValue *slot;
    TValue *rb = vRB(i);
    TValue *rc = vRC(i);
    lua_Unsigned n;
    if (ttisinteger(rc)  /* fast track for integers? */
        ? (cast_void(n = ivalue(rc)), luaV_fastgeti(L, rb, n, slot))
        : luaV_fastget(L, rb, rc, slot, luaH_get)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, rb, rc, ra, slot));
  }

  // 54	[34]	TEST     	7 1
  #undef  AOT_PC
  #define AOT_PC (code + 55)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_57
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_56
  label_54: {
    aot_vmfetch(0x000083c2);
    int cond = !l_isfalse(s2v(ra));
    docondjump();
  }

  // 55	[34]	JMP      	1	; to 58
  #undef  AOT_PC
  #define AOT_PC (code + 56)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_57
  label_55: {
    aot_vmfetch(0x80000038);
    updatetrap(ci);
    goto label_57;
  }

  // 56	[34]	LOADI    	7 0
  #undef  AOT_PC
  #define AOT_PC (code + 57)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_58
  label_56: {
    aot_vmfetch(0x7fff8381);
    lua_Integer b = GETARG_sBx(i);
    setivalue(s2v(ra), b);
  }

  // 57	[34]	CALL     	6 2 0	; 1 in all out
  #undef  AOT_PC
  #define AOT_PC (code + 58)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_59
  label_57: {
    aot_vmfetch(0x00020344);
    CallInfo *newci;
    int b = GETARG_B(i);
    int nresults = GETARG_C(i) - 1;
    if (b != 0)  /* fixed number of arguments? */
        L->top = ra + b;  /* top signals number of arguments */
    /* else previous instruction set top */
    savepc(L);  /* in case of errors */
    if ((newci = luaD_precall(L, ra, nresults)) == NULL)
        updatetrap(ci);  /* C call; nothing else to be done */
    else {
        ci = newci;
        ci->callstatus = 0;  /* call re-uses 'luaV_execute' */
        return ci;
    }
  }

  // 58	[34]	CALL     	5 0 1	; all in 0 out
  #undef  AOT_PC
  #define AOT_PC (code + 59)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_141
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_60
  label_58: {
    aot_vmfetch(0x010002c4);
    CallInfo *newci;
    int b = GETARG_B(i);
    int nresults = GETARG_C(i) - 1;
    if (b != 0)  /* fixed number of arguments? */
        L->top = ra + b;  /* top signals number of arguments */
    /* else previous instruction set top */
    savepc(L);  /* in case of errors */
    if ((newci = luaD_precall(L, ra, nresults)) == NULL)
        updatetrap(ci);  /* C call; nothing else to be done */
    else {
        ci = newci;
        ci->callstatus = 0;  /* call re-uses 'luaV_execute' */
        return ci;
    }
  }

  // 59	[35]	JMP      	81	; to 142
  #undef  AOT_PC
  #define AOT_PC (code + 60)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_61
  label_59: {
    aot_vmfetch(0x80002838);
    updatetrap(ci);
    goto label_141;
  }

  // 60	[35]	EQK      	4 11 0	; ","
  #undef  AOT_PC
  #define AOT_PC (code + 61)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_70
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_62
  label_60: {
    aot_vmfetch(0x000b023c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 61	[35]	JMP      	8	; to 71
  #undef  AOT_PC
  #define AOT_PC (code + 62)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_63
  label_61: {
    aot_vmfetch(0x800003b8);
    updatetrap(ci);
    goto label_70;
  }

  // 62	[36]	GETTABUP 	5 0 8	; _ENV "io"
  #undef  AOT_PC
  #define AOT_PC (code + 63)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_64
  label_62: {
    aot_vmfetch(0x0800028b);
    const TValue *slot;
    TValue *upval = cl->upvals[GETARG_B(i)]->v;
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, upval, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, upval, rc, ra, slot));
  }

  // 63	[36]	GETFIELD 	5 5 12	; "read"
  #undef  AOT_PC
  #define AOT_PC (code + 64)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_65
  label_63: {
    aot_vmfetch(0x0c05028e);
    const TValue *slot;
    TValue *rb = vRB(i);
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, rb, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, rb, rc, ra, slot));
  }

  // 64	[36]	LOADI    	6 1
  #undef  AOT_PC
  #define AOT_PC (code + 65)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_66
  label_64: {
    aot_vmfetch(0x80000301);
    lua_Integer b = GETARG_sBx(i);
    setivalue(s2v(ra), b);
  }

  // 65	[36]	CALL     	5 2 2	; 1 in 1 out
  #undef  AOT_PC
  #define AOT_PC (code + 66)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_67
  label_65: {
    aot_vmfetch(0x020202c4);
    CallInfo *newci;
    int b = GETARG_B(i);
    int nresults = GETARG_C(i) - 1;
    if (b != 0)  /* fixed number of arguments? */
        L->top = ra + b;  /* top signals number of arguments */
    /* else previous instruction set top */
    savepc(L);  /* in case of errors */
    if ((newci = luaD_precall(L, ra, nresults)) == NULL)
        updatetrap(ci);  /* C call; nothing else to be done */
    else {
        ci = newci;
        ci->callstatus = 0;  /* call re-uses 'luaV_execute' */
        return ci;
    }
  }

  // 66	[36]	SELF     	5 5 13k	; "byte"
  #undef  AOT_PC
  #define AOT_PC (code + 67)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_68
  label_66: {
    aot_vmfetch(0x0d058294);
    const TValue *slot;
    TValue *rb = vRB(i);
    TValue *rc = RKC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    setobj2s(L, ra + 1, rb);
    if (luaV_fastget(L, rb, key, slot, luaH_getstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, rb, rc, ra, slot));
  }

  // 67	[36]	CALL     	5 2 2	; 1 in 1 out
  #undef  AOT_PC
  #define AOT_PC (code + 68)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_69
  label_67: {
    aot_vmfetch(0x020202c4);
    CallInfo *newci;
    int b = GETARG_B(i);
    int nresults = GETARG_C(i) - 1;
    if (b != 0)  /* fixed number of arguments? */
        L->top = ra + b;  /* top signals number of arguments */
    /* else previous instruction set top */
    savepc(L);  /* in case of errors */
    if ((newci = luaD_precall(L, ra, nresults)) == NULL)
        updatetrap(ci);  /* C call; nothing else to be done */
    else {
        ci = newci;
        ci->callstatus = 0;  /* call re-uses 'luaV_execute' */
        return ci;
    }
  }

  // 68	[36]	SETTABLE 	1 2 5
  #undef  AOT_PC
  #define AOT_PC (code + 69)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_141
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_70
  label_68: {
    aot_vmfetch(0x05020090);
    const TValue *slot;
    TValue *rb = vRB(i);  /* key (table is in 'ra') */
    TValue *rc = RKC(i);  /* value */
    lua_Unsigned n;
    if (ttisinteger(rb)  /* fast track for integers? */
        ? (cast_void(n = ivalue(rb)), luaV_fastgeti(L, s2v(ra), n, slot))
        : luaV_fastget(L, s2v(ra), rb, slot, luaH_get)) {
      luaV_finishfastset(L, s2v(ra), slot, rc);
    }
    else
      Protect(luaV_finishset(L, s2v(ra), rb, rc, slot));
  }

  // 69	[37]	JMP      	71	; to 142
  #undef  AOT_PC
  #define AOT_PC (code + 70)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_71
  label_69: {
    aot_vmfetch(0x80002338);
    updatetrap(ci);
    goto label_141;
  }

  // 70	[37]	EQK      	4 14 0	; "["
  #undef  AOT_PC
  #define AOT_PC (code + 71)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_106
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_72
  label_70: {
    aot_vmfetch(0x000e023c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 71	[37]	JMP      	34	; to 107
  #undef  AOT_PC
  #define AOT_PC (code + 72)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_73
  label_71: {
    aot_vmfetch(0x800010b8);
    updatetrap(ci);
    goto label_106;
  }

  // 72	[38]	GETTABLE 	5 1 2
  #undef  AOT_PC
  #define AOT_PC (code + 73)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_74
  label_72: {
    aot_vmfetch(0x0201028c);
    const TValue *slot;
    TValue *rb = vRB(i);
    TValue *rc = vRC(i);
    lua_Unsigned n;
    if (ttisinteger(rc)  /* fast track for integers? */
        ? (cast_void(n = ivalue(rc)), luaV_fastgeti(L, rb, n, slot))
        : luaV_fastget(L, rb, rc, slot, luaH_get)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, rb, rc, ra, slot));
  }

  // 73	[38]	TEST     	5 0
  #undef  AOT_PC
  #define AOT_PC (code + 74)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_78
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_75
  label_73: {
    aot_vmfetch(0x000002c2);
    int cond = !l_isfalse(s2v(ra));
    docondjump();
  }

  // 74	[38]	JMP      	3	; to 79
  #undef  AOT_PC
  #define AOT_PC (code + 75)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_76
  label_74: {
    aot_vmfetch(0x80000138);
    updatetrap(ci);
    goto label_78;
  }

  // 75	[38]	GETTABLE 	5 1 2
  #undef  AOT_PC
  #define AOT_PC (code + 76)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_77
  label_75: {
    aot_vmfetch(0x0201028c);
    const TValue *slot;
    TValue *rb = vRB(i);
    TValue *rc = vRC(i);
    lua_Unsigned n;
    if (ttisinteger(rc)  /* fast track for integers? */
        ? (cast_void(n = ivalue(rc)), luaV_fastgeti(L, rb, n, slot))
        : luaV_fastget(L, rb, rc, slot, luaH_get)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, rb, rc, ra, slot));
  }

  // 76	[38]	EQI      	5 0 0
  #undef  AOT_PC
  #define AOT_PC (code + 77)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_141
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_78
  label_76: {
    aot_vmfetch(0x007f02bd);
    int cond;
    int im = GETARG_sB(i);
    if (ttisinteger(s2v(ra)))
      cond = (ivalue(s2v(ra)) == im);
    else if (ttisfloat(s2v(ra)))
      cond = luai_numeq(fltvalue(s2v(ra)), cast_num(im));
    else
      cond = 0;  /* other types cannot be equal to a number */
    docondjump();
  }

  // 77	[38]	JMP      	63	; to 142
  #undef  AOT_PC
  #define AOT_PC (code + 78)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_79
  label_77: {
    aot_vmfetch(0x80001f38);
    updatetrap(ci);
    goto label_141;
  }

  // 78	[39]	LOADI    	5 1
  #undef  AOT_PC
  #define AOT_PC (code + 79)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_80
  label_78: {
    aot_vmfetch(0x80000281);
    lua_Integer b = GETARG_sBx(i);
    setivalue(s2v(ra), b);
  }

  // 79	[40]	GTI      	5 0 0
  #undef  AOT_PC
  #define AOT_PC (code + 80)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_141
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_81
  label_79: {
    aot_vmfetch(0x007f02c0);
    op_orderI(L, l_gti, luai_numgt, 1, TM_LT);
  }

  // 80	[40]	JMP      	60	; to 142
  #undef  AOT_PC
  #define AOT_PC (code + 81)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_82
  label_80: {
    aot_vmfetch(0x80001db8);
    updatetrap(ci);
    goto label_141;
  }

  // 81	[41]	ADDI     	3 3 1
  #undef  AOT_PC
  #define AOT_PC (code + 82)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_83
  label_81: {
    aot_vmfetch(0x80030195);
    op_arithI(L, l_addi, luai_numadd);
  }

  // 82	[41]	MMBINI   	3 1 6 0	; __add
  #undef  AOT_PC
  #define AOT_PC (code + 83)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_84
  label_82: {
    aot_vmfetch(0x068001af);
    Instruction pi = 0x80030195;  /* original arith. expression */
    int imm = GETARG_sB(i);
    TMS tm = (TMS)GETARG_C(i);
    int flip = GETARG_k(i);
    StkId result = RA(pi);
    Protect(luaT_trybiniTM(L, s2v(ra), imm, flip, result, tm));
  }

  // 83	[42]	GETTABUP 	6 0 1	; _ENV "string"
  #undef  AOT_PC
  #define AOT_PC (code + 84)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_85
  label_83: {
    aot_vmfetch(0x0100030b);
    const TValue *slot;
    TValue *upval = cl->upvals[GETARG_B(i)]->v;
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, upval, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, upval, rc, ra, slot));
  }

  // 84	[42]	GETFIELD 	6 6 2	; "sub"
  #undef  AOT_PC
  #define AOT_PC (code + 85)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_86
  label_84: {
    aot_vmfetch(0x0206030e);
    const TValue *slot;
    TValue *rb = vRB(i);
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, rb, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, rb, rc, ra, slot));
  }

  // 85	[42]	MOVE     	7 0
  #undef  AOT_PC
  #define AOT_PC (code + 86)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_87
  label_85: {
    aot_vmfetch(0x00000380);
    setobjs2s(L, ra, RB(i));
  }

  // 86	[42]	MOVE     	8 3
  #undef  AOT_PC
  #define AOT_PC (code + 87)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_88
  label_86: {
    aot_vmfetch(0x00030400);
    setobjs2s(L, ra, RB(i));
  }

  // 87	[42]	MOVE     	9 3
  #undef  AOT_PC
  #define AOT_PC (code + 88)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_89
  label_87: {
    aot_vmfetch(0x00030480);
    setobjs2s(L, ra, RB(i));
  }

  // 88	[42]	CALL     	6 4 2	; 3 in 1 out
  #undef  AOT_PC
  #define AOT_PC (code + 89)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_90
  label_88: {
    aot_vmfetch(0x02040344);
    CallInfo *newci;
    int b = GETARG_B(i);
    int nresults = GETARG_C(i) - 1;
    if (b != 0)  /* fixed number of arguments? */
        L->top = ra + b;  /* top signals number of arguments */
    /* else previous instruction set top */
    savepc(L);  /* in case of errors */
    if ((newci = luaD_precall(L, ra, nresults)) == NULL)
        updatetrap(ci);  /* C call; nothing else to be done */
    else {
        ci = newci;
        ci->callstatus = 0;  /* call re-uses 'luaV_execute' */
        return ci;
    }
  }

  // 89	[42]	EQK      	6 14 0	; "["
  #undef  AOT_PC
  #define AOT_PC (code + 90)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_94
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_91
  label_89: {
    aot_vmfetch(0x000e033c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 90	[42]	JMP      	3	; to 95
  #undef  AOT_PC
  #define AOT_PC (code + 91)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_92
  label_90: {
    aot_vmfetch(0x80000138);
    updatetrap(ci);
    goto label_94;
  }

  // 91	[43]	ADDI     	5 5 1
  #undef  AOT_PC
  #define AOT_PC (code + 92)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_93
  label_91: {
    aot_vmfetch(0x80050295);
    op_arithI(L, l_addi, luai_numadd);
  }

  // 92	[43]	MMBINI   	5 1 6 0	; __add
  #undef  AOT_PC
  #define AOT_PC (code + 93)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_79
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_94
  label_92: {
    aot_vmfetch(0x068002af);
    Instruction pi = 0x80050295;  /* original arith. expression */
    int imm = GETARG_sB(i);
    TMS tm = (TMS)GETARG_C(i);
    int flip = GETARG_k(i);
    StkId result = RA(pi);
    Protect(luaT_trybiniTM(L, s2v(ra), imm, flip, result, tm));
  }

  // 93	[44]	JMP      	-15	; to 80
  #undef  AOT_PC
  #define AOT_PC (code + 94)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_95
  label_93: {
    aot_vmfetch(0x7ffff838);
    updatetrap(ci);
    goto label_79;
  }

  // 94	[44]	GETTABUP 	6 0 1	; _ENV "string"
  #undef  AOT_PC
  #define AOT_PC (code + 95)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_96
  label_94: {
    aot_vmfetch(0x0100030b);
    const TValue *slot;
    TValue *upval = cl->upvals[GETARG_B(i)]->v;
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, upval, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, upval, rc, ra, slot));
  }

  // 95	[44]	GETFIELD 	6 6 2	; "sub"
  #undef  AOT_PC
  #define AOT_PC (code + 96)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_97
  label_95: {
    aot_vmfetch(0x0206030e);
    const TValue *slot;
    TValue *rb = vRB(i);
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, rb, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, rb, rc, ra, slot));
  }

  // 96	[44]	MOVE     	7 0
  #undef  AOT_PC
  #define AOT_PC (code + 97)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_98
  label_96: {
    aot_vmfetch(0x00000380);
    setobjs2s(L, ra, RB(i));
  }

  // 97	[44]	MOVE     	8 3
  #undef  AOT_PC
  #define AOT_PC (code + 98)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_99
  label_97: {
    aot_vmfetch(0x00030400);
    setobjs2s(L, ra, RB(i));
  }

  // 98	[44]	MOVE     	9 3
  #undef  AOT_PC
  #define AOT_PC (code + 99)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_100
  label_98: {
    aot_vmfetch(0x00030480);
    setobjs2s(L, ra, RB(i));
  }

  // 99	[44]	CALL     	6 4 2	; 3 in 1 out
  #undef  AOT_PC
  #define AOT_PC (code + 100)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_101
  label_99: {
    aot_vmfetch(0x02040344);
    CallInfo *newci;
    int b = GETARG_B(i);
    int nresults = GETARG_C(i) - 1;
    if (b != 0)  /* fixed number of arguments? */
        L->top = ra + b;  /* top signals number of arguments */
    /* else previous instruction set top */
    savepc(L);  /* in case of errors */
    if ((newci = luaD_precall(L, ra, nresults)) == NULL)
        updatetrap(ci);  /* C call; nothing else to be done */
    else {
        ci = newci;
        ci->callstatus = 0;  /* call re-uses 'luaV_execute' */
        return ci;
    }
  }

  // 100	[44]	EQK      	6 15 0	; "]"
  #undef  AOT_PC
  #define AOT_PC (code + 101)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_79
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_102
  label_100: {
    aot_vmfetch(0x000f033c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 101	[44]	JMP      	-23	; to 80
  #undef  AOT_PC
  #define AOT_PC (code + 102)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_103
  label_101: {
    aot_vmfetch(0x7ffff438);
    updatetrap(ci);
    goto label_79;
  }

  // 102	[45]	ADDI     	5 5 -1
  #undef  AOT_PC
  #define AOT_PC (code + 103)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_104
  label_102: {
    aot_vmfetch(0x7e050295);
    op_arithI(L, l_addi, luai_numadd);
  }

  // 103	[45]	MMBINI   	5 1 7 0	; __sub
  #undef  AOT_PC
  #define AOT_PC (code + 104)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_79
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_105
  label_103: {
    aot_vmfetch(0x078002af);
    Instruction pi = 0x7e050295;  /* original arith. expression */
    int imm = GETARG_sB(i);
    TMS tm = (TMS)GETARG_C(i);
    int flip = GETARG_k(i);
    StkId result = RA(pi);
    Protect(luaT_trybiniTM(L, s2v(ra), imm, flip, result, tm));
  }

  // 104	[47]	JMP      	-26	; to 80
  #undef  AOT_PC
  #define AOT_PC (code + 105)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_141
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_106
  label_104: {
    aot_vmfetch(0x7ffff2b8);
    updatetrap(ci);
    goto label_79;
  }

  // 105	[49]	JMP      	35	; to 142
  #undef  AOT_PC
  #define AOT_PC (code + 106)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_107
  label_105: {
    aot_vmfetch(0x80001138);
    updatetrap(ci);
    goto label_141;
  }

  // 106	[49]	EQK      	4 15 0	; "]"
  #undef  AOT_PC
  #define AOT_PC (code + 107)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_141
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_108
  label_106: {
    aot_vmfetch(0x000f023c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 107	[49]	JMP      	33	; to 142
  #undef  AOT_PC
  #define AOT_PC (code + 108)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_109
  label_107: {
    aot_vmfetch(0x80001038);
    updatetrap(ci);
    goto label_141;
  }

  // 108	[50]	GETTABLE 	5 1 2
  #undef  AOT_PC
  #define AOT_PC (code + 109)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_110
  label_108: {
    aot_vmfetch(0x0201028c);
    const TValue *slot;
    TValue *rb = vRB(i);
    TValue *rc = vRC(i);
    lua_Unsigned n;
    if (ttisinteger(rc)  /* fast track for integers? */
        ? (cast_void(n = ivalue(rc)), luaV_fastgeti(L, rb, n, slot))
        : luaV_fastget(L, rb, rc, slot, luaH_get)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, rb, rc, ra, slot));
  }

  // 109	[50]	TEST     	5 0
  #undef  AOT_PC
  #define AOT_PC (code + 110)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_141
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_111
  label_109: {
    aot_vmfetch(0x000002c2);
    int cond = !l_isfalse(s2v(ra));
    docondjump();
  }

  // 110	[50]	JMP      	30	; to 142
  #undef  AOT_PC
  #define AOT_PC (code + 111)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_112
  label_110: {
    aot_vmfetch(0x80000eb8);
    updatetrap(ci);
    goto label_141;
  }

  // 111	[50]	GETTABLE 	5 1 2
  #undef  AOT_PC
  #define AOT_PC (code + 112)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_113
  label_111: {
    aot_vmfetch(0x0201028c);
    const TValue *slot;
    TValue *rb = vRB(i);
    TValue *rc = vRC(i);
    lua_Unsigned n;
    if (ttisinteger(rc)  /* fast track for integers? */
        ? (cast_void(n = ivalue(rc)), luaV_fastgeti(L, rb, n, slot))
        : luaV_fastget(L, rb, rc, slot, luaH_get)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, rb, rc, ra, slot));
  }

  // 112	[50]	EQI      	5 0 1
  #undef  AOT_PC
  #define AOT_PC (code + 113)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_141
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_114
  label_112: {
    aot_vmfetch(0x007f82bd);
    int cond;
    int im = GETARG_sB(i);
    if (ttisinteger(s2v(ra)))
      cond = (ivalue(s2v(ra)) == im);
    else if (ttisfloat(s2v(ra)))
      cond = luai_numeq(fltvalue(s2v(ra)), cast_num(im));
    else
      cond = 0;  /* other types cannot be equal to a number */
    docondjump();
  }

  // 113	[50]	JMP      	27	; to 142
  #undef  AOT_PC
  #define AOT_PC (code + 114)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_115
  label_113: {
    aot_vmfetch(0x80000d38);
    updatetrap(ci);
    goto label_141;
  }

  // 114	[51]	LOADI    	5 1
  #undef  AOT_PC
  #define AOT_PC (code + 115)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_116
  label_114: {
    aot_vmfetch(0x80000281);
    lua_Integer b = GETARG_sBx(i);
    setivalue(s2v(ra), b);
  }

  // 115	[52]	GTI      	5 0 0
  #undef  AOT_PC
  #define AOT_PC (code + 116)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_141
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_117
  label_115: {
    aot_vmfetch(0x007f02c0);
    op_orderI(L, l_gti, luai_numgt, 1, TM_LT);
  }

  // 116	[52]	JMP      	24	; to 142
  #undef  AOT_PC
  #define AOT_PC (code + 117)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_118
  label_116: {
    aot_vmfetch(0x80000bb8);
    updatetrap(ci);
    goto label_141;
  }

  // 117	[53]	ADDI     	3 3 -1
  #undef  AOT_PC
  #define AOT_PC (code + 118)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_119
  label_117: {
    aot_vmfetch(0x7e030195);
    op_arithI(L, l_addi, luai_numadd);
  }

  // 118	[53]	MMBINI   	3 1 7 0	; __sub
  #undef  AOT_PC
  #define AOT_PC (code + 119)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_120
  label_118: {
    aot_vmfetch(0x078001af);
    Instruction pi = 0x7e030195;  /* original arith. expression */
    int imm = GETARG_sB(i);
    TMS tm = (TMS)GETARG_C(i);
    int flip = GETARG_k(i);
    StkId result = RA(pi);
    Protect(luaT_trybiniTM(L, s2v(ra), imm, flip, result, tm));
  }

  // 119	[54]	GETTABUP 	6 0 1	; _ENV "string"
  #undef  AOT_PC
  #define AOT_PC (code + 120)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_121
  label_119: {
    aot_vmfetch(0x0100030b);
    const TValue *slot;
    TValue *upval = cl->upvals[GETARG_B(i)]->v;
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, upval, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, upval, rc, ra, slot));
  }

  // 120	[54]	GETFIELD 	6 6 2	; "sub"
  #undef  AOT_PC
  #define AOT_PC (code + 121)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_122
  label_120: {
    aot_vmfetch(0x0206030e);
    const TValue *slot;
    TValue *rb = vRB(i);
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, rb, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, rb, rc, ra, slot));
  }

  // 121	[54]	MOVE     	7 0
  #undef  AOT_PC
  #define AOT_PC (code + 122)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_123
  label_121: {
    aot_vmfetch(0x00000380);
    setobjs2s(L, ra, RB(i));
  }

  // 122	[54]	MOVE     	8 3
  #undef  AOT_PC
  #define AOT_PC (code + 123)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_124
  label_122: {
    aot_vmfetch(0x00030400);
    setobjs2s(L, ra, RB(i));
  }

  // 123	[54]	MOVE     	9 3
  #undef  AOT_PC
  #define AOT_PC (code + 124)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_125
  label_123: {
    aot_vmfetch(0x00030480);
    setobjs2s(L, ra, RB(i));
  }

  // 124	[54]	CALL     	6 4 2	; 3 in 1 out
  #undef  AOT_PC
  #define AOT_PC (code + 125)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_126
  label_124: {
    aot_vmfetch(0x02040344);
    CallInfo *newci;
    int b = GETARG_B(i);
    int nresults = GETARG_C(i) - 1;
    if (b != 0)  /* fixed number of arguments? */
        L->top = ra + b;  /* top signals number of arguments */
    /* else previous instruction set top */
    savepc(L);  /* in case of errors */
    if ((newci = luaD_precall(L, ra, nresults)) == NULL)
        updatetrap(ci);  /* C call; nothing else to be done */
    else {
        ci = newci;
        ci->callstatus = 0;  /* call re-uses 'luaV_execute' */
        return ci;
    }
  }

  // 125	[54]	EQK      	6 15 0	; "]"
  #undef  AOT_PC
  #define AOT_PC (code + 126)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_130
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_127
  label_125: {
    aot_vmfetch(0x000f033c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 126	[54]	JMP      	3	; to 131
  #undef  AOT_PC
  #define AOT_PC (code + 127)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_128
  label_126: {
    aot_vmfetch(0x80000138);
    updatetrap(ci);
    goto label_130;
  }

  // 127	[55]	ADDI     	5 5 1
  #undef  AOT_PC
  #define AOT_PC (code + 128)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_129
  label_127: {
    aot_vmfetch(0x80050295);
    op_arithI(L, l_addi, luai_numadd);
  }

  // 128	[55]	MMBINI   	5 1 6 0	; __add
  #undef  AOT_PC
  #define AOT_PC (code + 129)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_115
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_130
  label_128: {
    aot_vmfetch(0x068002af);
    Instruction pi = 0x80050295;  /* original arith. expression */
    int imm = GETARG_sB(i);
    TMS tm = (TMS)GETARG_C(i);
    int flip = GETARG_k(i);
    StkId result = RA(pi);
    Protect(luaT_trybiniTM(L, s2v(ra), imm, flip, result, tm));
  }

  // 129	[56]	JMP      	-15	; to 116
  #undef  AOT_PC
  #define AOT_PC (code + 130)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_131
  label_129: {
    aot_vmfetch(0x7ffff838);
    updatetrap(ci);
    goto label_115;
  }

  // 130	[56]	GETTABUP 	6 0 1	; _ENV "string"
  #undef  AOT_PC
  #define AOT_PC (code + 131)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_132
  label_130: {
    aot_vmfetch(0x0100030b);
    const TValue *slot;
    TValue *upval = cl->upvals[GETARG_B(i)]->v;
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, upval, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, upval, rc, ra, slot));
  }

  // 131	[56]	GETFIELD 	6 6 2	; "sub"
  #undef  AOT_PC
  #define AOT_PC (code + 132)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_133
  label_131: {
    aot_vmfetch(0x0206030e);
    const TValue *slot;
    TValue *rb = vRB(i);
    TValue *rc = KC(i);
    TString *key = tsvalue(rc);  /* key must be a string */
    if (luaV_fastget(L, rb, key, slot, luaH_getshortstr)) {
      setobj2s(L, ra, slot);
    }
    else
      Protect(luaV_finishget(L, rb, rc, ra, slot));
  }

  // 132	[56]	MOVE     	7 0
  #undef  AOT_PC
  #define AOT_PC (code + 133)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_134
  label_132: {
    aot_vmfetch(0x00000380);
    setobjs2s(L, ra, RB(i));
  }

  // 133	[56]	MOVE     	8 3
  #undef  AOT_PC
  #define AOT_PC (code + 134)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_135
  label_133: {
    aot_vmfetch(0x00030400);
    setobjs2s(L, ra, RB(i));
  }

  // 134	[56]	MOVE     	9 3
  #undef  AOT_PC
  #define AOT_PC (code + 135)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_136
  label_134: {
    aot_vmfetch(0x00030480);
    setobjs2s(L, ra, RB(i));
  }

  // 135	[56]	CALL     	6 4 2	; 3 in 1 out
  #undef  AOT_PC
  #define AOT_PC (code + 136)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_137
  label_135: {
    aot_vmfetch(0x02040344);
    CallInfo *newci;
    int b = GETARG_B(i);
    int nresults = GETARG_C(i) - 1;
    if (b != 0)  /* fixed number of arguments? */
        L->top = ra + b;  /* top signals number of arguments */
    /* else previous instruction set top */
    savepc(L);  /* in case of errors */
    if ((newci = luaD_precall(L, ra, nresults)) == NULL)
        updatetrap(ci);  /* C call; nothing else to be done */
    else {
        ci = newci;
        ci->callstatus = 0;  /* call re-uses 'luaV_execute' */
        return ci;
    }
  }

  // 136	[56]	EQK      	6 14 0	; "["
  #undef  AOT_PC
  #define AOT_PC (code + 137)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_115
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_138
  label_136: {
    aot_vmfetch(0x000e033c);
    TValue *rb = KB(i);
    /* basic types do not use '__eq'; we can use raw equality */
    int cond = luaV_equalobj(NULL, s2v(ra), rb);
    docondjump();
  }

  // 137	[56]	JMP      	-23	; to 116
  #undef  AOT_PC
  #define AOT_PC (code + 138)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_139
  label_137: {
    aot_vmfetch(0x7ffff438);
    updatetrap(ci);
    goto label_115;
  }

  // 138	[57]	ADDI     	5 5 -1
  #undef  AOT_PC
  #define AOT_PC (code + 139)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_140
  label_138: {
    aot_vmfetch(0x7e050295);
    op_arithI(L, l_addi, luai_numadd);
  }

  // 139	[57]	MMBINI   	5 1 7 0	; __sub
  #undef  AOT_PC
  #define AOT_PC (code + 140)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_115
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_141
  label_139: {
    aot_vmfetch(0x078002af);
    Instruction pi = 0x7e050295;  /* original arith. expression */
    int imm = GETARG_sB(i);
    TMS tm = (TMS)GETARG_C(i);
    int flip = GETARG_k(i);
    StkId result = RA(pi);
    Protect(luaT_trybiniTM(L, s2v(ra), imm, flip, result, tm));
  }

  // 140	[59]	JMP      	-26	; to 116
  #undef  AOT_PC
  #define AOT_PC (code + 141)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_142
  label_140: {
    aot_vmfetch(0x7ffff2b8);
    updatetrap(ci);
    goto label_115;
  }

  // 141	[63]	ADDI     	3 3 1
  #undef  AOT_PC
  #define AOT_PC (code + 142)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_143
  label_141: {
    aot_vmfetch(0x80030195);
    op_arithI(L, l_addi, luai_numadd);
  }

  // 142	[63]	MMBINI   	3 1 6 0	; __add
  #undef  AOT_PC
  #define AOT_PC (code + 143)
  #undef  AOT_NEXT_JUMP
  #define AOT_NEXT_JUMP label_08
  #undef  AOT_SKIP1
  #define AOT_SKIP1 label_144
  label_142: {
    aot_vmfetch(0x068001af);
    Instruction pi = 0x80030195;  /* original arith. expression */
    int imm = GETARG_sB(i);
    TMS tm = (TMS)GETARG_C(i);
    int flip = GETARG_k(i);
    StkId result = RA(pi);
    Protect(luaT_trybiniTM(L, s2v(ra), imm, flip, result, tm));
  }

  // 143	[64]	JMP      	-136	; to 9
  #undef  AOT_PC
  #define AOT_PC (code + 144)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  label_143: {
    aot_vmfetch(0x7fffbbb8);
    updatetrap(ci);
    goto label_08;
  }

  // 144	[65]	RETURN0  	
  #undef  AOT_PC
  #define AOT_PC (code + 145)
  #undef  AOT_NEXT_JUMP
  #undef  AOT_SKIP1
  label_144: {
    aot_vmfetch(0x00010247);
    if (l_unlikely(L->hookmask)) {
      L->top = ra;
      savepc(ci);
      luaD_poscall(L, ci, 0);  /* no hurry... */
      trap = 1;
    }
    else {  /* do the 'poscall' here */
      int nres;
      L->ci = ci->previous;  /* back to caller */
      L->top = base - 1;
      for (nres = ci->nresults; l_unlikely(nres > 0); nres--)
        setnilvalue(s2v(L->top++));  /* all results are nil */
    }
    if (ci->callstatus & CIST_FRESH)
        return NULL;  /* end this frame */
    else {
        ci = ci->previous;
        return ci;
    }
  }

}

static AotCompiledFunction AOT_FUNCTIONS[] = {
  magic_implementation_00,
  magic_implementation_01,
  magic_implementation_02,
  NULL
};

static const char AOT_MODULE_SOURCE_CODE[] = {
  102, 117, 110,  99, 116, 105, 111, 110,  32,  99, 108, 101,  97, 110,  98, 102,
   40,  98, 102,  41, 123,  10,  32,  32,  32,  32,  47,  47,  32,  82, 101, 109,
  111, 118, 101,  32,  97, 108, 108,  32, 110, 111, 110, 101,  58,  10,  32,  32,
   32,  32,  47,  42,  10,  32,  32,  32,  32,  32,  32,  32,  32,  45,  32,  43,
   32,  62,  32,  60,  32,  46,  32,  44,  32,  91,  32,  93,  10,  32,  32,  32,
   32,  42,  47,  10,  32,  32,  32,  32, 118,  97, 114,  32, 110, 101, 119,  98,
  102,  32,  61,  32,  34,  34,  59,  10,  32,  32,  32,  32, 102, 111, 114,  32,
   40, 105,  32,  61,  32,  49,  44,  32,  35,  98, 102,  41, 123,  10,  32,  32,
   32,  32,  32,  32,  32,  32, 118,  97, 114,  32,  99,  32,  61,  32, 115, 116,
  114, 105, 110, 103,  46, 115, 117,  98,  40,  98, 102,  44,  32, 105,  44,  32,
  105,  41,  59,  10,  32,  32,  32,  32,  32,  32,  32,  32, 105, 102,  32,  40,
   99,  32,  61,  61,  32,  34,  45,  34,  32, 124, 124,  32,  99,  32,  61,  61,
   32,  34,  43,  34,  32, 124, 124,  32,  99,  32,  61,  61,  32,  34,  62,  34,
   32, 124, 124,  32,  99,  32,  61,  61,  32,  34,  60,  34,  32, 124, 124,  32,
   99,  32,  61,  61,  32,  34,  46,  34,  32, 124, 124,  32,  99,  32,  61,  61,
   32,  34,  44,  34,  32, 124, 124,  32,  99,  32,  61,  61,  32,  34,  91,  34,
   32, 124, 124,  32,  99,  32,  61,  61,  32,  34,  93,  34,  41, 123,  10,  32,
   32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32, 110, 101, 119,  98, 102,
   32,  61,  32, 110, 101, 119,  98, 102,  46,  46,  99,  59,  10,  32,  32,  32,
   32,  32,  32,  32,  32, 125,  10,  32,  32,  32,  32, 125,  10,  10,  32,  32,
   32,  32, 114, 101, 116, 117, 114, 110,  32, 110, 101, 119,  98, 102,  10, 125,
   10, 102, 117, 110,  99, 116, 105, 111, 110,  32, 105, 110, 116, 101, 114, 112,
  114, 101, 116,  40,  98, 102,  41,  32, 123,  10,  32,  32,  32,  32,  98, 102,
   32,  61,  32,  99, 108, 101,  97, 110,  98, 102,  40,  98, 102,  41,  10,  32,
   32,  32,  32, 118,  97, 114,  32, 109, 101, 109,  32,  61,  32, 123, 125,  59,
   10,  32,  32,  32,  32, 118,  97, 114,  32, 112, 116, 114,  32,  61,  32,  49,
   59,  10,  32,  32,  32,  32, 118,  97, 114,  32, 105,  32,  61,  32,  49,  59,
   10,  10,  32,  32,  32,  32, 119, 104, 105, 108, 101,  40,  32, 105,  32,  60,
   61,  32,  35,  98, 102,  32,  41,  32, 123,  10,  32,  32,  32,  32,  32,  32,
   32,  32, 118,  97, 114,  32,  99,  32,  61,  32, 115, 116, 114, 105, 110, 103,
   46, 115, 117,  98,  40,  98, 102,  44,  32, 105,  44,  32, 105,  41,  59,  10,
   10,  32,  32,  32,  32,  32,  32,  32,  32, 105, 102,  40,  32,  99,  32,  61,
   61,  32,  34,  62,  34,  32,  41,  32, 123,  10,  32,  32,  32,  32,  32,  32,
   32,  32,  32,  32,  32,  32,  43,  43, 112, 116, 114,  32,  32,  32,  32,  59,
   10,  32,  32,  32,  32,  32,  32,  32,  32, 125,  32, 101, 108, 115, 101,  32,
  105, 102,  40,  32,  99,  32,  61,  61,  32,  34,  60,  34,  32,  41,  32, 123,
   10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  45,  45, 112,
  116, 114,  32,  32,  32,  32,  59,  10,  32,  32,  32,  32,  32,  32,  32,  32,
  125,  32, 101, 108, 115, 101,  32, 105, 102,  40,  32,  99,  32,  61,  61,  32,
   34,  43,  34,  32,  41,  32, 123,  10,  32,  32,  32,  32,  32,  32,  32,  32,
   32,  32,  32,  32, 109, 101, 109,  91, 112, 116, 114,  93,  32,  61,  32,  40,
  109, 101, 109,  91, 112, 116, 114,  93,  32, 124, 124,  32,  48,  41,  32,  43,
   32,  49,  59,  10,  32,  32,  32,  32,  32,  32,  32,  32, 125,  32, 101, 108,
  115, 101,  32, 105, 102,  40,  32,  99,  32,  61,  61,  32,  34,  45,  34,  32,
   41,  32, 123,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
  109, 101, 109,  91, 112, 116, 114,  93,  32,  61,  32,  40, 109, 101, 109,  91,
  112, 116, 114,  93,  32, 124, 124,  32,  48,  41,  32,  45,  32,  49,  59,  10,
   32,  32,  32,  32,  32,  32,  32,  32, 125,  32, 101, 108, 115, 101,  32, 105,
  102,  40,  32,  99,  32,  61,  61,  32,  34,  46,  34,  32,  41,  32, 123,  10,
   32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32, 105, 111,  46, 119,
  114, 105, 116, 101,  40, 115, 116, 114, 105, 110, 103,  46,  99, 104,  97, 114,
   40, 109, 101, 109,  91, 112, 116, 114,  93,  32, 124, 124,  32,  48,  41,  41,
   59,  10,  32,  32,  32,  32,  32,  32,  32,  32, 125,  32, 101, 108, 115, 101,
   32, 105, 102,  40,  32,  99,  32,  61,  61,  32,  34,  44,  34,  32,  41,  32,
  123,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32, 109, 101,
  109,  91, 112, 116, 114,  93,  32,  61,  32, 105, 111,  46, 114, 101,  97, 100,
   40,  49,  41,  45,  62,  98, 121, 116, 101,  40,  41,  59,  10,  32,  32,  32,
   32,  32,  32,  32,  32, 125,  32, 101, 108, 115, 101,  32, 105, 102,  40,  32,
   99,  32,  61,  61,  32,  34,  91,  34,  32,  41,  32, 123,  10,  32,  32,  32,
   32,  32,  32,  32,  32,  32,  32,  32,  32, 105, 102,  40,  32,  33,  32, 109,
  101, 109,  91, 112, 116, 114,  93,  32, 124, 124,  32, 109, 101, 109,  91, 112,
  116, 114,  93,  32,  61,  61,  32,  48,  32,  41,  32, 123,  10,  32,  32,  32,
   32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32, 118,  97, 114,
   32,  99, 111, 117, 110, 116,  32,  61,  32,  49,  59,  10,  32,  32,  32,  32,
   32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32, 119, 104, 105, 108,
  101,  40,  32,  99, 111, 117, 110, 116,  32,  62,  32,  48,  32,  41,  32, 123,
   10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
   32,  32,  32,  32,  32,  43,  43, 105,  32,  32,  32,  32,  59,  10,  32,  32,
   32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
   32,  32, 105, 102,  40,  32, 115, 116, 114, 105, 110, 103,  46, 115, 117,  98,
   40,  98, 102,  44,  32, 105,  44,  32, 105,  41,  32,  61,  61,  32,  34,  91,
   34,  32,  41,  32, 123,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
   32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  43,  43,
   99, 111, 117, 110, 116,  32,  32,  32,  32,  59,  10,  32,  32,  32,  32,  32,
   32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32, 125,
   32, 101, 108, 115, 101,  32, 105, 102,  40,  32, 115, 116, 114, 105, 110, 103,
   46, 115, 117,  98,  40,  98, 102,  44,  32, 105,  44,  32, 105,  41,  32,  61,
   61,  32,  34,  93,  34,  32,  41,  32, 123,  10,  32,  32,  32,  32,  32,  32,
   32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
   32,  32,  45,  45,  99, 111, 117, 110, 116,  32,  32,  32,  32,  59,  10,  32,
   32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
   32,  32,  32, 125,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
   32,  32,  32,  32,  32, 125,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
   32,  32,  32, 125,  10,  32,  32,  32,  32,  32,  32,  32,  32, 125,  32, 101,
  108, 115, 101,  32, 105, 102,  40,  32,  99,  32,  61,  61,  32,  34,  93,  34,
   32,  41,  32, 123,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
   32, 105, 102,  40,  32, 109, 101, 109,  91, 112, 116, 114,  93,  32,  38,  38,
   32, 109, 101, 109,  91, 112, 116, 114,  93,  32,  33,  61,  32,  48,  32,  41,
   32, 123,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
   32,  32,  32, 118,  97, 114,  32,  99, 111, 117, 110, 116,  32,  61,  32,  49,
   59,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
   32,  32, 119, 104, 105, 108, 101,  40,  32,  99, 111, 117, 110, 116,  32,  62,
   32,  48,  32,  41,  32, 123,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,
   32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  45,  45, 105,  32,  32,
   32,  32,  59,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
   32,  32,  32,  32,  32,  32,  32,  32, 105, 102,  40,  32, 115, 116, 114, 105,
  110, 103,  46, 115, 117,  98,  40,  98, 102,  44,  32, 105,  44,  32, 105,  41,
   32,  61,  61,  32,  34,  93,  34,  32,  41,  32, 123,  10,  32,  32,  32,  32,
   32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
   32,  32,  32,  32,  43,  43,  99, 111, 117, 110, 116,  32,  32,  32,  32,  59,
   10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
   32,  32,  32,  32,  32, 125,  32, 101, 108, 115, 101,  32, 105, 102,  40,  32,
  115, 116, 114, 105, 110, 103,  46, 115, 117,  98,  40,  98, 102,  44,  32, 105,
   44,  32, 105,  41,  32,  61,  61,  32,  34,  91,  34,  32,  41,  32, 123,  10,
   32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
   32,  32,  32,  32,  32,  32,  32,  32,  45,  45,  99, 111, 117, 110, 116,  32,
   32,  32,  32,  59,  10,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
   32,  32,  32,  32,  32,  32,  32,  32,  32, 125,  10,  32,  32,  32,  32,  32,
   32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32, 125,  10,  32,  32,  32,
   32,  32,  32,  32,  32,  32,  32,  32,  32, 125,  10,  32,  32,  32,  32,  32,
   32,  32,  32, 125,  10,  10,  32,  32,  32,  32,  32,  32,  32,  32,  43,  43,
  105,  32,  32,  32,  32,  59,  10,  32,  32,  32,  32, 125,  10, 125,   0
};

#define aot_LUAOPEN_NAME luaopen_bf

#include "aot_footer.c"

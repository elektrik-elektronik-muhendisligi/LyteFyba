;-------------------------------------------------------------------------------
; Assembler Structured Control-flow Macros
; Dave Keenan, 5-Feb-2010, updated 1-Jan-2018
; <d.keenan@bigpond.net.au>

; In memory of Wil Baden (1928-2016)
; http://www.boston-baden.com/hazel/dad/
; Among Wil's many gifts to the world, is the elegant control-flow implementation of ANS Forth (1994),
; on which this work is based.

; Make a Control-flow Stack (CS) in the assembler so we can implement structured control-flow words
; for assembly, similar to those in higher-level languages.
; It needs to have more elements than the maximum number of cases in any _CASE statement.

_CS_TOP	SET 0
_CS2	SET 0
_CS3	SET 0
_CS4	SET 0
_CS5	SET 0
_CS6	SET 0
_CS7	SET 0
_CS8	SET 0
_CS9	SET 0
_CS10	SET 0
_CS11	SET 0
_CS12	SET 0
_CS_COUNT SET 0

_CS_PUSH MACRO arg
_CS_COUNT SET _CS_COUNT + 1
		IF	_CS_COUNT > 12
			#error "Control flow stack overflow"
		ENDIF
_CS12	SET _CS11
_CS11	SET _CS10
_CS10	SET _CS9
_CS9	SET _CS8
_CS8	SET _CS7
_CS7	SET _CS6
_CS6	SET _CS5
_CS5	SET _CS4
_CS4	SET _CS3
_CS3	SET _CS2
_CS2	SET _CS_TOP
_CS_TOP SET arg
		ENDM

_CS_DROP MACRO
_CS_COUNT SET _CS_COUNT-1
		IF	_CS_COUNT < 0
			#error "Control flow stack underflow"
		ENDIF
_CS_TOP	SET _CS2
_CS2	SET _CS3
_CS3	SET _CS4
_CS4	SET _CS5
_CS5	SET _CS6
_CS6	SET _CS7
_CS7	SET _CS8
_CS8	SET _CS9
_CS9	SET _CS10
_CS10	SET _CS11
_CS11	SET _CS12
_CS12	SET 0
		ENDM

_CS_SWAP MACRO
_CS_TOP SET _CS_TOP^_CS2
_CS2	SET _CS_TOP^_CS2
_CS_TOP SET _CS_TOP^_CS2
		ENDM

; Check that the control flow stack is empty and has not underflowed.
; Use at end of program, or anywhere that control flow structures should all be complete.
_CS_CHECK MACRO
									LSTOUT-
		IF	_CS_COUNT != 0
			#error "Control flow stack is unbalanced"
		ENDIF
									LSTOUT+
									ENDM


; Initialise the variable used to generate unique labels beginning with "_L"
_LABEL_NUM	SET	100			; Can't be zero. Easier to read the listing if they are all 3 digits.


; Increment an assembler variable. Improves readability of macros below.
_INC	MACRO var
var SET var + 1
		ENDM

; Assign an assembler variable the value of an expression. Improves readability of macros below.
_SET	MACRO var, expr
var SET expr
		ENDM


; Generate a label from an integer variable. The label consists of "_L" followed by the
; decimal representation of the integer. Uses a recursive macro.
; The idea of using generated labels came from https://github.com/WestfW/structured_gas
; which was in turn inspired by the first version of my control-flow macros article,
; which did not use computed labels but relied on being able to ORG backwards
; to fill in a jump instruction using DW, once the offset was known.
; The GNU assembler can't ORG backwards.
; But it sure was difficult to come up with a way to generate labels in the IAR assembler.
; In the GNU assembler one can simply use % to evaluate an integer expression as a string
; at macro-expansion time.

_LABEL	MACRO num, str		; "\2" below is equivalent to "str" (2nd argument) but can be concatenated
		IF	num <= 0
									LSTOUT+
_L\2
									LSTOUT-
		ELSE
			IF num % 10 == 0
				_LABEL num / 10, 0\2
			ENDIF				; I want to use the assembler's ELSEIFs here, but they seem to be broken
			IF num % 10 == 1
				_LABEL num / 10, 1\2
			ENDIF
			IF num % 10 == 2
				_LABEL num / 10, 2\2
			ENDIF
			IF num % 10 == 3
				_LABEL num / 10, 3\2
			ENDIF
			IF num % 10 == 4
				_LABEL num / 10, 4\2
			ENDIF
			IF num % 10 == 5
				_LABEL num / 10, 5\2
			ENDIF
			IF num % 10 == 6
				_LABEL num / 10, 6\2
			ENDIF
			IF num % 10 == 7
				_LABEL num / 10, 7\2
			ENDIF
			IF num % 10 == 8
				_LABEL num / 10, 8\2
			ENDIF
			IF num % 10 == 9
				_LABEL num / 10, 9\2
			ENDIF
		ENDIF
		ENDM

; Assemble a (possibly conditional) jump instruction, generating the label from an integer variable.
_JUMP	MACRO cond, num, str	; "\3" below is equivalent to "str" (3rd argument) but can be concatenated
		IF	num <= 0
									LSTOUT+
		J\1	 _L\3
									LSTOUT-
		ELSE
			IF num % 10 == 0
				_JUMP cond,num / 10, 0\3
			ENDIF
			IF num % 10 == 1
				_JUMP cond,num / 10, 1\3
			ENDIF
			IF num % 10 == 2
				_JUMP cond,num / 10, 2\3
			ENDIF
			IF num % 10 == 3
				_JUMP cond,num / 10, 3\3
			ENDIF
			IF num % 10 == 4
				_JUMP cond,num / 10, 4\3
			ENDIF
			IF num % 10 == 5
				_JUMP cond,num / 10, 5\3
			ENDIF
			IF num % 10 == 6
				_JUMP cond,num / 10, 6\3
			ENDIF
			IF num % 10 == 7
				_JUMP cond,num / 10, 7\3
			ENDIF
			IF num % 10 == 8
				_JUMP cond,num / 10, 8\3
			ENDIF
			IF num % 10 == 9
				_JUMP cond,num / 10, 9\3
			ENDIF
		ENDIF
		ENDM

; Translate the jump instructions generated by _JUMP above, when "not" is placed before the condition code.
; Used by _IF and _UNTIL.

JnotZ	MACRO	label
		JNZ		label
									ENDM
JnotNZ	MACRO	label
		JZ		label
									ENDM
JnotEQ	MACRO	label
		JNE		label
									ENDM
JnotNE	MACRO	label
		JEQ		label
									ENDM
JnotHS	MACRO	label
		JLO		label
									ENDM
JnotC	MACRO	label
		JNC		label
									ENDM
JnotNC	MACRO	label
		JC		label
									ENDM
JnotLO	MACRO	label
		JHS		label
									ENDM
JnotN	MACRO	label 				; MSP430 specific.
		JN		$+4					; The best substitute for the non-existent JNN instruction
		JMP		label				; Thanks to Anders Lindgren
									ENDM
JnotNN	MACRO	label
		JN		label
									ENDM
JnotL	MACRO	label
		JGE		label
									ENDM
JnotGE	MACRO	label
		JL		label
									ENDM
JnotNEVER MACRO	label				; An unconditional jump
		JMP		label
									ENDM

;------------------------------------------------------------
; Define macros for simple conditionals

;		<test>
;		_IF cc
;			<stuff to do if cc>
;		_ENDIF

;		<test>
;		_IF cc
;			<stuff to do if cc>
;		_ELSE
;			<stuff to do if NOT cc>
;		_ENDIF
;
; Where cc is one of Z, NZ, EQ, NE, C, NC, HS, LO, N, NN, L, GE or NEVER.


; Mark the origin of a forward conditional branch.
; Called by _WHILE.

_IF 	MACRO cond		; "\1" below is equivalent to "cond" (1st argument) but can be concatenated
									LSTOUT-
		_JUMP		not\1, _LABEL_NUM ; Assemble a conditional jump with the opposite condition
		_CS_PUSH	_LABEL_NUM		; Push its label number
		_INC		_LABEL_NUM		; Increment the label number
									LSTOUT+
									ENDM

; Resolve a forward branch due to most recent _AHEAD, _IF, _ELSE or _WHILE.
; Called by _ELSE and _ENDW.

_ENDIF	MACRO
									LSTOUT-
		_LABEL		_CS_TOP			; Assemble the label for the previous _IF.
		_CS_DROP					; Drop its label number off the control-flow stack
									LSTOUT+
									ENDM

; Mark the origin of a forward unconditional branch and
; resolve a forward branch due to an _IF.

_ELSE	MACRO
									LSTOUT-
		_IF			NEVER			; Assemble an unconditional jump and push its label number
									LSTOUT-
		_CS_SWAP					; Get the original _IF label number back on top
		_ENDIF						; Assemble the label for the previous _IF, and drop its number
									ENDM


;------------------------------------------------------------
; Define macros for uncounted loops

;		_DO
;			<do_stuff>
;		_AGAIN				(infinite)

;		_DO
;			<do_stuff>
;			<test>
;		_UNTIL cc			(post-tested

;		_DO
;			<test>
;		_WHILE cc			(pre-tested)
;			<do_stuff>
;		_ENDW

;		_DO
;			<do_stuff>
;			<test>
;		_WHILE cc			(mid tested)
;			<do_other_stuff>
;		_ENDW


; Mark a backward destination (i.e. the start of a loop)

_DO  MACRO
									LSTOUT-
		_LABEL		_LABEL_NUM		; Assemble the label
		_CS_PUSH	_LABEL_NUM		; Push the number of the label to jump back to
		_INC		_LABEL_NUM		; Increment the label number
									LSTOUT+
									ENDM

; Resolve most recent _DO with a backward conditional branch
; The end of a post-tested loop

_UNTIL	MACRO cond
									LSTOUT-
		_JUMP		not\1, _CS_TOP	; Assemble a conditional jump back to the corresponding _DO
		_CS_DROP					; Drop its label number off the control-flow stack
									LSTOUT+
									ENDM

; Resolve most recent _DO with a backward unconditional branch
; The end of an infinite loop

_AGAIN  MACRO
									LSTOUT-
		_UNTIL		NEVER			; Assemble an unconditional jump back to the corresponding _DO
									ENDM

; Mark the origin of a forward conditional branch out of a loop
; The test of a pre-tested or mid-tested loop

_WHILE	MACRO cond
									LSTOUT-
		_IF			cond			; Assemble a conditional jump and push its label number
									LSTOUT-
		_CS_SWAP					; Get the _DO label number back on top
									LSTOUT+
									ENDM

; Resolve most recent _DO with a backward unconditional branch and
; resolve a forward branch due to most recent _WHILE.
; The end of a pre-tested or mid-tested loop

_ENDW	MACRO
									LSTOUT-
		_UNTIL		NEVER			; Assemble an unconditional jump back to the corresponding _DO
									LSTOUT-
		_ENDIF		  				; Assemble the label for the last _WHILE
									ENDM


; Any loop may have additional _WHILEs to exit it, but each additional one must be
; balanced by an _ENDIF (or _ELSE ... _ENDIF) after the end of the loop. Examples:
; _DO ... _WHILE cc1 ... _WHILE cc2  ... _ENDW  ... _ENDIF
; _DO ... _WHILE cc1 ... _UNTIL cc2  ... _ELSE  ... _ENDIF
; _DO ... _WHILE cc1 ... _WHILE cc2  ... _AGAIN ... _ENDIF ... _ENDIF
;
; See http://www.taygeta.com/forth/dpansa3.htm#A.3.2.3.2


;------------------------------------------------------------
; Short-circuit conditionals

; IF cond1 && cond2 && cond3 ... ENDIF
; is written as
;
;		_COND
;			<test1>
;		_AND_IF		cc1
;			<test2>
;		_AND_IF		cc2
;			<test3>
;		_AND_IF		cc3
;			<stuff to do when cond1 && cond2 && cond3>
;		_ENDIFS				; Note plural _ENDIFS when there is no ELSES clause
;
; Note: COND ... ENDIFS is equivalent to COND ... THENS which, although it is not standard ANS Forth,
; is also due to Wil Baden.

; IF cond1 && cond2 && cond3 ... ELSE ... ENDIF
; is written as
;
;		_COND
;			<test1>
;		_AND_IF		cc1
;			<test2>
;		_AND_IF		cc2
;			<test3>
;		_AND_IF		cc3
;			<stuff to do when cond1 && cond2 && cond3>
;		_ELSES				; Note plural _ELSES
;			<stuff to do when NOT (cond1 && cond2 && cond3)>
;		_ENDIF				; Note singular _ENDIF when there is an ELSES clause

; IF cond1 || cond2 || cond3 ... ENDIF
; is written as
;
;		_COND
;			<test1>
;		_OR_ELSE	cc1
;			<test2>
;		_OR_ELSE	cc2
;			<test3>
;		_OR_IFS 	cc3		; Note plural _OR_IFS for last condition
;			<stuff to do when cond1 || cond2 || cond3>
;		_ENDIF

; IF cond1 || cond2 || cond3 ... ELSE ... ENDIF
; is written as
;
;		_COND
;			<test1>
;		_OR_ELSE	cc1
;			<test2>
;		_OR_ELSE	cc2
;			<test3>
;		_OR_IFS 	cc3		; Note plural _OR_IFS for last condition
;			<stuff to do when cond1 || cond2 || cond3>
;		_ELSE
;			<stuff to do when NOT (cond1 || cond2 || cond3)>
;		_ENDIF


; Mark the origin of a forward conditional branch with the opposite condition.
; Aliased to _OR_ELSE.

_IF_NOT MACRO cond
									LSTOUT-
		_JUMP		cond, _LABEL_NUM ; Assemble a conditional jump with the same condition
		_CS_PUSH	_LABEL_NUM		; Push its label number
		_INC		_LABEL_NUM		; Increment the label number
									LSTOUT+
									ENDM

; Translate the jump instruction generated by _JUMP above, when the condition code is NN.
; Used by _OR_ELSE. MSP430 specific.

JNN		MACRO	label
		JN		$+4					; The best substitute for the non-existent JNN instruction
		JMP		label				; Thanks to Anders Lindgren
									ENDM

; Used by ELSES and OR_IFS below

_END_PRIOR_IF	MACRO
									LSTOUT-
		_CS_SWAP
		_ENDIF
									ENDM

; Begin a short-circuit conditional of either type.
; Aliased as _CASE.

_COND	MACRO
									LSTOUT-
		_CS_PUSH	0				; Push a zero marker onto the control flow stack
									LSTOUT+
									ENDM

; Short circuit AND condition. Counted _IF.
; Aliased as _OF.

_AND_IF	MACRO cond
									LSTOUT-
		_IF cond
									ENDM

; Resolve all _IFs or _ELSEs except the most recent one, back to the most recent _COND.
; Called by _ELSES.

_END_PRIOR_IFS MACRO
									LSTOUT-
		_CS_SWAP
		IF	_CS_TOP == 0
			_CS_DROP
		ELSE
			_ENDIF
									LSTOUT-
	 		_END_PRIOR_IFS			; Recursive
									LSTOUT-
		ENDIF
									LSTOUT+
									ENDM

; Used in place of _ELSE for short-circuit AND.
; Does an _ELSE, then ends all _IFs except the most recent one, back to the most recent _COND.

_ELSES	MACRO
									LSTOUT-
		_ELSE						; Assemble an _ELSE
									LSTOUT-
		_END_PRIOR_IFS				; Resolve all prior _IFs back to _COND
									ENDM

; Used in place of ENDIF for short-circuit AND, but only when there is no ELSES clause.
; Resolves multiple _IFs or _ELSEs.
; Aliased as _ENDCASE.

_ENDIFS	MACRO
									LSTOUT-
		IF	_CS_TOP == 0
			_CS_DROP
		ELSE
			_ENDIF
									LSTOUT-
			_ENDIFS					; Recursive
									LSTOUT-
		ENDIF
									LSTOUT+
									ENDM

; Short circuit OR condition, except last

_OR_ELSE MACRO cond
									LSTOUT-
		_IF_NOT cond
									ENDM

; Last short-circuit OR condition

_OR_IFS	MACRO cond
									LSTOUT-
		_IF	cond					; Assemble an _IF
									LSTOUT-
		_END_PRIOR_IFS				; Resolve all prior _IFs back to _COND
									ENDM


;------------------------------------------------------------
; CASE statement macros

; Typical use:

;	   _CASE
;          <test>
;		   _OF cc
;			   ...
;		   _ENDOF
;
;		   _OF_EQ src, dest			; OF_EQ uses CMP (word comparison)
;				...
;		   _ENDOF
;
;		   _OF_EQ_B src, dest		; OF_EQ_B uses CMP.B (byte comparison)
;			   ...
;		   _ENDOF
;		   ... ( default case )
;	   _ENDCASE

; IF cond1 ... ELSEIF cond2 ... ELSEIF cond3 ... ELSE ... ENDIF
; is written as
;
;	   _CASE
;          <test1>
;		   _OF cc1
;			   ...
;		   _ENDOF
;          <test2>
;		   _OF cc2
;			   ...
;		   _ENDOF
;          <test3>
;		   _OF cc3
;			   ...
;		   _ENDOF
;		   ...
;	   _ENDCASE


_CASE   MACRO
									LSTOUT-
		_COND			 			; Push a zero marker onto the control flow stack
									ENDM

_OF		MACRO cond
									LSTOUT-
		_IF cond					; Assemble an _IF
									ENDM

_OF_EQ	MACRO src, dest			; src is usually #N, dest can be Rn, X(Rn), &ADDR, ADDR
									LSTOUT-
		cmp		src, dest
		_OF EQ						; Assemble an _IF EQ
									ENDM

_OF_EQ_B MACRO src, dest		; src is usually #N, dest can be Rn, X(Rn), &ADDR, ADDR
									LSTOUT-
		cmp.b	src, dest
		_OF EQ						; Assemble an _IF EQ
									ENDM

_ENDOF  MACRO
									LSTOUT-
		_ELSE						; Resolve the previous cond'l jump and assemble an uncond'l jump
									ENDM

_ENDCASE MACRO
									LSTOUT-
		_ENDIFS	  					; Resolve all the uncoditional jumps from the _ENDOFs
									ENDM


;------------------------------------------------------------
; Counted loops

;	_FOR src, dest
;		...					; dest = src down to 1   (src 0 = 65536)
;	_NEXT_DEC dest

;	_FOR src, dest			; src must be even
;		...					; dest = src down to 2 in steps of 2   (src 0 = 65536)
;	_NEXT_DECD dest


_FOR	MACRO src, dest
		mov src, dest
									LSTOUT-
		_DO
									ENDM

_NEXT_DEC MACRO dest
		dec   dest
									LSTOUT-
		_UNTIL Z
									ENDM

_NEXT_DECD MACRO dest
		decd   dest
									LSTOUT-
		_UNTIL Z
									ENDM

; Byte versions of the above

;	_FOR_B src, dest
;		...					; dest = src down to 1   (src 0 = 256)
;	_NEXT_DEC_B dest

;	_FOR_B src, dest		; src must be even
;		...					; dest = src down to 2 in steps of 2   (src 0 = 256)
;	_NEXT_DECD_B dest


_FOR_B	MACRO src, dest
		mov.b src, dest
									LSTOUT-
		_DO
									ENDM

_NEXT_DEC_B  MACRO dest
		dec.b dest
									LSTOUT-
		_UNTIL Z
									ENDM

_NEXT_DECD_B MACRO dest
		decd.b dest
									LSTOUT-
		_UNTIL Z
									ENDM


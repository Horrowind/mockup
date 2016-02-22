all:
	bison -dv nuts.y
	flex nuts.l
	gcc -Ofast nuts.tab.c lex.yy.c -o nuts
test:
	./nuts ../../../../SMW/SMWIRQ/bank_00.asm
	./nuts ../../../../SMW/SMWIRQ/bank_01.asm
	./nuts ../../../../SMW/SMWIRQ/bank_02.asm
	./nuts ../../../../SMW/SMWIRQ/bank_03.asm
	./nuts ../../../../SMW/SMWIRQ/bank_04.asm
	./nuts ../../../../SMW/SMWIRQ/bank_05.asm
	./nuts ../../../../SMW/SMWIRQ/bank_06.asm
	./nuts ../../../../SMW/SMWIRQ/bank_07.asm
	./nuts ../../../../SMW/SMWIRQ/bank_08.asm
	./nuts ../../../../SMW/SMWIRQ/bank_09.asm
	./nuts ../../../../SMW/SMWIRQ/bank_0A.asm
	./nuts ../../../../SMW/SMWIRQ/bank_0B.asm
	./nuts ../../../../SMW/SMWIRQ/bank_0C.asm
	./nuts ../../../../SMW/SMWIRQ/bank_0D.asm
	./nuts ../../../../SMW/SMWIRQ/bank_0F.asm

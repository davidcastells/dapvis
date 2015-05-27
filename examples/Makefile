all: adi.exe trmm.exe trmm_opt.exe atax.exe fdtd-apml.exe linpack.exe sum2d.exe

clean:
	rm -fr *.exe
	rm -fr *.log
	
adi.exe: adi.c
	gcc -o adi.exe adi.c mem_trace.c
	
trmm.exe: trmm.c
	gcc -o trmm.exe trmm.c mem_trace.c

trmm_opt.exe: trmm_opt.c
	gcc -o trmm_opt.exe trmm_opt.c mem_trace.c

linpack.exe: linpack.c
	gcc -o linpack.exe -std=c99 linpack.c mem_trace.c -DDP -DROLL -DGTODay -DORDER=20

atax.exe: ataxTraces.c
	gcc -o atax.exe -std=c99 ataxTraces.c mem_trace.c

sum2d.exe: sum2d.c
	gcc -o sum2d.exe -std=c99 sum2d.c mem_trace.c
	
fdtd-apml.exe: fdtd-apml.c
	gcc -o fdtd-apml.exe -std=c99 fdtd-apml.c mem_trace.c
	
test_atax:
	rm -fr mem.log
	rm -fr atax.mem.log
	./atax.exe
	mv mem.log atax.mem.log

test_fdtd-apml:
	rm -fr mem.log
	rm -fr fdtd-apml.mem.log
	./fdtd-apml.exe
	mv mem.log fdtd-apml.mem.log

test_trmm:
	rm -fr mem.log
	rm -fr trmm.mem.log
	./trmm.exe
	mv mem.log trmm.mem.log

test_trmm_opt:
	rm -fr mem.log
	rm -fr trmm_opt.mem.log
	./trmm_opt.exe
	mv mem.log trmm_opt.mem.log
	
test_sum2d:
	rm -fr mem.log
	rm -fr sum2d.mem.log
	./sum2d.exe
	mv mem.log sum2d.mem.log

test_linpack:
	rm -fr mem.log
	rm -fr linpack.mem.log
	./linpack.exe
	mv mem.log linpack.mem.log


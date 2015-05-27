#include "mem_trace.h"
#include <stdio.h>

int logEnabled = 0;
FILE* f = NULL;

void logIntValue(char* key, char* var, int v)
{
	if (!logEnabled)
		return;
	
	if (f == NULL)
		mem_trace_open();
	
	fprintf(f, key);
	fprintf(f, "-");
	fprintf(f, var);
	fprintf(f, "=");
	fprintf(f, "%d", v);
	fprintf(f, "\n");
	
}

void logStrValue(char* key, char* var, char* v)
{
	if (!logEnabled)
		return;
	
	if (f == NULL)
		mem_trace_open();
	
	fprintf(f, key);
	fprintf(f, "-");
	fprintf(f, var);
	fprintf(f, "=");
	fprintf(f, v);
	fprintf(f, "\n");

}

void logStr(char* key, char* var)
{
	if (!logEnabled)
		return;
	
	if (f == NULL)
		mem_trace_open();
	
	fprintf(f, key);
	fprintf(f, "-");
	fprintf(f, var);
	fprintf(f, "\n");

}

void mem_trace_iter_start(char* var, int v)
{
	logIntValue("IS", var, v);
}

void mem_trace_iter_end(char* var, int v)
{
	logIntValue("IE", var, v);
}

void mem_trace_loop_start(char* var)
{
	logStr("LS", var);
}

void mem_trace_loop_end(char* var)
{
	logStr("LE", var);
}

void mem_trace_set_alias(char* v, char* w )
{
	logStrValue("al", v, w);
}

void mem_trace_read(char* v, void* idx)
{
	logIntValue("MR", v, idx);
}

void mem_trace_write(char* v, void* idx)
{
	logIntValue("MW", v, idx);
}

void mem_trace_set_offset(char* v, int off)
{
	logIntValue("Of", v, off);
}

void mem_trace_def_mem(char* var, void* ptr, int typeSize, int varSize)
{
	char str[20];
	sprintf(str, "%08X,%d,%d", (int) ptr, typeSize, varSize);
	logStrValue("dm", var, str);
}
   

void mem_trace_def_array2d(char* v, int d1, int d2)
{
	char dim[10];
	sprintf(dim, "%dx%d", d1, d2);
	logStrValue("Ad", v, dim);
}

void mem_trace_def_array3d(char* v, int d1, int d2, int d3)
{
	char dim[10];
	sprintf(dim, "%dx%d", d1, d2*d3);
	logStrValue("Ad", v, dim);
}

void mem_trace_def_array1d(char* v, int d1)
{
	char dim[10];
	sprintf(dim, "%dx%d", d1, 1);
	logStrValue("Ad", v, dim);
}

void mem_trace_enable(int v)
{
	logEnabled = v;
}


void mem_trace_open()
{
	f = fopen("mem.log", "a");
}

void mem_trace_close()
{
	fclose(f);
	f = NULL;
}

void mem_trace_flush(int v)
{
	mem_trace_close();
}
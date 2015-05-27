#ifndef MEM_TRACE_H_INCLUDED
#define MEM_TRACE_H_INCLUDED

void mem_trace_iter_start(char* var, int v);
void mem_trace_iter_end(char* var, int v);
void mem_trace_loop_start(char* var);
void mem_trace_loop_end(char* var);
void mem_trace_enable(int v);
void mem_trace_set_alias(char* v, char* w );
void mem_trace_read(char* v, void* idx);
void mem_trace_write(char* v, void* idx);
void mem_trace_def_array2d(char* v, int d1, int d2);
void mem_trace_def_array3d(char* v, int d1, int d2, int d3);
void mem_trace_set_offset(char* v, int off);
void mem_trace_def_mem(char* var, void* ptr, int typeSize, int varSize);


#endif
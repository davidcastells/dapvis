#include <string.h>
#include <math.h>

//#define PROBLEM_SIZE 500
#define PROBLEM_SIZE 15

double ch;
double mui;
double Ex[PROBLEM_SIZE + 1][PROBLEM_SIZE + 1][PROBLEM_SIZE + 1];
double Ey[PROBLEM_SIZE + 1][PROBLEM_SIZE + 1][PROBLEM_SIZE + 1];
double Bza[PROBLEM_SIZE + 1][PROBLEM_SIZE + 1][PROBLEM_SIZE + 1];
double Hz[PROBLEM_SIZE + 1][PROBLEM_SIZE + 1][PROBLEM_SIZE + 1];
double clf[PROBLEM_SIZE + 1][PROBLEM_SIZE + 1];
double tmp[PROBLEM_SIZE + 1][PROBLEM_SIZE + 1];
double Ry[PROBLEM_SIZE + 1][PROBLEM_SIZE + 1];
double Ax[PROBLEM_SIZE + 1][PROBLEM_SIZE + 1];
double cymh[PROBLEM_SIZE + 1];
double cyph[PROBLEM_SIZE + 1];
double cxmh[PROBLEM_SIZE + 1];
double cxph[PROBLEM_SIZE + 1];
double czm[PROBLEM_SIZE + 1];
double czp[PROBLEM_SIZE + 1];
static void init_array()
{
    int i, j, k;
    mui = 2341;
    ch = 42;
    for (i = 0;
        i <= PROBLEM_SIZE;
        )
    {
        czm[i] = ((double) i + 1) / PROBLEM_SIZE;
        czp[i] = ((double) i + 2) / PROBLEM_SIZE;
        i++;
    }
    for (i = 0;
        i <= PROBLEM_SIZE;
        )
    {
        cxmh[i] = ((double) i + 3) / PROBLEM_SIZE;
        cxph[i] = ((double) i + 4) / PROBLEM_SIZE;
        i++;
    }
    for (i = 0;
        i <= PROBLEM_SIZE;
        )
    {
        cymh[i] = ((double) i + 5) / PROBLEM_SIZE;
        cyph[i] = ((double) i + 6) / PROBLEM_SIZE;
        i++;
    }
    for (i = 0;
        i <= PROBLEM_SIZE;
        )
    {
        for (j = 0;
            j <= PROBLEM_SIZE;
            )
        {
            Ry[i][j] = ((double) i * j + 10) / PROBLEM_SIZE;
            Ax[i][j] = ((double) i * j + 11) / PROBLEM_SIZE;
            for (k = 0;
                k <= PROBLEM_SIZE;
                )
            {
                Ex[i][j][k] = ((double) i * (j + 1) + k + 1) / PROBLEM_SIZE;
                Ey[i][j][k] = ((double) (i - 4) * j + k + 2) / PROBLEM_SIZE;
                Hz[i][j][k] = ((double) (i + 3) * (j + 7) + k + 3) / PROBLEM_SIZE;
                k++;
            }
            j++;
        }
        i++;
    }
}
int main(int argc, char **argv)
{
    int iz, iy, ix;
    int Cz = PROBLEM_SIZE;
    int Cym = PROBLEM_SIZE;
    int Cxm = PROBLEM_SIZE;
    init_array();
    mem_trace_enable(1);
    mem_trace_def_array2d("clf", PROBLEM_SIZE + 1, PROBLEM_SIZE + 1);
    mem_trace_def_mem("clf", &clf, sizeof(double), sizeof (clf));
    mem_trace_def_array3d("Ex", PROBLEM_SIZE + 1, PROBLEM_SIZE + 1, PROBLEM_SIZE + 1);
    mem_trace_def_mem("Ex", &Ex, sizeof(double), sizeof (Ex));
    mem_trace_def_array3d("Ey", PROBLEM_SIZE + 1, PROBLEM_SIZE + 1, PROBLEM_SIZE + 1);
    mem_trace_def_mem("Ey", &Ey, sizeof(double), sizeof (Ey));
    mem_trace_def_array2d("tmp", PROBLEM_SIZE + 1, PROBLEM_SIZE + 1);
    mem_trace_def_mem("tmp", &tmp, sizeof(double), sizeof (tmp));
    mem_trace_def_array1d("cymh", PROBLEM_SIZE + 1);
    mem_trace_def_mem("cymh", &cymh, sizeof(double), sizeof (cymh));
    mem_trace_def_array1d("cyph", PROBLEM_SIZE + 1);
    mem_trace_def_mem("cyph", &cyph, sizeof(double), sizeof (cyph));
    mem_trace_def_array3d("Bza", PROBLEM_SIZE + 1, PROBLEM_SIZE + 1, PROBLEM_SIZE + 1);
    mem_trace_def_mem("Bza", &Bza, sizeof(double), sizeof (Bza));
    mem_trace_def_array1d("cxmh", PROBLEM_SIZE + 1);
    mem_trace_def_mem("cxmh", &cxmh, sizeof(double), sizeof (cxmh));
    mem_trace_def_array1d("cxph", PROBLEM_SIZE + 1);
    mem_trace_def_mem("cxph", &cxph, sizeof(double), sizeof (cxph));
    mem_trace_def_array3d("Hz", PROBLEM_SIZE + 1, PROBLEM_SIZE + 1, PROBLEM_SIZE + 1);
    mem_trace_def_mem("Hz", &Hz, sizeof(double), sizeof (Hz));
    mem_trace_def_array1d("czp", PROBLEM_SIZE + 1);
    mem_trace_def_mem("czp", &czp, sizeof(double), sizeof (czp));
    mem_trace_def_array1d("czm", PROBLEM_SIZE + 1);
    mem_trace_def_mem("czm", &czm, sizeof(double), sizeof (czm));
    mem_trace_def_array2d("Ry", PROBLEM_SIZE + 1, PROBLEM_SIZE + 1);
    mem_trace_def_mem("Ry", &Ry, sizeof(double), sizeof (Ry));
    mem_trace_def_array2d("Ax", PROBLEM_SIZE + 1, PROBLEM_SIZE + 1);
    mem_trace_def_mem("Ax", &Ax, sizeof(double), sizeof (Ax));
    for (iz = 0;
        iz < Cz;
        iz++)
    {
        mem_trace_iter_start("iz", iz);
        {
            for (iy = 0;
                iy < Cym;
                iy++)
            {
                mem_trace_iter_start("iy", iy);
                {
                    for (ix = 0;
                        ix < Cxm;
                        ix++)
                    {
                        mem_trace_iter_start("ix", ix);
                        {
                            mem_trace_write("clf", &clf[iz][iy]);
                            mem_trace_read("Ex", &Ex[iz][iy][ix]);
                            mem_trace_read("Ex", &Ex[iz][iy + 1][ix]);
                            mem_trace_read("Ey", &Ey[iz][iy][ix + 1]);
                            mem_trace_read("Ey", &Ey[iz][iy][ix]);
                            clf[iz][iy] = Ex[iz][iy][ix] - Ex[iz][iy + 1][ix] + Ey[iz][iy][ix + 1] - Ey[iz][iy][ix];
                            mem_trace_write("tmp", &tmp[iz][iy]);
                            mem_trace_read("cymh", &cymh[iy]);
                            mem_trace_read("cyph", &cyph[iy]);
                            mem_trace_read("Bza", &Bza[iz][iy][ix]);
                            mem_trace_read("cyph", &cyph[iy]);
                            mem_trace_read("clf", &clf[iz][iy]);
                            tmp[iz][iy] = (cymh[iy] / cyph[iy]) * Bza[iz][iy][ix] - (ch / cyph[iy]) * clf[iz][iy];
                            mem_trace_write("Hz", &Hz[iz][iy][ix]);
                            mem_trace_read("cxmh", &cxmh[ix]);
                            mem_trace_read("cxph", &cxph[ix]);
                            mem_trace_read("Hz", &Hz[iz][iy][ix]);
                            mem_trace_read("czp", &czp[iz]);
                            mem_trace_read("cxph", &cxph[ix]);
                            mem_trace_read("tmp", &tmp[iz][iy]);
                            mem_trace_read("czm", &czm[iz]);
                            mem_trace_read("cxph", &cxph[ix]);
                            mem_trace_read("Bza", &Bza[iz][iy][ix]);
                            Hz[iz][iy][ix] = (cxmh[ix] / cxph[ix]) * Hz[iz][iy][ix] + (mui * czp[iz] / cxph[ix]) * tmp[iz][iy] - (mui * czm[iz] / cxph[ix]) * Bza[iz][iy][ix];
                            mem_trace_write("Bza", &Bza[iz][iy][ix]);
                            mem_trace_read("tmp", &tmp[iz][iy]);
                            Bza[iz][iy][ix] = tmp[iz][iy];
                        }
                        mem_trace_iter_end("ix", ix);
                    }
                    mem_trace_write("clf", &clf[iz][iy]);
                    mem_trace_read("Ex", &Ex[iz][iy][Cxm]);
                    mem_trace_read("Ex", &Ex[iz][iy + 1][Cxm]);
                    mem_trace_read("Ry", &Ry[iz][iy]);
                    mem_trace_read("Ey", &Ey[iz][iy][Cxm]);
                    clf[iz][iy] = Ex[iz][iy][Cxm] - Ex[iz][iy + 1][Cxm] + Ry[iz][iy] - Ey[iz][iy][Cxm];
                    mem_trace_write("tmp", &tmp[iz][iy]);
                    mem_trace_read("cymh", &cymh[iy]);
                    mem_trace_read("cyph", &cyph[iy]);
                    mem_trace_read("Bza", &Bza[iz][iy][Cxm]);
                    mem_trace_read("cyph", &cyph[iy]);
                    mem_trace_read("clf", &clf[iz][iy]);
                    tmp[iz][iy] = (cymh[iy] / cyph[iy]) * Bza[iz][iy][Cxm] - (ch / cyph[iy]) * clf[iz][iy];
                    mem_trace_write("Hz", &Hz[iz][iy][Cxm]);
                    mem_trace_read("cxmh", &cxmh[Cxm]);
                    mem_trace_read("cxph", &cxph[Cxm]);
                    mem_trace_read("Hz", &Hz[iz][iy][Cxm]);
                    mem_trace_read("czp", &czp[iz]);
                    mem_trace_read("cxph", &cxph[Cxm]);
                    mem_trace_read("tmp", &tmp[iz][iy]);
                    mem_trace_read("czm", &czm[iz]);
                    mem_trace_read("cxph", &cxph[Cxm]);
                    mem_trace_read("Bza", &Bza[iz][iy][Cxm]);
                    Hz[iz][iy][Cxm] = (cxmh[Cxm] / cxph[Cxm]) * Hz[iz][iy][Cxm] + (mui * czp[iz] / cxph[Cxm]) * tmp[iz][iy] - (mui * czm[iz] / cxph[Cxm]) * Bza[iz][iy][Cxm];
                    mem_trace_write("Bza", &Bza[iz][iy][Cxm]);
                    mem_trace_read("tmp", &tmp[iz][iy]);
                    Bza[iz][iy][Cxm] = tmp[iz][iy];
                    for (ix = 0;
                        ix < Cxm;
                        ix++)
                    {
                        mem_trace_iter_start("ix", ix);
                        {
                            mem_trace_write("clf", &clf[iz][iy]);
                            mem_trace_read("Ex", &Ex[iz][Cym][ix]);
                            mem_trace_read("Ax", &Ax[iz][ix]);
                            mem_trace_read("Ey", &Ey[iz][Cym][ix + 1]);
                            mem_trace_read("Ey", &Ey[iz][Cym][ix]);
                            clf[iz][iy] = Ex[iz][Cym][ix] - Ax[iz][ix] + Ey[iz][Cym][ix + 1] - Ey[iz][Cym][ix];
                            mem_trace_write("tmp", &tmp[iz][iy]);
                            mem_trace_read("cymh", &cymh[Cym]);
                            mem_trace_read("cyph", &cyph[iy]);
                            mem_trace_read("Bza", &Bza[iz][iy][ix]);
                            mem_trace_read("cyph", &cyph[iy]);
                            mem_trace_read("clf", &clf[iz][iy]);
                            tmp[iz][iy] = (cymh[Cym] / cyph[iy]) * Bza[iz][iy][ix] - (ch / cyph[iy]) * clf[iz][iy];
                            mem_trace_write("Hz", &Hz[iz][Cym][ix]);
                            mem_trace_read("cxmh", &cxmh[ix]);
                            mem_trace_read("cxph", &cxph[ix]);
                            mem_trace_read("Hz", &Hz[iz][Cym][ix]);
                            mem_trace_read("czp", &czp[iz]);
                            mem_trace_read("cxph", &cxph[ix]);
                            mem_trace_read("tmp", &tmp[iz][iy]);
                            mem_trace_read("czm", &czm[iz]);
                            mem_trace_read("cxph", &cxph[ix]);
                            mem_trace_read("Bza", &Bza[iz][Cym][ix]);
                            Hz[iz][Cym][ix] = (cxmh[ix] / cxph[ix]) * Hz[iz][Cym][ix] + (mui * czp[iz] / cxph[ix]) * tmp[iz][iy] - (mui * czm[iz] / cxph[ix]) * Bza[iz][Cym][ix];
                            mem_trace_write("Bza", &Bza[iz][Cym][ix]);
                            mem_trace_read("tmp", &tmp[iz][iy]);
                            Bza[iz][Cym][ix] = tmp[iz][iy];
                        }
                        mem_trace_iter_end("ix", ix);
                    }
                    mem_trace_write("clf", &clf[iz][iy]);
                    mem_trace_read("Ex", &Ex[iz][Cym][Cxm]);
                    mem_trace_read("Ax", &Ax[iz][Cxm]);
                    mem_trace_read("Ry", &Ry[iz][Cym]);
                    mem_trace_read("Ey", &Ey[iz][Cym][Cxm]);
                    clf[iz][iy] = Ex[iz][Cym][Cxm] - Ax[iz][Cxm] + Ry[iz][Cym] - Ey[iz][Cym][Cxm];
                    mem_trace_write("tmp", &tmp[iz][iy]);
                    mem_trace_read("cymh", &cymh[Cym]);
                    mem_trace_read("cyph", &cyph[Cym]);
                    mem_trace_read("Bza", &Bza[iz][Cym][Cxm]);
                    mem_trace_read("cyph", &cyph[Cym]);
                    mem_trace_read("clf", &clf[iz][iy]);
                    tmp[iz][iy] = (cymh[Cym] / cyph[Cym]) * Bza[iz][Cym][Cxm] - (ch / cyph[Cym]) * clf[iz][iy];
                    mem_trace_write("Hz", &Hz[iz][Cym][Cxm]);
                    mem_trace_read("cxmh", &cxmh[Cxm]);
                    mem_trace_read("cxph", &cxph[Cxm]);
                    mem_trace_read("Hz", &Hz[iz][Cym][Cxm]);
                    mem_trace_read("czp", &czp[iz]);
                    mem_trace_read("cxph", &cxph[Cxm]);
                    mem_trace_read("tmp", &tmp[iz][iy]);
                    mem_trace_read("czm", &czm[iz]);
                    mem_trace_read("cxph", &cxph[Cxm]);
                    mem_trace_read("Bza", &Bza[iz][Cym][Cxm]);
                    Hz[iz][Cym][Cxm] = (cxmh[Cxm] / cxph[Cxm]) * Hz[iz][Cym][Cxm] + (mui * czp[iz] / cxph[Cxm]) * tmp[iz][iy] - (mui * czm[iz] / cxph[Cxm]) * Bza[iz][Cym][Cxm];
                    mem_trace_write("Bza", &Bza[iz][Cym][Cxm]);
                    mem_trace_read("tmp", &tmp[iz][iy]);
                    Bza[iz][Cym][Cxm] = tmp[iz][iy];
                }
                mem_trace_iter_end("iy", iy);
            }
        }
        mem_trace_iter_end("iz", iz);
    }
    double total = 0;
    for (int iter = 0;
        iter < Cz;
        iter++)
    {
        for (int iter2 = 0;
            iter2 < Cym;
            iter2++)
        {
            for (int iter3 = 0;
                iter3 < Cxm;
                iter3++)
            {
                total += Bza[iter][iter2][iter3];
            }
        }
    }
    printf("total=%f\n", total);
    return 0;
}

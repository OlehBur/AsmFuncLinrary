// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) float __cdecl  BurFunction(float xValue)
{
    float Y;
    float Four = 4.0f;
    float M[4] = { 1.0f, 2.0f, 3.0f, 4.0f }; // масив елементів
    int i, n = 4; // n - кількість елементів масиву

    _asm
    {
        fld xValue
        fcomp dword ptr[esp]
        fstsw ax
        sahf
        je Case1
        jb Case2
        jmp Case3

        Case1 :
        mov eax, 3
            jmp End

            Case2 :
        xor ecx, ecx // проінічення лічильника до 0.
            fldz // завантажуєм 0 до st(0).

            SumLoop :
        fadd dword ptr[M + ecx * 4] // додаємо курентний елемент масиву до st(0).
            inc ecx // інкрементимо лічил.
            cmp ecx, n // порівнюємо значення лічильника з кількістю елементів
            jl SumLoop // при умові, що лічильник менше кількості елем. повертаємось на початок цикла

            fmul dword ptr[Four] // множимо значення суми на 4.
            fstp Y // сейвимо результ в Y.
            jmp End

            Case3 :
        mov eax, 12

            End :
    }
    return Y;
}
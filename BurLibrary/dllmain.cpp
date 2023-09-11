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
//extern "C" __declspec(dllexport) float __cdecl BurFunction(float xValue)
//{
//    float Y;
//    float One = 1.0f, Two = 2.0f, Three = 3.0f, Four = 4.0f, Ten = 10.0f;
//    float M[4] = { 1.0f, 2.0f, 3.0f, 4.0f }; // масив елементів
//    int n = 4; // n - кількість елементів масиву
//
//    // Обчислюємо суму елементів масиву
//    float sum = 0;
//    for (int i = 0; i < n; i++)
//        sum += M[i];
//
//    // Виконуємо різні дії в залежності від значення xValue
//    if (xValue == One)
//        Y = One + Two;
//    else if (xValue == Two)
//        Y = Two * sum + Ten * xValue * xValue * sum;
//    else if (xValue >= Four)
//        Y = Three * Four;
//    else
//        Y = std::numeric_limits<float>::quiet_NaN(); // повертаємо NaN    //#include <limits>
//
//    return Y;
//}

extern "C" __declspec(dllexport) float __cdecl BurFunction(float xValue)
{
    //ініц змінних
    float Y { 0.0f };//результ
    float One = 1.0f, Two = 2.0f, Three = 3.0f, Four = 4.0f, Ten = 10.0f;//доп. змінні для обчислень
    float sum { 0.0f };//змінна суми
    float M[4] = { 1.0f, 2.0f, 3.0f, 4.0f }; // масив певних чисел
    int n = 4; // n - к-сть ел. масиву

    _asm
    {
        xor ecx, ecx; // занулення коунтера
        fldz; // завантаження 0 в фпю (стекс - ст(0))

    SumLoop:
        fadd dword ptr[M + ecx * 4]; // додавання курентного елемента масива в ст.
        inc ecx; // інкрементим коунтер
        cmp ecx, n; // зіставлення значення коунтера з к-стю елементів масиву
        jl SumLoop; // при умові коунтера - менше к-сті елементів, стрибаємо на початок цикла

        fstp dword ptr[sum]; // сейв суми елементів масиву

        fld xValue; // завантаж. xValue в регістр стеку

        fld st(0);
        fcomp dword ptr[One];//зіставляння Х та 1
        fstsw ax;
        sahf;
        je Case1; //при Х=1, стрибаємо на Вип.1

        fld st(0);
        fcomp dword ptr[Two];//Х та 2
        fstsw ax;
        sahf;
        je Case2;//при Х=2 - стрибаємо на Вип.2

        fld st(0);//Х та 4
        fcomp dword ptr[Four];
        fstsw ax;
        sahf;
        jae Case4;//при Х>=4 -  пригаємо на Вип.4

        //в усіх інших випадках де не спрацювали провірки
    DefaultCase://при Х=3 та Х<=0
        fldz;//занул регістра стеку
        fdiv st(0), st(0); //ділимо 0 на 0 (отримаємо НаН)
        fstp Y;//сейвимо отримане значення в змінну

        jmp End;//ліваєм на кінець функи

    Case1://Х=1
        fld dword ptr[One]; //закидуємо значення 1 в стек
        fadd dword ptr[Two];//додаємо до нього 2
        fstp Y;//отримуєм на виході 3 котре зберігаємо в У

        jmp End;//вистрибуємо в кінець функи

    Case2://Х=2
        fld xValue;//закид в стек Х
        fmul st(0), st(0);//робимо квадрат Х
        fmul dword ptr[Ten];//квадрат *10
        fmul dword ptr[sum];//* сума ел. масиву

        fld dword ptr[sum];//закид суму в фпю стеку.-(0)
        fmul dword ptr[Two];//*2

        faddp st(1), st(0);//додаємо ці два значення в стеку

        fstp Y;//сейвим результ в У

        jmp End;//виприг

    Case4://х>=4
        fld dword ptr[Three];//3 в стек
        fmul dword ptr[Four];//*4
        fstp Y;//отримані 12 кидаємо в У

    End://інші х
    }

    return Y; // повертаємо результат функи
}




//extern "C" __declspec(dllexport) float __cdecl BurFunction(float xValue)
//{
//    float Y;
//    float One, Two, Three, Four, Ten, sum;
//    float M[4] = { 1.0f, 2.0f, 3.0f, 4.0f }; // масив елементів
//    int i, n = 4; // n - кількість елементів масиву
//
//    _asm
//    {
//        FINIT; // ініціалізація регістра FPU
//        FLD1; // завантажуємо 1 в ST(0)
//        FSTP dword ptr[One]; // зберігаємо значення ST(0) в One
//        FLD1; // завантажуємо 1 в ST(0)
//        FADD dword ptr[One]; // додаємо One до ST(0)
//        FSTP dword ptr[Two]; // зберігаємо значення ST(0) в Two
//        FLD1; // завантажуємо 1 в ST(0)
//        FADD dword ptr[Two]; // додаємо Two до ST(0)
//        FSTP dword ptr[Three]; // зберігаємо значення ST(0) в Three
//        FLD1; // завантажуємо 1 в ST(0)
//        FADD dword ptr[Three]; // додаємо Three до ST(0)
//        FSTP dword ptr[Four]; // зберігаємо значення ST(0) в Four
//        FLD1; // завантажуємо 1 в ST(0)
//        FADD dword ptr[Four]; // додаємо Four до ST(0)
//        FADD dword ptr[Four]; // додаємо Four до ST(0)
//        FSTP dword ptr[Ten]; // зберігаємо значення ST(0) в Ten
//
//        xor ecx, ecx; // обнуляємо лічильник
//        fldz; // завантажуємо 0 в ST(0)
//
//    SumLoop:
//        fadd dword ptr[M + ecx * 4]; // додаємо поточний елемент масиву до ST(0)
//        inc ecx; // інкрементуємо лічильник
//        cmp ecx, n; // порівнюємо значення лічильника з кількістю елементів
//        jl SumLoop; // якщо лічильник менше кількості елементів, повертаємось на початок цикла
//
//        fstp dword ptr[sum]; // зберігаємо суму елементів масиву
//
//        fld xValue; // завантажуємо xValue в регістр st(0)
//
//        fld st(0);
//        fcomp dword ptr[One];
//        fstsw ax;
//        sahf;
//        je Case1;
//
//        fld st(0);
//        fcomp dword ptr[Two];
//        fstsw ax;
//        sahf;
//        je Case2;
//
//        fld st(0);
//        fcomp dword ptr[Three];
//        fstsw ax;
//        sahf;
//        je Case3;
//
//    Case1:
//        fld dword ptr[One];
//        fadd dword ptr[Two];
//        fstp Y;
//        jmp End;
//
//    Case2:
//        fld xValue;
//        fmul st(0), st(0);
//        fmul dword ptr[Two];
//
//        fld dword ptr[sum];
//
//        faddp st(1), st(0);
//
//        fmul dword ptr[Ten];
//
//        fstp Y;
//        jmp End;
//
//    Case3:
//        fldz;
//        fdiv st(0), st(0);
//        fstp Y;
//
//        jmp End;
//
//    Case4:
//        fld dword ptr[Four];
//        fmul dword ptr[Three];
//        fstp Y;
//
//        End:
//    }
//    return Y;
//}


//extern "C" __declspec(dllexport) float __cdecl BurFunction(float xValue)
//{
//    float Y;
//    float One = 1.0f;
//    float Two = 2.0f;
//    float Three = 3.0f;
//    float Four = 4.0f;
//    float Ten = 10.0f;
//    float M[4] = { 1.0f, 2.0f, 3.0f, 4.0f }; // масив елементів
//    int i, n = 4; // n - кількість елементів масиву
//    float sum = M[0] + M[1] + M[2] + M[3]; // сума елементів масиву
//
//    _asm
//    {
//        fld xValue //закид значення Х в регістр ст(0) - флоат поінт юніт
//        fcomp dword ptr[One] //зіставляєм із значенням одиниці
//        fstsw ax//зберігаємо те шо в флоат-п-ю у ах
//        sahf//завант стан у флажки регістру 
//        je Case1//перевірка чи знач = 1
//        fcomp dword ptr[Two] //якщо !=, то порівнюємо далі
//        fstsw ax//аналогічно все вищому
//        sahf
//        je Case2
//        fcomp dword ptr[Three]//зіставлюємо вже з трьома
//        fstsw ax
//        sahf
//        je Case3 // змінили на je, щоб перевіряти на рівність
//        fcomp dword ptr[Four]//зіставлюємо вже з 4кою
//        fstsw ax
//        sahf
//        jae Case4 // змінили на jae, щоб перевіряти на більше або рівно
//
//        Case1 :
//        fld dword ptr[One]//якщо х=1 то додаємо згнач два та виводим його
//            fadd dword ptr[Two]
//            fstp Y
//            jmp End
//
//            Case2 :
//        fld xValue // завантажуєм значення xValue до st(0)
//            fmul st(0), st(0) // множимо xValue на самого себе (xValue^2)
//            fmul dword ptr[Two] // множимо результат на 2
//
//            fld dword ptr[sum] // завантажуєм суму елементів масиву до st(0)
//
//            faddp st(1), st(0) // додаємо результат до суми елементів масиву
//
//            fmul dword ptr[Ten] // множимо результат на 10
//
//            fstp Y // сейвимо результ в Y без додаткових обчислень.
//            jmp End
//
//            Case3 ://при інших випадках
//        fldz  // завантаєумо значення 0 в регістр фпю.
//            fdiv st(0), st(0)  // ділимо це значення 0 на 0 для отримання NaN
//            fstp Y // зберігаємо знач У, та читимо регістр фпю (ст(0)).
//
//            jmp End
//
//            Case4 :
//        fld dword ptr[Four]
//            fmul dword ptr[Three] // множимо 4 на 3 (12)
//            fstp Y
//
//            End :
//    }
//    return Y;
//}


//extern "C" __declspec(dllexport) float __cdecl BurFunction(float xValue)
//{
//    float Y;
//    float One = 1.0f;
//    float Two = 2.0f;
//    float Three = 3.0f;
//    float Four = 4.0f;
//    float Ten = 10.0f;
//    float M[4] = { 1.0f, 2.0f, 3.0f, 4.0f }; // масив елементів
//    int i, n = 4; // n - кількість елементів масиву
//
//    _asm
//    {
//        fld xValue //закид значення Х в регістр ст(0) - флоат поінт юніт
//        fcomp dword ptr[One] //зіставляєм із значенням одиниці
//        fstsw ax//зберігаємо те шо в флоат-п-ю у ах
//        sahf//завант стан у флажки регістру 
//        je Case1//перевірка чи знач = 1
//        fcomp dword ptr[Two] //якщо !=, то порівнюємо далі
//        fstsw ax//аналогічно все вищому
//        sahf
//        je Case2
//        fcomp dword ptr[Three]//зіставлюємо вже з трьома
//        fstsw ax
//        sahf
//        je Case3 // змінили на je, щоб перевіряти на рівність
//        fcomp dword ptr[Four]//зіставлюємо вже з 4кою
//        fstsw ax
//        sahf
//        jae Case4 // змінили на jae, щоб перевіряти на більше або рівно
//
//        Case1 :
//        fld dword ptr[One]//якщо х=1 то додаємо згнач два та виводим його
//            fadd dword ptr[Two]
//            fstp Y
//            jmp End
//
//            Case2 :
//        xor ecx, ecx // проінічення лічильника до 0.
//            fldz // завантажуєм 0 до st(0).
//
//            SumLoop :
//        fadd dword ptr[M + ecx * 4] // додаємо курентний елемент масиву до st(0).
//            inc ecx // інкрементимо лічил.
//            cmp ecx, n // порівнюємо значення лічильника з кількістю елементів
//            jl SumLoop // при умові, що лічильник менше кількості елем. повертаємось на початок цикла
//
//            fld xValue // завантажуєм значення xValue до st(0)
//            fmul st(0), st(0) // множимо xValue на самого себе (xValue^2)
//            fmul dword ptr[Two] // множимо результат на 2
//
//            faddp st(1), st(0) // додаємо результат до суми елементів масиву
//
//            fmul dword ptr[Ten] // множимо результат на 10
//
//            fstp Y // сейвимо результ в Y без додаткових обчислень.
//            jmp End
//
//            Case3 ://при інших випадках
//        fldz  // завантаєумо значення 0 в регістр фпю.
//            fdiv st(0), st(0)  // ділимо це значення 0 на 0 для отримання NaN
//            fstp Y // зберігаємо знач У, та читимо регістр фпю (ст(0)).
//
//            jmp End
//
//            Case4 :
//        fld dword ptr[Four]
//            fmul dword ptr[Three] // множимо 4 на 3 (12)
//            fstp Y
//
//            End :
//    }
//    return Y;
//}


//extern "C" __declspec(dllexport) float __cdecl BurFunction(float xValue)
//{
//    float Y;
//    float One = 1.0f;
//    float Two = 2.0f;
//    float Three = 3.0f;
//    float Four = 4.0f;
//    float M[4] = { 1.0f, 2.0f, 3.0f, 4.0f }; // масив елементів
//    int i, n = 4; // n - кількість елементів масиву
//
//    _asm
//    {
//        fld xValue //закид значення Х в регістр ст(0) - флоат поінт юніт
//        fcomp dword ptr[One] //зіставляєм із значенням одиниці
//        fstsw ax//зберігаємо те шо в флоат-п-ю у ах
//        sahf//завант стан у флажки регістру 
//        je Case1//перевірка чи знач = 1
//        fcomp dword ptr[Two] //якщо !=, то порівнюємо далі
//        fstsw ax//аналогічно все вищому
//        sahf
//        je Case2
//        fcomp dword ptr[Three]//зіставлюємо вже з трьома
//        fstsw ax
//        sahf
//        je Case3 // змінили на je, щоб перевіряти на рівність
//        fcomp dword ptr[Four]//зіставлюємо вже з 4кою
//        fstsw ax
//        sahf
//        je Case4 // змінили на je, щоб перевіряти на рівність
//        jmp Case5
//
//        Case1 :
//        fld dword ptr[One]//якщо х=1 то додаємо згнач два та виводим його
//            fadd dword ptr[Two]
//            fstp Y
//            jmp End
//
//            Case2 :
//        xor ecx, ecx // проîнîчення лîчильника до 0.
//            fldz // завантажуєм 0 до st(0).
//
//            SumLoop :
//        fadd dword ptr[M + ecx * 4] // додаємо курентний елемент масиву до st(0).
//            inc ecx // îнкрементимо лîчил.
//            cmp ecx, n // порîвнюємо значення лîчильника з кîлькîстю елементîв
//            jl SumLoop // при умовî, що лîчильник менше кîлькостî елем. повертаємось на початок цикла
//
//            fstp Y // сейвимо результ в Y без додаткових обчислень.
//            jmp End
//
//            Case3 :
//        xor ecx, ecx // проîнîчення лîчильника до 0.
//            fldz // завантажуєм 0 до st(0).
//
//            SumLoopCase3 :
//        fadd dword ptr[M + ecx * 4] // додаємо курентний елемент масиву до st(0).
//            inc ecx // îнкрементимо лîчил.
//            cmp ecx, n // порîвнюємо значення лîчильника з кîлькîстю елементîв
//            jl SumLoopCase3 // при умовî, що лîчильник менше кîлькостî елем. повертаємось на початок цикла
//
//            fstp Y // сейвимо результ в Y без додаткових обчислень.
//            jmp End
//
//            Case4 :
//        fld dword ptr[Four]
//            fmul dword ptr[Three] // множимо 4 на 3 (12)
//            fstp Y
//
//            Case5 ://при інших випадках
//        fldz  // завантаєумо значення 0 в регістр фпю.
//            fdiv st(0), st(0)  // ділимо це значення 0 на 0 для отримання NaN
//            fstp Y // зберігаємо знач У, та читимо регістр фпю (ст(0)).
//
//            End :
//    }
//    return Y;
//}


//extern "C" __declspec(dllexport) float __cdecl BurFunction(float xValue)
//{
//    float Y;
//    float One = 1.0f;
//    float Two = 2.0f;
//    float Three = 3.0f;
//    float Four = 4.0f;
//    float M[4] = { 1.0f, 2.0f, 3.0f, 4.0f }; // масив елементів
//    int i, n = 4; // n - кількість елементів масиву
//
//    _asm
//    {
//        fld xValue //закид значення Х в регістр ст(0) - флоат поінт юніт
//        fcomp dword ptr[One] //зіставляєм із значенням одиниці
//        fstsw ax//зберігаємо те шо в флоат-п-ю у ах
//        sahf//завант стан у флажки регістру 
//        je Case1//перевірка чи знач = 1
//        fcomp dword ptr[Two] //якщо !=, то порівнюємо далі
//        fstsw ax//аналогічно все вищому
//        sahf
//        je Case2
//        fcomp dword ptr[Four]//зіставлюємо вже з 4кою
//        fstsw ax
//        sahf
//        jae Case3 // змінили на jae, щоб перевіряти на більше або рівно
//        jmp Case4
//
//        Case1 :
//        fld dword ptr[One]//якщо х=1 то додаємо згнач два та виводим його
//            fadd dword ptr[Two]
//            fstp Y
//            jmp End
//
//            Case2 :
//        xor ecx, ecx // проінічення лічильника до 0.
//            fldz // завантажуєм 0 до st(0).
//
//            SumLoop :
//        fadd dword ptr[M + ecx * 4] // додаємо курентний елемент масиву до st(0).
//            inc ecx // інкрементимо лічил.
//            cmp ecx, n // порівнюємо значення лічильника з кількістю елементів
//            jl SumLoop // при умові, що лîчильник менше кîлькостî елем. повертаємось на початок цикла
//
//            fstp Y // сейвимо результ в Y без додаткових обчислень.
//            jmp End
//
//            Case3 :
//        fld dword ptr[Four]
//            fmul dword ptr[Three] // множимо 4 на 3 (12)
//            fstp Y
//
//            Case4 ://при інших випадках
//        fldz  // завантаєумо значення 0 в регістр фпю.
//            fdiv st(0), st(0)  // ділимо це значення 0 на 0 для отримання NaN
//            fstp Y // зберігаємо знач У, та читимо регістр фпю (ст(0)).
//
//            End :
//    }
//    return Y;
//}

//extern "C" __declspec(dllexport) float __cdecl BurFunction(float xValue)
//{
//    float Y;
//    float One = 1.0f;
//    float Two = 2.0f;
//    float Four = 4.0f;
//    float M[4] = { 1.0f, 1.5f, 2.0f, 2.5f }; // масив елементів
//    int i, n = 4; // n - кількість елементів масиву
//
//    _asm
//    {
//        fld xValue //закид значення Х в регістр ст(0) - флоат поінт юніт
//        fcomp dword ptr[One] //зіставляєм із значенням одиниці
//        fstsw ax//зберігаємо те шо в флоат-п-ю у ах
//        sahf//завант стан у флажки регістру 
//        je Case1//перевірка чи знач = 1
//        fcomp dword ptr[Two] //якщо !=, то порівнюємо далі
//        fstsw ax//аналогічно все вищому
//        sahf
//        je Case2
//        fcomp dword ptr[Four]//зіставлюємо вже з 4кою
//        fstsw ax
//        sahf
//        jbe Case3
//        jmp Case4
//
//        Case1 :
//        fld dword ptr[One]//якщо х=1 то додаємо згнач два та виводим його
//            fadd dword ptr[Two]
//            fstp Y
//            jmp End
//
//            Case2 :
//        xor ecx, ecx // проінічення лічильника до 0.
//            fldz // завантажуєм 0 до st(0).
//
//            SumLoop :
//        fadd dword ptr[M + ecx * 4] // додаємо курентний елемент масиву до st(0).
//            inc ecx // інкрементимо лічил.
//            cmp ecx, n // порівнюємо значення лічильника з кількістю елементів
//            jl SumLoop // при умові, що лîчильник менше кîлькостî елем. повертаємось на початок цикла
//
//            fld st(0)
//            fmul dword ptr[Two] // множимо значення суми на 2.
//            fld st(1)
//            fmul dword ptr[Four] // множимо значення суми на 4.
//            fmul xValue // множимо значення суми на квадрат значення аргументу.
//            faddp st(1), st(0) // додаємо два значення в стеку.
//            fstp Y // сейвимо результ в Y.
//            jmp End
//
//            Case3 :
//        fld dword ptr[Four]
//            fmul dword ptr[Four]
//            fstp Y
//
//            Case4 :
//        fld1
//            fchs
//            fstp Y
//
//            End :
//    }
//    return Y;
//}


//extern "C" __declspec(dllexport) float __cdecl BurFunction(float xValue)
//{
//    float Y;
//    float Four = 4.0f;
//    float M[4] = { 1.0f, 2.0f, 3.0f, 4.0f }; // масив елементів
//    int i, n = 4; // n - кількість елементів масиву
//
//    _asm
//    {
//        fld xValue
//        fcomp dword ptr[Four]
//        fstsw ax
//        sahf
//        je Case1
//        jb Case2
//        jmp Case3
//
//        Case1 :
//        fld dword ptr[Four]
//            fdiv dword ptr[Four]
//            fstp Y
//            jmp End
//
//            Case2 :
//        xor ecx, ecx // проінічення лічильника до 0.
//            fldz // завантажуєм 0 до st(0).
//
//            SumLoop :
//        fadd dword ptr[M + ecx * 4] // додаємо курентний елемент масиву до st(0).
//            inc ecx // інкрементимо лічил.
//            cmp ecx, n // порівнюємо значення лічильника з кількістю елементів
//            jl SumLoop // при умові, що лîчильник менше кîлькостî елем. повертаємось на початок цикла
//
//            fmul dword ptr[Four] // множимо значення суми на 4.
//            fstp Y // сейвимо результ в Y.
//            jmp End
//
//            Case3 :
//        fld dword ptr[Four]
//            fmul dword ptr[Four]
//            fmul dword ptr[Four]
//            fstp Y
//
//            End :
//    }
//    return Y;
//}


//extern "C" __declspec(dllexport) float __cdecl BurFunction(float xValue)
//{
//    float Y;
//    float Four = 4.0f;
//    float M[4] = { 1.0f, 2.0f, 3.0f, 4.0f }; // масив елементів
//    int i, n = 4; // n - кількість елементів масиву
//
//    _asm
//    {
//        fld xValue
//        fcomp dword ptr[Four]
//        fstsw ax
//        sahf
//        je Case1
//        jb Case2
//        jmp Case3
//
//        Case1 :
//        fld dword ptr[Four]
//            fdiv dword ptr[Four]
//            fstp Y
//            jmp End
//
//            Case2 :
//        xor ecx, ecx // проінічення лічильника до 0.
//            fldz // завантажуєм 0 до st(0).
//
//            SumLoop :
//        fadd dword ptr[M + ecx * 4] // додаємо курентний елемент масиву до st(0).
//            inc ecx // інкрементимо лічил.
//            cmp ecx, n // порівнюємо значення лічильника з кількістю елементів
//            jl SumLoop // при умові, що лîчильник менше кîлькостî елем. повертаємось на початок цикла
//
//            fmul dword ptr[Four] // множимо значення суми на 4.
//            fstp Y // сейвимо результ в Y.
//            jmp End
//
//            Case3 :
//        fld dword ptr[Four]
//            fmul dword ptr[Four]
//            fmul dword ptr[Four]
//            fstp Y
//
//            End :
//    }
//    return Y;
//}
//
//extern "C" __declspec(dllexport) float __cdecl BurFunction(float xValue)
//{
//    float Y;
//    float Four = 4.0f;
//    float M[4] = { 1.0f, 2.0f, 3.0f, 4.0f }; // масив елементів
//    int i, n = 4; // n - кількість елементів масиву
//
//    _asm
//    {
//        fld xValue
//        fcomp dword ptr[Four]
//        fstsw ax
//        sahf
//        je Case1
//        jb Case2
//        jmp Case3
//
//        Case1 :
//        fld dword ptr[Four]
//            fdiv dword ptr[Four]
//            fstp Y
//            jmp End
//
//            Case2 :
//        xor ecx, ecx // проінічення лічильника до 0.
//            fldz // завантажуєм 0 до st(0).
//
//            SumLoop :
//        fadd dword ptr[M + ecx * 4] // додаємо курентний елемент масиву до st(0).
//            inc ecx // інкрементимо лічил.
//            cmp ecx, n // порівнюємо значення лічильника з кількістю елементів
//            jl SumLoop // при умові, що лîчильник менше кîлькостî елем. повертаємось на початок цикла
//
//            fmul dword ptr[Four] // множимо значення суми на 4.
//            fstp Y // сейвимо результ в Y.
//            jmp End
//
//            Case3 :
//        fld dword ptr[Four]
//            fmul dword ptr[Four]
//            fmul dword ptr[Four]
//            fstp Y
//
//            End :
//    }
//    return Y;
//}
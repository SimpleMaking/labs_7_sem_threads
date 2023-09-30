#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <fstream>
// Длина отдельного вектора
constexpr size_t N = 10000000;
// Левая граница генерации значения вектора
constexpr size_t LEFT_BORDER = 0;
// Правая граница генерации значения вектора
constexpr size_t RIGHT_BORDER = 100;
// Количество потоков каждого случая
constexpr size_t THREADS_COUNT[4] = { 1, 2, 4, 10 };


/*
    Заполнение векторов случайными числами
    @param 1 левый вектор 
    @param 2 правый вектор
*/
void filling(std::vector<int>& A, std::vector<int>& B)
{
    for (size_t i = 0; i < N; ++i)
    {
        A.push_back(rand() % (RIGHT_BORDER - LEFT_BORDER + 1) + LEFT_BORDER);
        B.push_back(rand() % (RIGHT_BORDER - LEFT_BORDER + 1) + LEFT_BORDER);
    }
}


/*
    Скалярное произведение векторов
    @param 1 левый вектор 
    @param 2 правый вектор
    @param 3 левая граница для умножения
    @param 4 правая граница для умножения
    @param 5 результат вычисления текущей части 
*/
void scalar_product(std::vector<int>& A, std::vector<int>& B, size_t a, size_t b, float& result)
{
    for (size_t i = a + 1; i < b; i++)
    {
        result += A[i] * B[i];
    }
}


int main()
{
    srand(time(0));
    std::vector<int> A;
    std::vector<int> B;
    filling(A, B);
    std::ofstream out;         
    out.open("data.txt");   

    // Основной цикл выполнения (рассматирваем 4 случая)
    for (size_t k = 0; k < 4; k++)
    {
        const size_t threads_count = THREADS_COUNT[k];
        float* results = new float[threads_count];
        for (size_t i = 0; i < threads_count; i++)
            results[i] = 0;

        // Задаем количество потоков
        std::thread* threads = new std::thread[threads_count];
        
        // Задаем количество элементов для каждой части, одна часть - один поток выполнения
        size_t amount_of_work = A.size() / threads_count;

        // Создаем потоки
        for (size_t i = 0; i < threads_count; i++)
        {
            threads[i] = std::thread(scalar_product, std::ref(A), std::ref(B), amount_of_work * i, (i == threads_count + 1 ? A.size() : amount_of_work * (i + 1)), std::ref(results[i]));
        }

        // Запускаем все используемые потоки и замеряем время их выпополнения
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < threads_count; ++i)
        {
            threads[i].join();
        }
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> milli_diff = end - start;
        
        // Объединяем все части результата вычисления от каждого потока
        float result = 0;
        for (size_t i = 0; i < threads_count; ++i)
        {
            result += results[i];
        }

        // Записываем в файл количество потоков и время выполения операции 
        out << "threads_count: " << threads_count << std::endl;
        out << "time: " << milli_diff.count() << std::endl;
    }
    out.close();
    
    // Вызываем файл python для отрисовки графика
    system("python graphs.py");
    return 0;
}

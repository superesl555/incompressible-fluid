Несжимаемая жидкость

Для запуска проекта пропишите следующие команды:

1) mkdir build и cd build

2) cmake -DTYPES=<Параметры компиляции> .. Пример: cmake -DTYPES=FIXED\(16,12\),FAST_FIXED\(32,16\) .. (не ставьте лишних пробелов, -DTYPES написан в соответствии с ТЗ)

3) cmake --build .

4) ./fluid --p-type=..., --v-type=..., --v-flow-type=... <файл с размером поля> Пример: ./fluid --p-type=FIXED\(16,12\), --v-type=FAST_FIXED\(32,16\), --v-flow-type=FIXED\(16,12\) test_file.txt 

Важное примечание!: Программа может выдавать ошибку, если при компиляции было задано множество разных параметров. Данную проблему можно исправить увеличив стек командой ulimit -s <количество байт>
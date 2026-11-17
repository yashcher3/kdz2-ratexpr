
Библиотека ratexpr. В ней есть класс Fraction для рациональных чисел с арифметическими операторами и иерархия выражений над Fraction (Const, Negative, Add, Sub, Mul, Div). Для доп задания добавлены функции simplify и count_operations, которые упрощают дерево свёрткой констант и считают операции.

Компилятор с поддержкой C++17 и CMake 3.10 или новее.

Сборка


cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug && cmake --build build-debug -j

cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release && cmake --build build-release -j


Запуск


./build-debug/apps/demo_fraction

./build-debug/apps/demo_expression

./build-release/apps/demo_simplify


Для Release пути такие же, только вместо build-debug нужно писать build-release.

Программа demo_simplify сначала печатает два небольших примера симплификации, потом замеряет время на больших выражениях. По умолчанию берутся глубины 10, 14, 18 и 21, то есть до 4,2 млн операций, и для уровня 21 нужно около 270 МБ памяти. Замеры имеет смысл запускать только в Release. Глубины можно задать аргументами.


./build-release/apps/demo_simplify 12 16



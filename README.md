## Сборка
компилятор `gcc (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0`

сборка на CMake

```
mkdir build
cd build
cmake ..
cmake --build .
```
затем в той же папке
 
```
make
```
экзешники кладутся в папку `/bin`

## Запуск тестов

в папке `/build`

```
cd bin
mkdir test_data
./test
```

в папку `test_data` кладутся I/O файлы, создаваемые утилитой обработки

## Запустить утилиту обработки данных отдельно

в папке `/bin`

```
./process /path/to/input1 /path/to/input2 /path/to/output
```





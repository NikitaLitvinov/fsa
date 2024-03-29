# FSA 
[![Build Status](https://travis-ci.org/NikitaLitvinov/fsa.svg?branch=master)](https://travis-ci.org/NikitaLitvinov/fsa)

**FSA (File Systems Api)** - Маленькая блиотека для работы с файловой системой (Linux) и для безопастного исполнения shell в Си. 


### Поддерживаемые операции:
* Создание файла - _**fsa_file_touch**_
* Проверка наличия файла - _**fsa_file_exist**_
* Копирование файла  - _**fsa_file_copy**_
* Перемещение/переименование файла (в рамках одной файловой системы)  - _**fsa_file_rename**_
* Удаление файла  - _**fsa_file_remove**_
* Создание директории (рекурсивное) - _**fsa_dir_create**_
* Удаление директории (с содержимым) - _**fsa_dir_remove**_
* Проверка наличия директории - _**fsa_dir_exist**_
* Запуск команды в shell - _**fsa_run_command**_

### Сборка
Для сборке библиотеки и тестов, следует выполнить команду:

    mkdir cmake-build && cd cmake-build && cmake ..

Для запуска тестов

    make test

Для сборки Doxygen

    make doc

#### TODO:
*  ~~Тесты~~
* fsa_file_rename для разных систем
* ~~Doxygen~~
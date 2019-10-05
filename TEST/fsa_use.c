//
// Created by Nikita Litvinov on 07.08.18.
//

#include <string.h>
#include <errno.h>

#include <fff.h>            /* Макросы для создания заглушек/mocks */
#include <tap.h>            /* Тестирующие макросы (протокол TAP) */

#include <fsa/fsa.h>

/* Инициализация тестовой среды */
DEFINE_FFF_GLOBALS

void fsa_test_use(
    void);

void fsa_test_use(
    void)
{
    bool exist = false;
    ok((0 == fsa_dir_create("test1/test2/test3")), "fsa_dir_create: создаем несколько директорий");
    ok((0 == fsa_dir_exist("test1/test2/test3", &exist)), "fsa_dir_exist: проверяем наиличие директории");
    ok((true == exist), "Директория должна быть");

    ok((0 == fsa_file_touch("test1/test2/test3/test_file")), "fsa_file_touch: создаем файл");

    ok((0 == fsa_file_exist("test1/test2/test3/test_file", &exist)), "fsa_file_exist: проверяем наличие файла");
    ok((true == exist), "Файл должен быть");

    ok((0 == fsa_file_rename("test1/test2/test3/test_file", "test1/test2/test_file2")),
        "fsa_file_rename: преносим файл");

    ok((0 == fsa_file_exist("test1/test2/test3/test_file", &exist)), "fsa_file_exists: проверяем наличие файла");
    ok((false == exist), "Файла не должно быть");

    ok((0 == fsa_run_command("touch %s", "test1/test2/test3/test_file3")),
        "fsa_run_command: создаем еще файл с помощью другой команды");

    ok((0 == fsa_file_exist("test1/test2/test3/test_file3", &exist)), "fsa_file_exist: проверяем наличие файла");
    ok((false == exist), "Файл должен быть");

    ok((0 == fsa_file_copy("test1/test2/test3/test_file3", "test1/test_file3")), "fsa_file_copy: копируем файл");

    ok((0 == fsa_dir_remove("test1")), "fsa_dir_remove: удаляем всё");
}

int main(
    int   argc,
    char *argv[])
{
    plan(14);               /* Количество запланированных тестов */
    fsa_test_use();
    done_testing();             /* Проверка выполнения плана */
}

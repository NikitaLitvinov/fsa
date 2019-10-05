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

void fsa_test_null_ptr(
    void);

void fsa_test_null_ptr(
    void)
{
    bool exist = true;
    /* Проверка на NULL */
    ok((-1 == fsa_file_copy(NULL, "test")), "fsa_file_copy: Предаем 1ый параметр NULL, получаем -1");
    ok((-1 == fsa_file_copy("test", NULL)), "fsa_file_copy: Предаем 2ой параметр NULL, получаем -1");
    ok((-1 == fsa_file_copy(NULL, NULL)), "fsa_file_copy: Предаем оба параметра NULL, получаем -1");

    ok((-1 == fsa_file_rename("test", NULL)), "fsa_file_rename: Предаем 1ый параметр NULL, получаем -1");
    ok((-1 == fsa_file_rename(NULL, "test")), "fsa_file_rename: Предаем 2ой параметр NULL, получаем -1");
    ok((-1 == fsa_file_rename(NULL, NULL)), "fsa_file_rename: Предаем оба параметра NULL, получаем -1");

    ok((-1 == fsa_file_exist(NULL, &exist)), "fsa_file_exist: Предаем 1ый параметр NULL, получаем -1");
    ok((-1 == fsa_file_exist("test", NULL)), "fsa_file_exist: Предаем 2ой NULL, получаем -1");
    ok((-1 == fsa_file_exist(NULL, NULL)), "fsa_file_exist: Предаем оба NULL, получаем -1");

    ok((-1 == fsa_file_touch(NULL)), "fsa_file_touch: Предаем NULL, получаем -1");

    ok((-1 == fsa_file_remove(NULL)), "fsa_file_remove: Предаем NULL, получаем -1");

    ok((-1 == fsa_dir_create(NULL)), "fsa_dir_create: Предаем NULL, получаем -1");

    ok((-1 == fsa_dir_remove(NULL)), "fsa_dir_remove: Предаем NULL, получаем -1");

    ok((-1 == fsa_dir_exist(NULL, &exist)), "fsa_dir_exist: Предаем 1ый параметр NULL, получаем -1");
    ok((-1 == fsa_dir_exist("test", NULL)), "fsa_dir_exist: Предаем 2ой NULL, получаем -1");
    ok((-1 == fsa_dir_exist(NULL, NULL)), "fsa_dir_exist: Предаем оба NULL, получаем -1");

    ok((-1 == fsa_run_command(NULL)), "fsa_run_command: Предаем NULL, получаем -1");
}


int main(
    int   argc,
    char *argv[])
{
    plan(17);               /* Количество запланированных тестов */
    fsa_test_null_ptr();
    done_testing();             /* Проверка выполнения плана */
}

#ifndef FSA_FSA_H
#define FSA_FSA_H

enum {
    CMD_MAX_SIZE = 2048
};

int fsa_file_copy(
        const char *input_file,
        const char *output_file);

int fsa_file_rename(
        const char *input_file,
        const char *output_file);

int fsa_file_exist(
        const char *file_name,
        int        *exist);

int fsa_file_touch(
        const char *file_name);

int fsa_file_remove(
        const char *file_name);

int fsa_dir_create(
        const char *dir_name);

int fsa_dir_remove(
        const char *dir_name);

int fsa_dir_exist(
        const char *dir_name,
        int        *exist);

int fsa_run_command(
        const char *cmd,
        ...);

#endif //FSA_FSA_H

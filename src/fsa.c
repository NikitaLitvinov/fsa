#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdarg.h>
#include <libgen.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>

#include <fsa/fsa.h>

int fsa_file_copy(
        const char *input_file,
        const char *output_file)
{
    int input_fd = -1, output_fd = -1;
    off_t off = 0;
    size_t len = 0;
    ssize_t rv;
    char *dir = NULL;
    struct stat st;
    char tmp_dir[PATH_MAX] = { 0 };
    int ret = 0;
    errno = 0;

    if (NULL == input_file || NULL == output_file)
    {
        ret = -1;
        goto exit;
    }

    if ((input_fd = open(input_file, O_RDONLY)) < 0)
    {
        printf("Error! Can't open file \"%s\" (%s)\n", input_file, strerror(errno));
        ret = -2;
        goto exit;
    }

    strcpy(tmp_dir, output_file);
    dir = dirname(tmp_dir);
    fsa_dir_create(dir);

    if ((output_fd = open(output_file, (O_CREAT | O_WRONLY), 0777)) < 0)
    {
        printf("Error! Can't open file \"%s\" (%s)\n", output_file, strerror(errno));

        ret = -3;
        goto exit;
    }

    if (stat(input_file, &st) != 0)
    {
        len = (size_t)st.st_size;
    }

    if ((rv = sendfile(output_fd, input_fd, &off, len)) < 0)
    {
        printf("Error! %s\n", strerror(errno));
        ret = -4;
        goto exit;
    }

    exit:
    close(input_fd);
    close(output_fd);

    return ret;
}

int fsa_file_rename(
        const char *input_file,
        const char *output_file)
{
    int ret = 0;
    errno = 0;

    if (NULL == input_file || NULL == output_file)
    {
        ret = -1;
        goto exit;
    }

    ret = rename(input_file, output_file);
    if (ret < 0)
    {
        printf("Error! %s\n", strerror(errno));
        goto exit;
    }

    exit:
    return ret;
}

int fsa_file_exist(
        const char *file_name,
        int        *exist)
{
    int ret = 0;
    errno = 0;

    if (NULL == file_name || NULL == exist)
    {
        ret = -1;
        goto exit;
    }

    *exist = 0;
    ret = access(file_name, F_OK);
    if (ret == 0)
    {
        *exist = 1;
        goto exit;
    }

    if (errno != ENOENT)
    {
        printf("Error! Can't check file (%s)\n", strerror(errno));
        ret = -1;
        goto exit;
    }
    ret = 0;

    exit:
    return ret;
}

int fsa_file_touch(
        const char *file_name)
{
    int fd = -1;
    int ret = 0;
    errno = 0;

    if (NULL == file_name)
    {
        ret = -1;
        goto exit;
    }

    if ((fd = open(file_name, (O_CREAT | O_WRONLY), 0777)) < 0)
    {
        printf("Error! Can't open file \"%s\" (%s)\n", file_name, strerror(errno));
        ret = -2;
        goto exit;
    }

    exit:
    close(fd);
    return ret;
}

int fsa_file_remove(
        const char *file_name)
{
    int ret = 0;
    errno = 0;

    if (NULL == file_name)
    {
        ret = -1;
        goto exit;
    }

    ret = unlink(file_name);
    if (ret < 0)
    {
        printf("Error! Can't unlink file \"%s\" (%s)\n", file_name, strerror(errno));
        goto exit;
    }

    exit:
    return ret;
}


int fsa_dir_exist(
        const char *dir_name,
        int        *exist)
{
    DIR *directory = NULL;
    int ret = 0;
    errno = 0;

    if (NULL == dir_name || NULL == exist)
    {
        ret = -1;
        goto exit;
    }

    *exist = 0;
    directory = opendir(dir_name);
    if (directory)
    {
        *exist = 1;
        closedir(directory);
        goto exit;
    }

    if (ENOENT != errno)
    {
        printf("Error! Can't open dir \"%s\" (%s)", dir_name, strerror(errno));
        ret = -2;
        goto exit;
    }
    ret = 0;

    exit:
    return ret;
}

int fsa_dir_create(
        const char *dir_name)
{
    char *splitter = NULL;
    char *tmp_dir_iter = NULL;
    char tmp_dir[PATH_MAX] = { 0 };
    char mkdir_cmd[PATH_MAX] = { 0 };
    char tmp_cmd[PATH_MAX] = { 0 };
    struct stat st;
    int done = 0;
    int exist = 0;
    int ret = 0;

    if (NULL == dir_name)
    {
        ret = -1;
        goto exit;
    }

    fsa_dir_exist(dir_name, &exist);
    if (1 == exist)
    {
        goto exit;
    }

    strcpy(tmp_dir, dir_name);
    tmp_dir_iter = tmp_dir;
    tmp_dir_iter++;

    while (!done)
    {
        errno = 0;
        if (*tmp_dir_iter == 0)
        {
            break;
        }
        if ((splitter = strchr(tmp_dir_iter, '/')) != NULL)
        {
            strncpy(mkdir_cmd, dir_name, (size_t)(strlen(dir_name) - strlen(splitter)));
            tmp_dir_iter = splitter + 1;
            if (stat(mkdir_cmd, &st) == 0)
            {
                continue;
            }
            sprintf(tmp_cmd, "%s", mkdir_cmd);
        }
        else
        {
            sprintf(tmp_cmd, "%s", dir_name);
            done = 1;
        }

        if (mkdir(tmp_cmd, S_IRWXU))
        {
            printf("Error! %s\n", strerror(errno));
            ret = -2;
            goto exit;
        }
    }

    exit:
    return ret;
}

int fsa_dir_remove(
        const char *dir_name)
{
    DIR *directory = NULL;
    struct dirent *p = NULL;
    size_t path_len = 0;
    size_t buf_len = 0;
    int ret = 0;
    int internal_ret = -1;
    char buf[PATH_MAX] = { 0 };
    struct stat stat_buf;

    if (NULL == dir_name)
    {
        ret = -1;
        goto exit;
    }

    errno = 0;
    directory = opendir(dir_name);
    if (NULL == directory)
    {
        printf("Error! Can't open dir (%s)\n", strerror(errno));
        ret = -2;
        goto exit;
    }
    path_len = strlen(dir_name);

    while (!ret && (p = readdir(directory)))
    {
        if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
        {
            continue;
        }

        buf_len = path_len + strlen(p->d_name) + 2;
        if (buf_len >= PATH_MAX)
        {
            printf("Error! Long directory name\n");
            ret = -3;
            goto exit;
        }
        snprintf(buf, buf_len, "%s/%s", dir_name, p->d_name);

        if (!stat(buf, &stat_buf))
        {
            if (S_ISDIR(stat_buf.st_mode))
            {
                internal_ret = fsa_dir_remove(buf);
            }
            else
            {
                internal_ret = unlink(buf);
            }
        }

        memset(buf, 0, buf_len);
        ret = internal_ret;
    }

    if (!ret)
    {
        errno = 0;
        ret = rmdir(dir_name);
        if (ret != 0)
        {
            printf("Error! %s\n", strerror(errno));
            goto exit;
        }
    }

    exit:
    if (NULL != directory && 0 != closedir(directory))
    {
        printf("Error! Can't close dire (%s)\n", strerror(errno));
    }
    return ret;
}


static int system_vfork(
        const char *cmd)
{
    pid_t pid = -1;
    pid_t pid_wait = -1;
    int ret = 0;
    const char *sh = "/bin/sh";
    char *const argv[] = { (char *const)sh, (char *const)"-c", (char *const)cmd, (char *const)NULL };

    pid = vfork();
    if (pid < 0)
    {
        ret = -1;
        goto exit;
    }

    if (pid == 0)
    {
        execv(sh, argv);
        _exit(127);
    }

    do
    {
        pid_wait = waitpid(pid, &ret, 0);
        if (pid_wait <= -1 && errno != EINTR)
        {
            ret = -1;
            break;
        }
    } while (pid_wait != pid);

    exit:
    return ret;
}

int fsa_run_command(
        const char *cmd,
        ...)
{
    char buf[PATH_MAX] = { 0 };
    va_list args;
    int ret = 0;

    if (NULL == cmd)
    {
        ret = -1;
        goto exit;
    }
    va_start(args, cmd);
    vsprintf(buf, cmd, args);
    va_end(args);

    ret = system_vfork(buf);

    exit:
    return ret >= 0 ? WEXITSTATUS(ret) : ret;
}


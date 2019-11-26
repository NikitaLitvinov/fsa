/** FSA. @file
 *
 * Detailed information can be found in README.md
 * Examples can be found in the catalog TEST
 *
 * @version 1.0
 * @author created by Nikita Litvinov, 2019
 *
 */
#include <fsa/fsa.h>

#define LOG_ERR(fmt, ...) printf("[%s:%d] Error! "fmt, __func__, __LINE__, ##__VA_ARGS__ )
#define LOG_INFO(fmt, ...) printf(fmt, ##__VA_ARGS__ )

enum {
    CMD_MAX_SIZE = 2048
};

/** Copy file.
 *
 * @param[in]   input_file Name source file.
 * @param[in]   output_file Name file for copy, If file isn't exist, it will be created.
 *
 * @return      On error, ERROR_CODES is returned, otherwise 0.
 */

int fsa_file_copy(
        const char *input_file,
        const char *output_file)
{
    int input_fd = -1;
    int output_fd = -1;
    off_t off = 0;
    size_t len = 0;
    ssize_t rv;
    char *dir = NULL;
    struct stat st;
    char tmp_dir[PATH_MAX] = {0};
    int ret = 0;

    if (NULL == input_file || NULL == output_file)
    {
        ret = -1;
        goto exit;
    }

    errno = 0;
    if ((input_fd = open(input_file, O_RDONLY)) < 0)
    {
        LOG_ERR("Can't open file \"%s\" (%s)\n", input_file, strerror(errno));
        ret = -2;
        goto exit;
    }

    strcpy(tmp_dir, output_file);
    dir = dirname(tmp_dir);
    fsa_dir_create(dir);

    errno = 0;
    if ((output_fd = open(output_file, (O_CREAT | O_WRONLY), 0777)) < 0)
    {
        LOG_ERR("Can't open file \"%s\" (%s)\n", output_file, strerror(errno));

        ret = -3;
        goto exit;
    }
    ret = stat(input_file, &st);
    if (ret != 0)
    {
        LOG_ERR("Can't stat file \"%s\"", input_file);
        ret = -4;
        goto exit;
    }
    len = (size_t) st.st_size;

    if ((rv = sendfile(output_fd, input_fd, &off, len)) < 0)
    {
        LOG_ERR("Can't copy (sendfile) files (%s)\n", strerror(errno));
        ret = -5;
        goto exit;
    }

    exit:
    if (input_fd != -1)
    {
        close(input_fd);
    }
    if (output_fd != -1)
    {
        close(output_fd);
    }

    errno = 0;

    return ret;
}

/** Move file.
 *
 * @warning Use it only for one FS.
 *
 * @param[in]   input_file Name source file.
 * @param[in]   output_file Name file for move.
 *
 * @return      On error, ERROR_CODES is returned, otherwise 0.
 */
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
        LOG_ERR("Can't rename file (%s)\n", strerror(errno));
        goto exit;
    }

    exit:

    errno = 0;

    return ret;
}

/** Check file exist.
 *
 * @param[in]   file_name Name file.
 * @param[out]   exist If file exist this val = true, otherwise = false.
 *
 * @return      On error, ERROR_CODES is returned, otherwise 0.
 */
int fsa_file_exist(
        const char *file_name,
        bool *exist)
{
    int ret = 0;

    if (NULL == file_name || NULL == exist)
    {
        ret = -1;
        goto exit;
    }

    errno = 0;
    *exist = false;
    ret = access(file_name, F_OK);
    if (ret == 0)
    {
        *exist = true;
        goto exit;
    }

    if (errno != ENOENT)
    {
        LOG_ERR("Can't check file (%s)\n", strerror(errno));
        ret = -2;
        goto exit;
    }
    ret = 0;

    exit:

    errno = 0;

    return ret;
}

/** Create file.
 *
 * @param[in]   file_name Name file for create.
 *
 * @return      On error, ERROR_CODES is returned, otherwise 0.
 */
int fsa_file_touch(
        const char *file_name)
{
    int fd = -1;
    int ret = 0;

    if (NULL == file_name)
    {
        ret = -1;
        goto exit;
    }

    errno = 0;
    if ((fd = open(file_name, (O_CREAT | O_WRONLY), 0777)) < 0)
    {
        LOG_ERR("Can't open file \"%s\" (%s)\n", file_name, strerror(errno));
        ret = -2;
        goto exit;
    }

    exit:
    if (fd != -1)
    {
        close(fd);
    }

    errno = 0;

    return ret;
}

/** Delete file.
 *
 * @param[in]   file_name Name file for delete.
 *
 * @return      On error, ERROR_CODES is returned, otherwise 0.
 */
int fsa_file_remove(
        const char *file_name)
{
    int ret = 0;

    if (NULL == file_name)
    {
        ret = -1;
        goto exit;
    }

    errno = 0;
    ret = unlink(file_name);
    if (ret < 0)
    {
        LOG_ERR("Can't unlink file \"%s\" (%s)\n", file_name, strerror(errno));
        goto exit;
    }

    exit:

    errno = 0;

    return ret;
}

/** Check directory exist.
 *
 * @param[in]   dir_name Name directory.
 * @param[out]   exist If dir exist this val = true, otherwise = false.
 *
 * @return      On error, ERROR_CODES is returned, otherwise 0.
 */
int fsa_dir_exist(
        const char *dir_name,
        bool *exist)
{
    DIR *directory = NULL;
    int ret = 0;

    if (NULL == dir_name || NULL == exist)
    {
        ret = -1;
        goto exit;
    }

    errno = 0;
    *exist = false;
    directory = opendir(dir_name);
    if (directory)
    {
        *exist = true;
        goto exit;
    }

    if (ENOENT != errno)
    {
        LOG_ERR("Can't open dir \"%s\" (%s)", dir_name, strerror(errno));
        ret = -2;
        goto exit;
    }
    ret = 0;

    exit:
    if (*exist == true)
    {
        closedir(directory);
    }

    errno = 0;

    return ret;
}

/** Create directory, is support for recursive creation.
 *
 * @param[in]   dir_name Name directory.
 *
 * @return      On error, ERROR_CODES is returned, otherwise 0.
 */
int fsa_dir_create(
        const char *dir_name)
{
    char *splitter = NULL;
    char *tmp_dir_iter = NULL;
    char tmp_dir[PATH_MAX] = {0};
    char mkdir_cmd[PATH_MAX] = {0};
    char tmp_cmd[PATH_MAX] = {0};
    struct stat st;
    int done = 0;
    bool exist = false;
    int ret = 0;

    if (NULL == dir_name)
    {
        ret = -1;
        goto exit;
    }

    fsa_dir_exist(dir_name, &exist);
    if (false == exist)
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
            strncpy(mkdir_cmd, dir_name, (size_t) (strlen(dir_name) - strlen(splitter)));
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
            LOG_ERR("Can't create dir (%s)\n", strerror(errno));
            ret = -2;
            goto exit;
        }
    }

    exit:
    return ret;
}

/** Delete directory with content.
 *
 * @param[in]   dir_name Name directory.
 *
 * @return      On error, ERROR_CODES is returned, otherwise 0.
 */
int fsa_dir_remove(
        const char *dir_name)
{
    DIR *directory = NULL;
    struct dirent *p = NULL;
    size_t path_len = 0;
    size_t buf_len = 0;
    int ret = 0;
    int internal_ret = -1;
    char buf[PATH_MAX] = {0};
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
        LOG_ERR("Can't open dir (%s)\n", strerror(errno));
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
            LOG_ERR("Long directory name\n");
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
            LOG_ERR("Can't remove dir (%s)\n", strerror(errno));
            goto exit;
        }
    }

    exit:
    if (NULL != directory && 0 != closedir(directory))
    {
        LOG_ERR("Can't close dire (%s)\n", strerror(errno));
    }

    errno = 0;

    return ret;
}


static int system_vfork(
        const char *cmd)
{
    pid_t pid = -1;
    pid_t pid_wait = -1;
    int ret = 0;
    const char *sh = "/bin/sh";
    char *const argv[] = {sh, "-c", cmd, NULL};

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

/** A function for safely and correctly running shell commands.
 *
 * @param[in]   cmd Command, supports command input as a format string.
 *
 * @return      On error, ERROR_CODES is returned, otherwise 0.
 */
int fsa_run_command(
        const char *cmd,
        ...)
{
    char buf[PATH_MAX] = {0};
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


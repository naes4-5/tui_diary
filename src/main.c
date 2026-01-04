#include "../include/cltools.h"
#include "../include/findpaths.h"
#include "../include/readnote.h"
#include "../include/writenote.h"
#include <linux/limits.h>

int main(int argc, char *argv[]) {
    const char *homepath = getenv("HOME");
    char dierypath[PATH_MAX];
    char *projectname = get_project_name();
    char projectpath[PATH_MAX];
    char *configpath = config_path();
    DIR *projectdir = get_project_dir(homepath, projectname);
    exit_t env_check = check_env(homepath, dierypath, projectname, projectpath,
                                 configpath, projectdir);
    if (env_check != NONE) {
        return exit_error("error parsing environment variables", env_check);
    }

    operation op = get_operation(argc, argv);
    note_t notelevel;
    exit_t op_check = check_operation(argc, argv, op, &notelevel);
    if (op_check != NONE) {
        return exit_error("error parsing operation", op_check);
    }

    if (op == READ) {
        int noteexit = BADFLAG;
        switch (notelevel) {
        case PROJECT:
            noteexit = read_project(projectpath, projectname);
            break;
        case NORMAL:
            noteexit = read_note(projectpath);
            break;
        case FFILE:
        case INVALID:
        case AMEND:
        case BACK:
            closedir(projectdir);
            return exit_error("bad operation for the read operation",
                              BADOPERATION);
        }
        if (noteexit) {
            closedir(projectdir);
            return exit_error("could not read note", noteexit);
        }
    } else if (op == WRITE) {
        int noteexit = BADFLAG;
        switch (notelevel) {
        case PROJECT:
            noteexit =
                write_project_note(projectpath, projectname, argv[argc - 1]);
            break;
        case NORMAL:
            noteexit = write_normal_note(projectpath, argv[argc - 1]);
            break;
        case FFILE:
            noteexit = write_from_file(projectpath, argv[argc - 1]);
            break;
        case INVALID:
        case AMEND:
        case BACK:
            exit_error("invalid flag for a write operation", BADOPERATION);
            break;
        }
        if (noteexit) {
            closedir(projectdir);
            return exit_error("could not make note", noteexit);
        }
    }

    closedir(projectdir);
    return 0;
}

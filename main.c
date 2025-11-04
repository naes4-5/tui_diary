#include "src/includes.h"
#include "src/makenote.c"
#include <linux/limits.h>
#include <unistd.h>

void mkentry(FILE *note, const char *entry);
void initcheck(void);
int initdirs();

int main(int argc, char *argv[]) {
  bool daily = false;
  char *title = malloc(16);
  memcpy(title, "thought", sizeof("thought"));
  char opts[argc - 1];
  for (int i = 1; i < argc; i++) {
    if (*argv[i] == '-' && strlen(argv[i]) > 1) {
      switch (argv[i][1]) {
      case 'd':
        daily = true;
        break;
      case 'p':
        printf("More options for a project...ig\n");
        break;
      case 't':
        if (strlen(argv[++i]) >= 16) {
          printf("title is too long, defaulting to 'thought'\n");
          break;
        }
        memcpy(title, argv[i], strlen(argv[i]) + 1);
        break;
      default:
        printf("Flag '-%c' not supported\n", argv[i][1]);
      }
    }
  }
  initcheck();
  FILE *note = mknote(daily, title);
  if (!note) {
    fprintf(stderr, "Error in opening, aborting.\n");
    return 1;
  }
  mkentry(note, argv[argc - 1]);

  fclose(note);
  return 0;
}

void mkentry(FILE *note, const char *entry) { fprintf(note, "%s\n", entry); }

void initcheck(void) {
  char path[PATH_MAX];
  if (getcwd(path, sizeof(path)) == NULL) {
    fprintf(stderr, "Error occured: %s\n", strerror(errno));
  }

  DIR *d;
  struct dirent *entry;
  d = opendir(path);
  if (!d) {
    fprintf(stderr, "Error occured: %s\n", strerror(errno));
  }
  while ((entry = readdir(d)) != NULL) {
    char buf[64];
    sprintf(buf, "%s", entry->d_name);
    if (!strcmp(buf, "Diary")) {
      closedir(d);
      return;
    }
  }
  closedir(d);
  initdirs();
}

int initdirs() {
  if (mkdir("Diary", 0744) == -1) {
    fprintf(stderr, "Directory creation failed: %s\n", strerror(errno));
    return 1;
  }

  for (int i = 0; i < sizeof(months) / sizeof(months[0]); i++) {
    char dir[32];
    snprintf(dir, sizeof(dir), "Diary/%s", months[i]);
    if (mkdir(dir, 0744) == -1) {
      if (errno == EEXIST) {
        fprintf(stderr,
                "Directory creation failed, \"%s\" already exists: %s\n", dir,
                strerror(errno));
      } else {
        fprintf(stderr, "\"mkdir\" failed: %s\n", strerror(errno));
      }
    }
  }

  return 0;
}

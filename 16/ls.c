#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MAXPATH 1024
char path[MAXPATH] = ".";

typedef struct entry_t {
  char *name;
  struct stat st;
} Entry;

typedef struct vector_t {
  Entry *data;
  int size;
  int capacity;
} Vector;

void free_entry(Entry *entry) { free(entry->name); }
void free_vector(Vector *vector) {
  for (int i = 0; i < vector->size; i++) {
    free_entry(&vector->data[i]);
  }
  free(vector->data);
}

void push_back(Vector *vec, Entry entry) {
  if (vec->size == vec->capacity) {
    vec->capacity *= 2;
    vec->data = realloc(vec->data, vec->capacity * sizeof(Entry));
  }
  vec->data[vec->size++] = entry;
}

int entry_cmp(const void *a, const void *b) {
  Entry *a_entry = (Entry *)a;
  Entry *b_entry = (Entry *)b;
  if (S_ISDIR(a_entry->st.st_mode) != S_ISDIR(b_entry->st.st_mode)) {
    return S_ISDIR(a_entry->st.st_mode) ? -1 : 1;
  }
  return strcmp(a_entry->name, b_entry->name);
}

void print_entry(Entry entry) {
  if (S_ISDIR(entry.st.st_mode))
    printf("d");
  else if (S_ISLNK(entry.st.st_mode))
    printf("l");
  else if (S_ISBLK(entry.st.st_mode))
    printf("b");
  else if (S_ISREG(entry.st.st_mode))
    printf(".");
  else if (S_ISCHR(entry.st.st_mode))
    printf("c");
  else if (S_ISFIFO(entry.st.st_mode))
    printf("p");
  else
    printf("?");
  printf(entry.st.st_mode & S_IRUSR ? "r" : "-");
  printf(entry.st.st_mode & S_IWUSR ? "w" : "-");
  printf(entry.st.st_mode & S_IXUSR ? "x" : "-");
  printf(entry.st.st_mode & S_IRGRP ? "r" : "-");
  printf(entry.st.st_mode & S_IWGRP ? "w" : "-");
  printf(entry.st.st_mode & S_IXGRP ? "x" : "-");
  printf(entry.st.st_mode & S_IROTH ? "r" : "-");
  printf(entry.st.st_mode & S_IWOTH ? "w" : "-");
  printf(entry.st.st_mode & S_IXOTH ? "x" : "-");

  printf(" %lu", entry.st.st_nlink);

  struct passwd *pw = getpwuid(entry.st.st_uid);
  struct group *gr = getgrgid(entry.st.st_gid);
  printf("\t%s", pw->pw_name);
  printf("\t%s", gr->gr_name);

  printf("\t%ld", entry.st.st_size);

  struct tm *tm = localtime(&entry.st.st_mtime);
  char date_buf[100] = {0};
  strftime(date_buf, 100, "%b %d %H:%M", tm);
  printf("\t%s", date_buf);

  printf("\t%s", entry.name);

  if (S_ISLNK(entry.st.st_mode)) {
    char link_buf[MAXPATH] = {0};
    char fq_name[MAXPATH] = {0};
    snprintf(fq_name, MAXPATH, "%s/%s", path, entry.name);
    if (readlink(fq_name, link_buf, MAXPATH) == -1) {
      perror("can't readlink");
      return;
    }
    printf(" => %s", link_buf);
  }

  printf("\n");
}

int main(int argc, char **argv) {
  if (argc > 1) {
    strncpy(path, argv[1], MAXPATH);
  }

  Vector entries;
  entries.data = malloc(sizeof(Entry));
  entries.size = 0;
  entries.capacity = 1;

  DIR *dir;
  struct dirent *dirent;
  char qual_dir_name[MAXPATH];

  if ((dir = opendir(path)) == NULL) {
    perror("can't opendir");
    return 1;
  }

  while ((dirent = readdir(dir)) != NULL) {
    struct stat st;
    snprintf(qual_dir_name, MAXPATH, "%s/%s", path, dirent->d_name);
    if (lstat(qual_dir_name, &st) == -1) {
      perror("can't stat");
      return 1;
    }

    Entry new_entry;
    new_entry.name = strdup(dirent->d_name);
    new_entry.st = st;
    push_back(&entries, new_entry);
  }

  qsort(entries.data, entries.size, sizeof(Entry), entry_cmp);

  for (int i = 0; i < entries.size; i++) {
    print_entry(entries.data[i]);
  }

  free_vector(&entries);
  return 0;
}

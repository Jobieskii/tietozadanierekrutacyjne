#include "file_thread.h"
#include "raw_struct.h"

extern logger_function* logger;

char* get_line_suffix(size_t size, char s[size], FILE* stream) {
  s[0] = '\0';
  char* ret = fgets(s, size, stream);
  if (ret) {
    // check that line starts with 'cpu(n)'.
    if (strlen(s) > 3 && strncmp(s, "cpu", 3)) return 0;
    // and skip that part
    s += 3;
    if (s[0] != ' ') {
      char* e;
      strtoul(s, &e, 10);
      s = e;
    }
    ret = s;
    // replace newline with \0
    char* pos = strchr(s, '\n');
    if (pos) *pos = '\0';
    else ret = 0;
  }
  return ret;
}
// s must be null terminated
void fill_from_line(unsigned long long numbers[stat_count_],
                    char const* s,
                    int base) {
  char const* const t = s;
  char* next = 0;
  for (size_t i = 0; i < stat_count_; ++i) {
    numbers[i] = strtoull(s, &next, base);
    if (s == next || !s[0]) {
      logger("line from /proc/stat not fully read. (%s)\n", t);
      break;
    }
    s = next;
  }
}

void fill_from_raw(FILE* stat, size_t proc_no,
                  unsigned long long stat_raw[proc_no + 1][stat_count_]) {
  char buf[256];
  for (size_t n = 0; n < proc_no + 1; ++n) {
    char* nbuf = get_line_suffix(sizeof buf, buf, stat);
    if (nbuf) {
      fill_from_line(stat_raw[n], nbuf, 10);
    }
  }
  return;
}

int file_thread(void* arg) {
  State* state = arg;
  size_t i = 0;
  logger("Reading thread started.\n");
  while(!state->finished) {
    logger("Reading trying (%d)\n", i);
    mtx_lock(&state->raw_stats[i]->mtx);
    logger("Reading thread locked (%d).\n", i);
    freopen("/proc/stat", "r", state->stat);
    fill_from_raw(state->stat, state->proc_no, state->raw_stats[i]->raw_arr);

    mtx_unlock(&state->raw_stats[i]->mtx);
    logger("Reading thread unlocked (%d).\n", i);
    ++i;
    i %= 2;
  }
  return 0;
}


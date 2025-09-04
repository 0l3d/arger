#include "arger.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Arger_Parser arger_parser(int argc, char **argv, char *desc) {
  struct Arger_Parser ret = {
      .desc = desc,
      .argc = argc,
      .argv = argv,
  };
  return ret;
}

struct Arger_Args {
  char *longer, *shorter;
};

void check_args(struct Arger_Parser parser, struct Arger_Arg **arger,
                int size) {
  if (parser.argc < 2) {
    fprintf(stderr, "Usage: %s ... (%s -h/--help)\n", parser.argv[0],
            parser.argv[0]);
    return;
  }

  struct Arger_Args **args = malloc(sizeof(struct Arger_Args *) * size);
  int help_msg = 0;
  for (int i = 0; i < size; i++) {
    args[i] = malloc(sizeof(struct Arger_Args));

    if (arger[i]->long_n == 1) {
      args[i]->longer = malloc(strlen(arger[i]->name) + 3);
      snprintf(args[i]->longer, strlen(arger[i]->name) + 3, "--%s",
               arger[i]->name);
    } else {
      args[i]->longer = NULL;
    }

    if (arger[i]->short_n == 1) {
      args[i]->shorter = malloc(4);
      snprintf(args[i]->shorter, 4, "-%c", arger[i]->name[0]);
    } else {
      args[i]->shorter = NULL;
    }
  }

  for (int i = 0; i < parser.argc; i++) {
    if (strcmp(parser.argv[i], "--help") == 0 ||
        strcmp(parser.argv[i], "-h") == 0) {
      int max_len = 0;
      for (int i = 0; i < size; i++) {
        int len = strlen(arger[i]->name);
        if (len > max_len)
          max_len = len;
      }

      printf("%s - %s\n\n", parser.argv[0], parser.desc);

      printf("Usage: %s [OPTIONS] ...\n\n", parser.argv[0]);

      printf("\x1b[4m\x1b[1mOPTIONS:\x1b[0m\n");
      for (int i = 0; i < size; i++) {
        char opt[256] = "";
        if (arger[i]->short_n == 1) {
          strcat(opt, args[i]->shorter);
          if (arger[i]->long_n == 1)
            strcat(opt, ", ");
        }
        if (arger[i]->long_n == 1) {
          strcat(opt, args[i]->longer);
        }

        printf("\x1b[1m %-25s \x1b[0m%-*s\n", opt, max_len, arger[i]->desc);
      }
    }

    for (int arger_i = 0; arger_i < size; arger_i++) {
      if ((args[arger_i]->longer &&
           strcmp(parser.argv[i], args[arger_i]->longer) == 0) ||
          (args[arger_i]->shorter &&
           strcmp(parser.argv[i], args[arger_i]->shorter) == 0)) {
        if (arger[arger_i]->f != NULL) {
          if (arger[arger_i]->f != NULL) {
            int j = i + 1;
            int capacity = 256;
            char *value = malloc(capacity);
            value[0] = '\0';

            while (j < parser.argc && parser.argv[j][0] != '-') {
              if (strlen(value) + strlen(parser.argv[j]) + 2 > capacity) {
                capacity *= 2;
                value = realloc(value, capacity);
              }
              strcat(value, parser.argv[j]);
              strcat(value, " ");
              j++;
            }

            size_t len = strlen(value);
            if (len > 0 && value[len - 1] == ' ')
              value[len - 1] = '\0';

            arger[arger_i]->f(value);
            free(value);
          }
        }
      }
    }
  }

  for (int i = 0; i < size; i++) {
    free(args[i]->longer);
    free(args[i]->shorter);
    free(args[i]);
  }
  free(args);
}

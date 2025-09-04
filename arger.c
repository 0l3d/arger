#include "arger.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *version_info = NULL;

struct Arger_Parser arger_parser(int argc, char **argv, char *desc, char *ver) {
  version_info = ver;
  struct Arger_Parser ret = {
      .desc = desc,
      .argc = argc,
      .argv = argv,
  };
  return ret;
}

struct Arger_Args {
  char *longer, *shorter;
  int called;
};

const char *type_str(enum Arger_Type t) {
  switch (t) {
  case ARGER_STRING:
    return "<string>";
  case ARGER_INT:
    return "<int>";
  case ARGER_FLOAT:
    return "<float>";
  case ARGER_BOOL:
    return "<bool>";
  default:
    return "";
  }
}

void check_args(struct Arger_Parser parser, struct Arger_Arg **arger,
                int size) {
  if (parser.argc < 2) {
    fprintf(stderr, "Usage: %s ... (%s -h/--help)\n", parser.argv[0],
            parser.argv[0]);
    return;
  }

  struct Arger_Args **args = malloc(sizeof(struct Arger_Args *) * (size + 2));
  int args_i = 0;

  for (int i = 0; i < size; i++, args_i++) {
    args[args_i] = malloc(sizeof(struct Arger_Args));
    if (arger[i]->long_n == 1) {
      args[args_i]->longer = malloc(strlen(arger[i]->name) + 3);
      snprintf(args[args_i]->longer, strlen(arger[i]->name) + 3, "--%s",
               arger[i]->name);
    } else {
      args[args_i]->longer = NULL;
    }

    if (arger[i]->short_n == 1) {
      args[args_i]->shorter = malloc(4);
      snprintf(args[args_i]->shorter, 4, "-%c", arger[i]->name[0]);
    } else {
      args[args_i]->shorter = NULL;
    }
    args[args_i]->called = 0;
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

      printf("OPTIONS:\n");

      for (int j = 0; j < size; j++) {
        char opt[256] = "";
        if (arger[j]->short_n == 1) {
          strcat(opt, args[j]->shorter);
          if (arger[j]->long_n == 1)
            strcat(opt, ", ");
        }
        if (arger[j]->long_n == 1) {
          strcat(opt, args[j]->longer);
        }

        printf(" %-25s %-*s %s\n", opt, max_len, arger[j]->desc,
               type_str(arger[j]->type));
      }
      printf("\nEXTRA:\n");

      printf(" %-25s %-*s\n", "-h, --help", max_len, "Print this help message");
      if (version_info != NULL)
        printf(" %-25s %-*s \n", "-v, --version", max_len,
               "Show version information");

      exit(0);
    }

    if (strcmp(parser.argv[i], "--version") == 0 ||
        strcmp(parser.argv[i], "-v") == 0) {
      printf("%s - %s\n", parser.argv[0], version_info);
      exit(0);
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

            void *converted = NULL;
            int type_ok = 1;

            switch (arger[arger_i]->type) {
            case ARGER_STRING:
              converted = value;
              break;

            case ARGER_INT: {
              char *endptr;
              long val = strtol(value, &endptr, 10);
              if (*endptr != '\0') {
                type_ok = 0;
                break;
              }
              int *i_val = malloc(sizeof(int));
              *i_val = (int)val;
              converted = i_val;
              break;
            }

            case ARGER_FLOAT: {
              char *endptr;
              float fval = strtof(value, &endptr);
              if (*endptr != '\0') {
                type_ok = 0;
                break;
              }
              float *f_val = malloc(sizeof(float));
              *f_val = fval;
              converted = f_val;
              break;
            }

            case ARGER_BOOL: {
              if (strcmp(value, "true") == 0 || strcmp(value, "1") == 0 ||
                  strcmp(value, "yes") == 0 || strcmp(value, "on") == 0) {
                int *b_val = malloc(sizeof(int));
                *b_val = 1;
                converted = b_val;
              } else if (strcmp(value, "false") == 0 ||
                         strcmp(value, "0") == 0 || strcmp(value, "no") == 0 ||
                         strcmp(value, "off") == 0) {
                int *b_val = malloc(sizeof(int));
                *b_val = 0;
                converted = b_val;
              } else {
                type_ok = 0;
              }
              break;
            }
            }

            if (!type_ok) {
              fprintf(
                  stderr,
                  "Error: Invalid value \"%s\" for option %s (expected %s)\n",
                  value,
                  arger[arger_i]->long_n ? args[arger_i]->longer
                                         : args[arger_i]->shorter,
                  type_str(arger[arger_i]->type));
              continue;
            }

            args[arger_i]->called = 1;
            arger[arger_i]->f(converted);

            if (arger[arger_i]->type != ARGER_STRING) {
              free(converted);
            }

            free(value);
          }
        }
      }
    }
  }

  for (int i = 0; i < size; i++) {
    if (arger[i]->req && !args[i]->called) {
      fprintf(stderr, "Error: Required argument %s is missing\n",
              args[i]->longer ? args[i]->longer : args[i]->shorter);
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < size; i++) {
    free(args[i]->longer);
    free(args[i]->shorter);
    free(args[i]);
  }
  free(args);
}

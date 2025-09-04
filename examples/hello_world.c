#include "../arger.h"
#include <stdio.h>
#include <stdlib.h>

int count = 0;

arger_func(hello_count) { count = atoi(out); }

arger_func(hello_world) {
  for (int i = 0; i < count; i++) {
    printf("%s\n", out);
  }
}

arger_func(parser_test) {
  int num;
  char text[100];
  if (arger_f_parser("%s %d", text, &num) != 2) {
    fprintf(stderr, "Error: Failed to parse input with expected types.\n");
    fprintf(stderr, "Help: --parser text count\n");
    return;
  }
  for (int i = 0; i < num; i++) {
    printf("%s\n", text);
  }
}

int main(int argc, char *argv[]) {
  PARSER(argc, argv, "Example CLI for arger");

  ARG(count_arg){
      .name = "count",
      .desc = "Select a count.",
      .long_n = 1,
      .short_n = 1,
      .f = hello_count,
  };

  ARG(text_arg){
      .name = "text",
      .desc = "Select a text.",
      .long_n = 1,
      .short_n = 1,
      .f = hello_world,
  };

  ARG(parse_arg){
      .name = "parse",
      .desc = "Parse in one line.",
      .long_n = 1,
      .short_n = 1,
      .f = parser_test,
  };

  ARGS(&count_arg, &text_arg, &parse_arg);
  CHECK;
}

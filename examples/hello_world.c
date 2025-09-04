#include "../arger.h"
#include <stdio.h>
#include <stdlib.h>

int count = 0;

arger_func(hello_count) {
  int dereference = *(int *)out;
  count = dereference;
}

arger_func(hello_world) {
  char *text = (char *)out;
  for (int i = 0; i < count; i++) {
    printf("%s\n", text);
  }
}

int main(int argc, char *argv[]) {
  PARSER("Example CLI for arger", "v0.1.0\ninformation of version...");

  ARG(count_arg){
      .name = "count",
      .desc = "Select a count.",
      .long_n = 1,
      .short_n = 1,
      .type = ARGER_INT,
      .f = hello_count,
  };

  ARG(text_arg){
      .name = "text",
      .desc = "Select a text.",
      .long_n = 1,
      .short_n = 1,
      .type = ARGER_STRING,
      .req = 1,
      .f = hello_world,
  };

  ARGS(&count_arg, &text_arg);
  CHECK;
}

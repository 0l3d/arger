struct Arger_Parser {
  char *desc;
  int argc;
  char **argv;
};

enum Arger_Type { ARGER_STRING, ARGER_INT, ARGER_FLOAT, ARGER_BOOL };

struct Arger_Arg {
  char *name;
  char *desc;
  int long_n, short_n;
  int req;
  enum Arger_Type type;
  void (*f)(void *);
};

#define PARSER(x, z, y) struct Arger_Parser parser = arger_parser(x, z, y)
#define ARG(x) struct Arger_Arg x =
#define ARGS(...) struct Arger_Arg *args[] = {__VA_ARGS__}
#define CHECK check_args(parser, args, sizeof(args) / sizeof(args[0]))
#define arger_func(x) void x(void *out)

struct Arger_Parser arger_parser(int argc, char **argv, char *desc);
void check_args(struct Arger_Parser parser, struct Arger_Arg **arger, int size);

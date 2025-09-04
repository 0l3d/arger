# ARGER

Arger is a simple, cross-platform and single-header argument parser for C.

<img width="743" height="885" alt="image" src="https://github.com/user-attachments/assets/6a5635b6-3dfb-4efc-b140-7b1d370c99ef" />

## API Reference

The Arger API is designed to be simple and easy to understand using macros.

### Structs

---

`struct Arger_Parser`: High level parser struct.  
 `struct Arger_Arg`: General-Purpose argument struct.

### Functions And Macros

---

If you're using macros, the usage is straightforward:

```c
  PARSER("Example CLI for arger", "v0.1.0");

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
```

`PARSER`: Initializes the main parser.  
`ARG`: Creates new argument.  
`ARGS`: Accepts a collection of ARGs.  
`CHECK`: Parses the arguments and calls the associated functions.

Macro-based function definition:

```c
arger_func(hello_world) {
  char *text = (char *)out;
  for (int i = 0; i < count; i++) {
    printf("%s\n", text);
  }
}
```

`arger_func(name)`: Macro to define an argument-handling function.  
`out`: Refers to the optarg (the value provided with the argument).

---

If you're not using macros, setup becomes slightly more verbose:

```c
  struct Arger_Parser parser = arger_parser(argc, argv, "Arger example code", "v0.1.0");

  struct Arger_Arg count_arg = {
      .name = "count",
      .desc = "Select a count.",
      .long_n = 1,
      .short_n = 1,
      .f = hello_count,
  };

  struct Arger_Arg text_arg = {
      .name = "text",
      .desc = "Select a text.",
      .long_n = 1,
      .short_n = 1,
      .req = 1,
      .f = hello_world,
  };

  struct Arger_Arg *args[] = {&count_arg, &text_arg};
  check_args(parser, args, 2);
  // check_args(parser pointer, args pointer, total arguments);

```

---

## License

This project is licensed under the **GPL-3.0 License**.

## Author

Created by **0l3d**

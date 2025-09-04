# ARGER

Arger is a simple, single-header argument parser for C.

## Example:
`examples/hello_world.c`:
<div align="left">
  <img src="https://github.com/user-attachments/assets/fd6f03c9-5290-4953-9e72-e8084a181ff3" width="476" height="220" alt="image" />  
 
  <img width="315" height="161" alt="image" src="https://github.com/user-attachments/assets/2cfb3bd7-108c-4e8d-b667-c5e69e925908" />  
  
  <img width="539" height="284" alt="image" src="https://github.com/user-attachments/assets/24863c23-ab2a-45fa-8990-63ab53c7fe03" />  
</div>

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
```

`PARSER`: Initializes the main parser.  
`ARG`: Creates new argument.  
`ARGS`: Accepts a collection of ARGs.  
`CHECK`: Parses the arguments and calls the associated functions.

Macro-based function definition:

```c
arger_func(hello_world) {
  for (int i = 0; i < count; i++) {
    printf("%s\n", out);
  }
}
```

`arger_func(name)`: Macro to define an argument-handling function.  
`out`: Refers to the optarg (the value provided with the argument).

---

If you're not using macros, setup becomes slightly more verbose:

```c
  struct Arger_Parser parser = arger_parser(argc, argv, "Arger example code");

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

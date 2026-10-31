# AI Agent Guidance for Banking System

## Purpose
This file tells AI coding agents how to understand and work with this small C++ banking system.

## Project type
- Simple C++ console application with a small class-based design.
- Focus is on account management, input handling, file-based persistence, and display buffering.

## Key files
- `account.h` / `account.cpp`: account model and accessors.
- `database.h` / `database.cpp`: simple file I/O helpers using templates.
- `input.h` / `input.cpp`: console input wrapper with a string specialization.
- `display.h` / `display.cpp`: buffered display output.
- `bank.txt`: sample data file used by the project.

## Conventions
- Uses `using namespace std;` in headers and source files.
- Class names are lowercase: `account`, `database`, `input`, `display`.
- Header guards use double underscores, e.g. `__ACCOUNT_H__`.
- Templates are defined in headers but specializations may be implemented in `.cpp` files.

## What agents should do first
1. Find the project entrypoint. There is currently no `main` defined in the workspace.
2. Prefer small, incremental fixes that preserve the existing design and naming style.
3. Avoid introducing large frameworks or dependencies; keep the implementation simple and console-based.

## Notes for `font`-related work
- There is no UI font or styling system in this repository.
- If asked about `font`, assume the user is referring to text presentation in console output or formatting of displayed text.
- Recommend console-friendly text formatting only, not GUI or graphical font changes.

## If adding files
- Keep file names descriptive and consistent with existing lowercase style.
- Use `#ifndef/#define/#endif` guards in headers.
- Keep I/O simple and portable.

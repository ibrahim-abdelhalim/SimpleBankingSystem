# simpleBankApplication

A small console-based banking application written in C++. It supports account
creation, deposits, withdrawals, balance enquiries, and keeps account data in a
plain-text file between runs.

## Features

- Open a new account with a first name, last name, and initial balance.
- Close an existing account by id.
- Balance enquiry for a given account id.
- Deposit to and withdraw from an account.
- List all accounts.
- Persistent storage in `bank.txt` (loaded on startup, saved on every change).
- Exception-based error handling for invalid names, amounts, and file errors.
- Buffered console output through a small `Display` wrapper.

## Project structure

| File | Purpose |
| --- | --- |
| `main.cpp` | Entry point; creates the `BankApp` and runs the menu loop. |
| `bankapp.h` / `bankapp.cpp` | Console menu, state machine, and user interaction. |
| `bank.h` / `bank.cpp` | `Bank` domain logic and custom exceptions. |
| `account.h` / `account.cpp` | `Account` model (id, name, balance). |
| `database.h` / `database.cpp` | File persistence helpers for the account map. |
| `input.h` / `input.cpp` | Typed console input helper. |
| `display.h` / `display.cpp` | Console output helper. |
| `bank.txt` | Sample data file used by the application. |

## Build

Requires a C++11-capable compiler (tested with MinGW g++ 6.3).

```sh
g++ -std=c++11 main.cpp bankapp.cpp bank.cpp account.cpp database.cpp input.cpp display.cpp -o bankapp
```

Or use the VS Code **C/C++ Runner** configuration in `.vscode/` (Run/Debug).

## Run

```sh
./bankapp
```

On Windows the executable is `bankapp.exe`. The application reads and writes
`bank.txt` in the working directory.

## Usage

The main menu accepts a number:

```
1  Open an Account
2  Close an Account
3  Balance Enquiry
4  Deposit
5  Withdrawal
6  Show All Accounts
7  Quit
```

## Data file format

`bank.txt` stores one account per four lines: id, first name, last name, balance.

```
0
ibrahim1
foley1
1000
1
ibrahim4
foley4
545556
```

## License

No license has been chosen for this project yet.

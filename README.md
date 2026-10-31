# Simple Banking System

A small console-based banking application written in C++. Accounts are created,
closed, deposited to, and withdrawn from through a numbered menu, and every
change is persisted to a plain-text file so data survives between runs.

The codebase is intentionally minimal: seven translation units, no third-party
dependencies, and a layered design (presentation → domain → storage → model)
held together by a small exception hierarchy.

---

## Table of contents

- [Features](#features)
- [Requirements](#requirements)
- [Build](#build)
- [Run](#run)
- [Using the application](#using-the-application)
- [Architecture](#architecture)
- [Class reference](#class-reference)
  - [BankApp](#bankapp)
  - [Bank](#bank)
  - [Database](#database)
  - [Account](#account)
  - [Input](#input)
  - [Display](#display)
  - [Exceptions](#exceptions)
- [Data file format](#data-file-format)
- [Error handling model](#error-handling-model)
- [Project structure](#project-structure)
- [Conventions](#conventions)
- [Known limitations](#known-limitations)
- [Troubleshooting](#troubleshooting)
- [License](#license)

---

## Features

- Open an account with first name, last name, and initial balance.
- Close an account by id.
- Balance enquiry returning the account holder's name and balance.
- Deposit into and withdraw from an existing account.
- List every account in the bank.
- Persistent storage in `bank.txt` — loaded at startup, rewritten on every change.
- Typed console input (`Input::get<T>()`) and buffered output (`Display::put()`).
- Exception-based error handling with a shared base class that records the
  source file and line of every failure.

---

## Requirements

| | |
| --- | --- |
| Language standard | C++11 or later |
| Compiler | Any conforming C++ compiler (verified with MinGW-w64 g++ 6.3, `-std=c++11` and `-std=c++17`) |
| Platform | Windows, Linux, or macOS — no platform-specific code |
| Dependencies | None beyond the C++ standard library |

---

## Build

From the project root:

```sh
g++ -std=c++11 main.cpp bankapp.cpp bank.cpp account.cpp database.cpp input.cpp display.cpp -o bankapp
```

On Windows the output is `bankapp.exe`. To enable warnings (recommended, and the
project currently compiles clean at this level):

```sh
g++ -std=c++11 -Wall main.cpp bankapp.cpp bank.cpp account.cpp database.cpp input.cpp display.cpp -o bankapp
```

Using CMake or an IDE, add all `.cpp` files to the target — there are no special
flags or generated headers.

---

## Run

```sh
./bankapp
```

The application reads and writes `bank.txt` **in the current working directory**.
Run it from the project root, or pass a different path programmatically via the
`BankApp` constructor:

```cpp
BankApp bankapp("My first bank", "bank.txt");
```

---

## Using the application

The menu is printed on start-up and re-printed after each action:

```
	Select one option below
	1 Open an Account
	2 Close an Account
	3 Balance Enquiry
	4 Deposit
	5 Withdrawal
	6 Show All Accounts
	7 Quit
Enter your choice:
```

| Choice | Action | Prompts |
| --- | --- | --- |
| `1` | Open an account | First name, last name, initial balance |
| `2` | Close an account | Account id |
| `3` | Balance enquiry | Account id |
| `4` | Deposit | Account id, amount |
| `5` | Withdrawal | Account id, amount |
| `6` | Show all accounts | — |
| `7` | Quit | — |

Anything outside `1`–`7` prints `Plz, Enter Correct Choice!` and returns to the
menu.

**Rules enforced by the domain layer:**

- First and last name must both be non-empty.
- Initial balance must be non-zero.
- Deposit and withdrawal amounts must be greater than zero.
- A withdrawal may not take the balance below zero.
- The account id must exist for enquiry, deposit, withdrawal, and close.

### Example session

```
	Select one option below
	...
Enter your choice: 1
Enter First Name: Ada
Enter Second Name: Lovelace
Enter Initial Balance: 250
Congradulation Account Is Created.

	Select one option below
	...
Enter your choice: 6
Account ID : 0
Account first name: Ada
Account second name: Lovelace
Account balance : 250
----------------
```

---

## Architecture

Four layers, each depending only on the one below it:

```
        main.cpp
           │
           ▼
     ┌───────────┐
     │  BankApp  │   presentation: menu, prompts, state machine
     └─────┬─────┘
           │  Input / Display  (console I/O helpers)
           ▼
     ┌───────────┐
     │   Bank    │   domain: business rules, validation, orchestration
     └─────┬─────┘
           │
           ▼
     ┌───────────┐
     │ Database  │   storage: load / save / update the account map
     └─────┬─────┘
           │
           ▼
     ┌───────────┐
     │  Account  │   model: id, names, balance
     └───────────┘

     BankException ──► shared by every layer
```

`Bank` owns the in-memory `std::map<int, Account>` and keeps it in sync with
disk: each mutating operation calls `Database::update()`, which writes the file
and reloads it.

**Request flow for a deposit:**

1. `BankApp::run()` reads the id and amount via `Input`.
2. `Bank::deposit(id, amount)` validates the id and amount.
3. On success it updates the in-memory balance and calls `Database::update()`.
4. `Database::update()` calls `saveAccounts()` then `loadAccounts()`.
5. On failure, a `BankException` subclass is thrown and `BankApp` prints it.

---

## Class reference

### BankApp

Console front end; one `run()` call performs one state transition.

| Member | Description |
| --- | --- |
| `BankApp(std::string bankName, std::string dataBaseFileDirectory)` | Constructs the contained `Bank` and starts in `SHOW_MENU`. |
| `void run()` | Executes the current state, then sets the next one. |

Private state machine (`State` enum): `SHOW_MENU`, `OPEN_ACCOUNT`,
`CLOSE_ACCOUNT`, `BALANCE_ENQUIRY`, `DEPOSIT`, `WITHDRAWAL`,
`SHOW_ALL_ACCOUNTS`, `QUIT`, `NONE`.

The menu choice is converted with `static_cast<State>(choice)`, so an
out-of-range number falls through to the `default` branch and returns to the
menu. `main()` calls `run()` in an infinite loop.

### Bank

Domain layer. Owns the account map and the persistence object.

| Method | Description | Throws |
| --- | --- | --- |
| `Bank(std::string bankName, std::string dataBaseFileDirectory)` | Loads existing accounts from storage. | `OpenFileStorage` |
| `void openAccount(std::string fname, std::string lname, long double initialBalance)` | Creates and stores a new account. | `EmptyName`, `InvalidBalance`, `OpenFileStorage` |
| `void closeAccount(int id)` | Erases an account. | `InvalidAccount`, `EmptyAccounts`, `OpenFileStorage` |
| `int getTotalAccounts()` | Number of accounts known to the counter. | — |
| `long double getAccountBalance(int id)` | Balance of one account. | `InvalidAccount` |
| `std::string getAccountFirstName(int id)` | First name. | `InvalidAccount` |
| `std::string getAccountLastName(int id)` | Last name. | `InvalidAccount` |
| `void deposit(int id, long double amount)` | Adds funds. | `InvalidAccount`, `InvalidAmount`, `OpenFileStorage` |
| `void withdraw(int id, long double amount)` | Removes funds. | `InvalidAccount`, `InvalidAmount`, `InvalidWithdrawAmount`, `OpenFileStorage` |
| `void showAllAccounts()` | Prints every account to `std::cout`. | `EmptyAccounts` |

### Database

Text-file persistence for `std::map<int, Account>`.

| Member | Description | Throws |
| --- | --- | --- |
| `Database(std::string fileDirectory = "")` | Stores the path; no I/O yet. | — |
| `void setDirectory(std::string fileDirectory)` | Changes the path. Rejects an empty string. | `EmptyFileDirectory` |
| `void saveAccounts(std::map<int, Account>& accounts)` | Truncates the file and writes all accounts. | `EmptyAccounts`, `OpenFileStorage` |
| `std::map<int, Account> loadAccounts()` | Reads the file into a map. | `OpenFileStorage` |
| `void update(std::map<int, Account>& accounts)` | `saveAccounts()` followed by `loadAccounts()`, replacing the caller's map. | `EmptyAccounts`, `OpenFileStorage` |

`infile` and `oufile` are members rather than locals, and the streams are opened
and closed inside each call.

### Account

Plain data model.

| Member | Description |
| --- | --- |
| `Account(std::string fname = "", std::string lname = "", long double initialBalance = 0)` | Assigns `id = Account::accounts` (the static counter). |
| `Account(const Account&)` | Copy constructor; copies all four fields. |
| `int getId()` / `void setId(int)` | Account identifier. |
| `std::string getFirstName()` / `setFirstName(std::string)` | First name. |
| `std::string getLastName()` / `setLastName(std::string)` | Last name. |
| `long double getBalance()` / `setBalance(long double)` | Balance. |
| `static int getAccountsCounter()` / `static void setAccountsCounter(int)` | Shared next-id counter. |

The static `accounts` counter is the single source of truth for id allocation;
`loadAccounts()` resets it after reading the file.

### Input

| Member | Description |
| --- | --- |
| `template<typename T> T get()` | Reads one whitespace-delimited value with `operator>>`. |
| `void flush()` | Discards one character from the input buffer (`std::cin.ignore()`). |

`Input::get<std::string>` is explicitly specialised in `input.cpp` to use
`std::getline`, so names may contain spaces at input time (but see
[Known limitations](#known-limitations) regarding persistence).

### Display

| Member | Description |
| --- | --- |
| `void put(std::string data)` | Writes `data` to `std::cout` with no added newline or flush. |

All user-visible output goes through `Display`, making it the single place to
redirect or capture console output later.

### Exceptions

Every exception derives from `BankException` (`exception.h`), which stores the
message and prints the originating file and line when thrown:

```cpp
class BankException : public std::exception
{
    private:
    std::string message;

    public:
    BankException(std::string msg, const char* const File, const int Line)
        : message(msg)
    {
        std::cout<< "File: " << File << std::endl; 
        std::cout<< "Line: " << Line << std::endl;
    }

    const char* what() const noexcept override
    {
        return message.c_str(); 
    }
};
```

Derived classes supply only their message:

```cpp
class InvalidAccount : public BankException
{
    public:
    InvalidAccount(const char* const File, const int Line)
        : BankException("#Error: Invalid Account.", File, Line) {}
};
```

| Exception | Message | Raised when |
| --- | --- | --- |
| `InvalidAccount` | `#Error: Invalid Account.` | The requested id is not in the map. |
| `EmptyName` | `#Error: Empty name.` | First or last name is empty. |
| `InvalidAmount` | `#Error: Invalid amount.` | Deposit/withdrawal amount is zero or negative. |
| `InvalidWithdrawAmount` | `#Error: Invalid withdraw amount.` | Withdrawal would make the balance negative. |
| `InvalidBalance` | `#Error: Invalid balance.` | Initial balance is exactly zero. |
| `EmptyFileDirectory` | `#Error: Empty file directory.` | `setDirectory("")` is called. |
| `EmptyAccounts` | `#Error: Empty accounts.` | An operation requires at least one account. |
| `OpenFileStorage` | `#Error: Open file storage failure.` | The data file cannot be opened. |

Because all eight share a base, `catch (BankException& e)` handles every
application error, while the specific types remain available for finer-grained
handling. `what()` is `const noexcept override`, so messages resolve correctly
even when caught as `std::exception&`.

---

## Data file format

`bank.txt` stores four lines per account, in insertion order:

```
<id>
<first name>
<last name>
<balance>
```

Example:

```
0
ibrahim
Abdelhalim
8000
1
Ahemed
Mohamed
10000
```

- The file is rewritten in full (`std::ios::trunc`) on every mutation.
- The `id` written is a sequential counter, not necessarily `Account::getId()` —
  see [Known limitations](#known-limitations).
- Names must not contain whitespace; values are read with `operator>>`.

To reset the application, replace `bank.txt` with an empty file (an empty map
cannot be saved — see below).

---

## Error handling model

1. Each validation failure throws a `BankException` subclass, passing
   `__FILE__` and `__LINE__` so the origin is identifiable.
2. `BankApp::run()` wraps each operation in `try`/`catch` and prints
   `err.what()` through `Display`, followed by a newline or a context message.
3. A trailing `catch (...)` prints `#Error: Undefined error.` for anything
   unexpected.

The constructor of `BankException` prints the file and line immediately, so that
diagnostic appears **before** anything the caller prints — including when the
exception is subsequently caught and handled.

---

## Project structure

| File | Purpose |
| --- | --- |
| `main.cpp` | Entry point; constructs `BankApp` and loops on `run()`. |
| `bankapp.h` / `bankapp.cpp` | Menu, state machine, user interaction, catch blocks. |
| `bank.h` / `bank.cpp` | `Bank` domain logic and five domain exceptions. |
| `account.h` / `account.cpp` | `Account` model and the static id counter. |
| `database.h` / `database.cpp` | File persistence for the account map and three storage exceptions. |
| `exception.h` | `BankException` base class — shared message and `what()`. |
| `input.h` / `input.cpp` | Typed console input helper. |
| `display.h` / `display.cpp` | Console output helper. |
| `bank.txt` | Sample data file. |
| `AGENTS.md` | Guidance for AI coding agents working on this repo. |

---

## Conventions

- Classes are PascalCase (`Bank`, `Account`); files are lowercase (`bank.cpp`).
- Getters and setters are `getId()` / `setId()` style.
- Header guards use the `__NAME_H__` form.
- Methods and control-flow blocks use braces on their own line; simple
  comparisons are written `if (true == x)` / `if (false == y)`.
- Braces and blocks are indented with four spaces.
- Exceptions carry a `#Error:` prefix.

---

## Known limitations

These are behaviours of the current implementation, verified against the code.
They are documented rather than hidden so they can be addressed deliberately.

| Area | Behaviour |
| --- | --- |
| **Id stability** | `saveAccounts()` writes a sequential counter instead of `Account::getId()`, so ids are renumbered after a save/load round trip. An account created as id `99` comes back as id `0`. |
| **Counter drift** | `loadAccounts()` loops while `peek() != EOF`. Because files end with a trailing newline, one extra iteration runs and still increments the counter, so `getAccountsCounter()` reports one more than the number of accounts. |
| **Balance precision** | Balances are written with default stream precision (6 significant digits). `1234567.891` is stored as `1.23457e+006` and reloads as `1234570`. |
| **Names with spaces** | Input accepts them, but the file format is whitespace-delimited. A name like `John Paul` reloads as first name `John`, last name `Paul`, and a zero balance. |
| **Empty bank** | `saveAccounts()` throws `EmptyAccounts` for an empty map, so the last account cannot be deleted through a persisted save. |
| **Zero initial balance** | `openAccount()` rejects an initial balance of exactly `0`, so accounts cannot be opened empty. |
| **Exception side effect** | `BankException` prints file and line from its constructor; the diagnostic cannot be suppressed by the caller. |
| **Quit** | Choosing `7` calls `std::exit(0)` directly, so nothing unwinds and no final save occurs. |
| `Input::flush()` | Discards a single character, which assumes exactly one leftover newline in the buffer. |
| **`Input::get<T>()`** | The generic template declares `type var;` uninitialised, so a failed extraction (e.g. text where a number is expected) returns an indeterminate value and leaves `std::cin` in a failed state. |
| **End of input** | `main()` loops on `run()` unconditionally, so when `std::cin` reaches EOF the menu reprints forever instead of exiting. Redirected or scripted runs need an explicit `7` on stdin. |
| **Streams as members** | `Database` keeps `infile`/`oufile` as members although they are opened and closed per call. |

---

## Troubleshooting

**`g++: command not found`** — install a compiler and put it on `PATH`. On
Windows, MinGW-w64 through MSYS2 or `winget install MSYS2.MSYS2` works; verify
with `g++ --version`.

**`#Error: Open file storage failure.` at startup** — `bank.txt` does not exist
in the working directory. Create an empty file, or run from the project root:

```sh
: > bank.txt          # bash
type nul > bank.txt   # cmd
```

**`#Error: Empty accounts.` when closing the last account** — expected; see
[Known limitations](#known-limitations).

**Menu repeats without accepting input** — the input stream is in a failed
state after a non-numeric entry, or has reached EOF. Press `Ctrl+C` to exit and
restart; when piping input, make sure a `7` is supplied to quit.

---

## License

No license has been chosen for this project yet.

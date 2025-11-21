# 📘 Telpos — Command-Line Shell Written in C

**Telpos** is a lightweight custom command-line shell built from scratch in C.  
It includes essential utilities for working with files, directories, text processing, timers, themes, and command history.

---

## 🚀 Available Commands

### 📄 File Operations

| Command | Description |
|--------|-------------|
| `create <file>` | Creates a new empty file |
| `read <file>` | Displays file content |
| `write <file> <text>` | Writes text to a file (overwrite) |
| `copy <src> <dst>` | Copies a file |
| `rename <old> <new>` | Renames a file |
| `rm <file>` | Deletes a file |
| `cl <file>` | Counts the number of lines in a file |
| `head <file> <n>` | Shows the first `n` lines of a file |
| `tail <file> <n>` | Shows the last `n` lines of a file |
| `grep <text> <file>` | Prints lines that contain `<text>` |

---

### 📁 Directory Operations

| Command | Description |
|--------|-------------|
| `ls` | Lists files and folders in the current directory |
| `cd <path>` | Changes the current directory |
| `mkdir <name>` | Creates a folder |
| `rmdir <name>` | Removes a folder |

---

### 🕒 Time, Timers & System Commands

| Command | Description |
|--------|-------------|
| `time` | Shows current system time and date |
| `timer <seconds>` | A simple countdown timer |
| `clear` | Clears the console |
| `logo` | Prints the Telpos ASCII logo |
| `theme <preset>` | Changes theme (light, dark, classic, etc.) |
| `echo <text>` | Prints text to the console |
| `help` | Displays list of commands |
| `exit` | Exits Telpos |

---

### 📜 Command History

| Command | Description |
|--------|-------------|
| `history` | Shows previously executed commands |

---

## 🔧 Installation (Windows)

```bash
git clone https://github.com/kostia-ssss/Telpos
cd Telpos
mkdir build
gcc source/main.c source/commands.c -o build/Telpos.exe

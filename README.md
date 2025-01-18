
# MTerm

(MinimalTerm) - Minimalist terminal emulator with straight forward config in one file.

### Build: deps(gtk3-devel, vte3-devel)
```bash
sudo make
```
### Config: in main.c
```c
// Config:
const char *font = "Comic Code 21"; // example "Monospace 12";
double transparency = 0.8;          // Default transparency (80%)
const char *shell = "/bin/fish"; // example "/bin/bash";
```

## Screenshots

![App Screenshot](https://github.com/antomfdez/mterm/blob/main/scr.png)

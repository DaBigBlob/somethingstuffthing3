

## get new windows system lib
```sh
cd lib
wget https://github.com/Leandros/WindowsHModular/raw/refs/heads/master/include/win32/libname.h
```

## C to asm
```sh
make cmpl
```
.S will be in `asm/` directory

## cleanup assembly further
```sh
make ascl
```

## asm to exe
```sh
make assm
```
.exe will be in `bin/` directory

## clean up old artifacts
```sh
make clen
```

## C to asm to exe in one go
```sh
make all
```

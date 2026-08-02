# libmpsse programs

Programs the link against [libmpsse](https://github.com/devttys0/libmpsse) for FT232H interface.

## i2c scanner

A small scanner now lives in `i2c_scanner/`.

Build it with:

```
cd i2c_scanner
make
```

The default Makefile assumes `mpsse.h` and `libmpsse` are installed under `/usr/local`.

If your libmpsse headers or library are installed somewhere else, override the flags, for example:

```
make CPPFLAGS='-I/path/to/libmpsse/include' LDFLAGS='-L/path/to/libmpsse/lib' LDLIBS='-lmpsse'
```

Run it with:

```
./i2c_scanner
./i2c_scanner --frequency 400000
./i2c_scanner --repeat 1000
./i2c_scanner --index 0
```



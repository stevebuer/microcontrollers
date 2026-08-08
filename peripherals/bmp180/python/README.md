# BMP180 Python Notes

## Environment

Use the workspace virtual environment so imports resolve consistently:

```bash
source /home/steve/GITHUB/microcontrollers/.venv/bin/activate
```

Or run scripts with the venv interpreter explicitly:

```bash
/home/steve/GITHUB/microcontrollers/.venv/bin/python bmp180-blinka.py
```

## Install Dependencies

From this folder:

```bash
pip install -r requirements.txt
```

If `board` import behavior looks wrong, ensure the unrelated `board` package is not installed:

```bash
pip uninstall -y board
pip install --force-reinstall adafruit-blinka
```

## FT232H / Blinka Mode

For FT232H access through Blinka, run with:

```bash
BLINKA_FT232H=1 /home/steve/GITHUB/microcontrollers/.venv/bin/python bmp180-blinka.py
```

## Quick Checks

Show script usage/output directly:

```bash
/home/steve/GITHUB/microcontrollers/.venv/bin/python bmp180-blinka.py
```

USB visibility check for FT232H:

```bash
lsusb | grep -i 0403:6014
```

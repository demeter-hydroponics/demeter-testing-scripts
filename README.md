# scripts
random scripts

## AIO Test
- Hook up the analog output to GPIO33
- Open a serial monitor with baud rate = 9600
    - Or use platform IO monitor
    - Or use `python3 scripts/build_arduino.py --deploy --monitor`

To run with TOF, uncomment this line: `//#define USE_TOF_SENSE`, and then build/deploy the PIO (see command above as one way) 
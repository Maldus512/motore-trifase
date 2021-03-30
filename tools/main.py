from serial import Serial
from serial.serialutil import SerialException
import termios
from minimalmodbus import Instrument, NoResponseError, InvalidResponseError
import time
import sys

def mainmb():
    port_name = sys.argv[1]

    modbus = Instrument(port_name, 2)
    modbus.serial.baudrate = 115200
    modbus.serial.timeout = 0.2
    modbus.clear_buffers_before_each_transaction = True
    time.sleep(.1)

    try:
        modbus.write_bit(2, 1)
    except (NoResponseError, InvalidResponseError) as e:
        print('Errore di comunicazione:', e)
        return False


def main():
    port_name = sys.argv[1]
    port = Serial(port_name, 115200, timeout=0.2)

    msg = ("stringa lunga per provare " + "pad. "*10)[:50]

    port.write(bytes(msg, "ASCII"))
    print(port.read(100))


if __name__ == '__main__':
    mainmb()
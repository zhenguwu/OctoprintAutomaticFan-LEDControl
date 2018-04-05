import serial
import serial.tools.list_ports
import sys
import termios

def send_message(mes):
    ports = list(serial.tools.list_ports.comports())
    arduino = ""
    for x in ports:
        if "USB2.0-Serial" in x[1]:
            arduino = x[0]
            break
    if arduino != "":
        f = open(arduino)
        attrs = termios.tcgetattr(f)
        attrs[2] = attrs[2] & ~termios.HUPCL
        termios.tcsetattr(f, termios.TCSAFLUSH, attrs)
        f.close()
        ser = serial.Serial()
        ser.baudrate = 9600
        ser.port = arduino
        ser.stopbits = 1
        ser.databits = 8
        ser.open()
        ser.write(str(mes).encode("utf-8"))
        ser.close()
if __name__ == "__main__":
    mes = str(sys.argv[1])
    send_message(mes)


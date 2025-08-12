import serial
import cv2
import numpy as np
import argparse

# JPEG Start and End markers
JPEG_START = b'\xff\xd8'
JPEG_END = b'\xff\xd9'

def read_jpeg_from_serial(ser):
    buffer = b""
    while True:
        byte = ser.read(1)
        if not byte:
            continue

        buffer += byte

        if JPEG_START in buffer:
            start = buffer.find(JPEG_START)
            end = buffer.find(JPEG_END, start)

            if end != -1:
                jpeg_data = buffer[start:end+2]
                buffer = buffer[end+2:]
                return jpeg_data

def main():
    parser = argparse.ArgumentParser(description='Serial JPEG Viewer')
    parser.add_argument('--port', type=str, help='Serial port (e.g., COM3 or /dev/ttyUSB0)')
    parser.add_argument('--baudrate', type=int, default=921600, help='Baudrate (default: 115200)')
    args = parser.parse_args()

    try:
        if not args.port:
            for port_number in range(0, 20):
                try:
                    ser = serial.Serial(f'COM{port_number}', args.baudrate, timeout=1)
                    print(f"Verbunden mit {args.port} @ {args.baudrate} Baud")
                except:
                    continue

        while True:
            try:
                jpeg = read_jpeg_from_serial(ser)
                img_array = np.frombuffer(jpeg, dtype=np.uint8)
                frame = cv2.imdecode(img_array, cv2.IMREAD_COLOR)
                cv2.imshow("Serial Video Stream", frame)
                # ESC-Taste zum Beenden
                if cv2.waitKey(1) & 0xFF == 27:
                    break
            except:
                continue

    except serial.SerialException as e:
        print(f"Fehler beim Öffnen des Serial-Ports:", e)
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
        cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
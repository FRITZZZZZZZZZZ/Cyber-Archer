import serial
import cv2
import numpy as np
import argparse

# --- Auflösung der Kamera einstellen ---
WIDTH = 1600
HEIGHT = 1200

# Framegröße bei RGB565 (2 Bytes pro Pixel)
RGB565_FRAME_SIZE = WIDTH * HEIGHT * 2

def read_rgb565_from_serial(ser):
    """Liest ein komplettes RGB565-Frame aus der seriellen Schnittstelle."""
    buffer = b""
    while len(buffer) < RGB565_FRAME_SIZE:
        data = ser.read(RGB565_FRAME_SIZE - len(buffer))
        if not data:
            continue
        buffer += data
    return buffer

def rgb565_to_bgr(rgb565_bytes):
    """Wandelt RGB565-Rohdaten in ein OpenCV-BGR-Bild um."""
    # Bytes in 16-Bit-Werte umwandeln
    arr = np.frombuffer(rgb565_bytes, dtype=np.uint16)
    arr = arr.reshape((HEIGHT, WIDTH))

    # RGB565 nach BGR888 konvertieren
    r = ((arr >> 11) & 0x1F) << 3
    g = ((arr >> 5) & 0x3F) << 2
    b = (arr & 0x1F) << 3

    bgr = np.dstack((b, g, r)).astype(np.uint8)
    return bgr

def main():
    parser = argparse.ArgumentParser(description='Serial RGB565 Viewer')
    parser.add_argument('--port', type=str, required=True, help='Serial port (e.g., COM3 or /dev/ttyUSB0)')
    parser.add_argument('--baudrate', type=int, default=921600, help='Baudrate (default: 921600)')
    args = parser.parse_args()

    try:
        ser = serial.Serial(args.port, args.baudrate, timeout=1)
        print(f"Verbunden mit {args.port} @ {args.baudrate} Baud")

        while True:
            rgb565_data = read_rgb565_from_serial(ser)
            frame_bgr = rgb565_to_bgr(rgb565_data)
            cv2.imshow("Serial RGB565 Stream", frame_bgr)

            if cv2.waitKey(1) & 0xFF == 27:  # ESC zum Beenden
                break

    except serial.SerialException as e:
        print("Fehler beim Öffnen des Serial-Ports:", e)
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
        cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
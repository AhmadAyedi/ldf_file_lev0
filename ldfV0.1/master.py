import serial
import time
import RPi.GPIO as GPIO
import re

# GPIO setup
SLP_PIN = 18
GPIO.setmode(GPIO.BCM)
GPIO.setup(SLP_PIN, GPIO.OUT)
GPIO.output(SLP_PIN, GPIO.HIGH)  # Wake up LIN module

# Serial setup
linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

def parse_ldf(ldf_file):
    schedule_table = []
    with open(ldf_file, 'r') as file:
        content = file.read()
        
        # Parse schedule table using regex
        schedule_pattern = r'Cmd: "publish" (frame\d) (\d+) ms;'
        matches = re.findall(schedule_pattern, content)
        
        frame_to_message = {
            'frame1': ('3', 'Hello arduino1'),  # ID 3 for Slave 1
            'frame2': ('2', 'Hello arduino2')   # ID 2 for Slave 2
        }
        
        for frame, period in matches:
            message_id, message = frame_to_message[frame]
            schedule_table.append({
                "id": message_id,
                "message": message,
                "period": int(period) / 1000  # Convert ms to seconds
            })
    
    return schedule_table

def send_message(message, message_id):
    formatted_message = f"{message_id}:{message}\n"
    linSerial.write(formatted_message.encode('utf-8'))
    print(f"Sent: {formatted_message.strip()}")
    time.sleep(1)

try:
    print("Master Ready")
    schedule_table = parse_ldf("ldf2.ldf")  # Parse the LDF file
    while True:
        for entry in schedule_table:
            send_message(entry["message"], entry["id"])
            time.sleep(entry["period"])
except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
    GPIO.cleanup()

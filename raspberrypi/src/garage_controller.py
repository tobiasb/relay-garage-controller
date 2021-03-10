from flask import Flask
import RPi.GPIO as GPIO
import time
import sys

app = Flask(__name__)
pin = 22


@app.route('/health', methods=['GET'])
def status():
    return {
        "status": "running"
    }

@app.route('/garage/trigger', methods=['POST'])
def send_trigger():
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(pin, GPIO.OUT, initial=GPIO.LOW)
    GPIO.output(pin, GPIO.LOW)

    # This delay is necessary for some reason I haven't figured out yet
    # If not there, the behavior is inconsistent
    time.sleep(1) 
  
    # Send a 1 second "high"
    print("Output -> High")
    GPIO.output(pin, GPIO.HIGH)
    time.sleep(1)
    print("Output -> Low")
    GPIO.output(pin, GPIO.LOW)
    GPIO.cleanup()
    print("Done")
    return {}

from __future__ import absolute_import
import octoprint.plugin
import RPi.GPIO as GPIO

def initPins():
    GPIO.setmode(GPIO.BCM)
    #setting GPIO 4 to high when printer is preheated
    GPIO.setup(4, GPIO.OUT)
    GPIO.output(4, GPIO.LOW)
    
def fan(comm, parsed_temps):
    if parsed_temps.has_key('T0'):
        ext1 = int(list(parsed_temps['T0'])[1])
        if ext1 >= 50:
            GPIO.output(4, GPIO.HIGH)
        elif parsed_temps.has_key('T1'):
            ext2 = int(list(parsed_temps['T1'])[1])
            if ext2 >= 50:
                GPIO.output(4, GPIO.HIGH)
            elif parsed_temps.has_key('T2'):
                ext3 = int(list(parsed_temps['T2'])[1])
                if ext3 >= 50:
                    GPIO.output(4, GPIO.HIGH)
                elif parsed_temps.has_key('T3'):
                    ext4 = int(list(parsed_temps['T3'])[1])
                    if ext4 >= 50:
                        GPIO.output(4, GPIO.HIGH)
                    else:
                        GPIO.output(4, GPIO.LOW)
    return parsed_temps

__plugin_name__ = "Octoprint Temperature LED 12864"
__plugin_author__ = "Michaelwu21"
__plugin_author_email__ = "Michaelwu21@gmail.com"
__plugin_version__ = "1.0"
__plugin_description__ = """Check if printer is heating to update LEDS"""
__plugin_implementation__ = initPins()
__plugin_hooks__ = {
    "octoprint.comm.protocol.temperatures.received": fan
}

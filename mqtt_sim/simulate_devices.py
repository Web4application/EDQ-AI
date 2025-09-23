import paho.mqtt.client as mqtt
import time, json
import random

client = mqtt.Client()
client.connect("localhost", 1883)
client.loop_start()

devices = ["drone01","cam02","sensor03"]

while True:
    device = random.choice(devices)
    event = {
        "device_id": device,
        "timestamp": int(time.time()),
        "detections": [
            {"type":"object","class_id":0,"confidence":round(random.uniform(0.7,1.0),2)},
            {"type":"anomaly","description":"pattern "+str(random.randint(1,5))}
        ]
    }
    client.publish("edq_ai/events/"+device, json.dumps(event))
    time.sleep(2)

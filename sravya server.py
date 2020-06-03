import mysql.connector
from mysql.connector import Error
from datetime import datetime, timezone
from dateutil import tz
import paho.mqtt.client as mqtt
import mysql.connector
import json
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("distance")
def on_message(client, userdata, msg):
    print("topic: "+msg.topic+" data: "+str(msg.payload))
    data=str(msg.payload.decode("utf-8","ignore"))
    print(data)
    print(type(data))
    data_in=json.loads(data)
    print(data_in)
    add_data(data_in['distance'])
    

   
def add_data(distance):
    from_zone = tz.gettz('UTC')
    to_zone = tz.gettz('Asia/Kolkata')
    utc = datetime.now().replace(tzinfo=from_zone)
    dt = utc.astimezone(to_zone)
    dt=str(dt)
    try:

        connection = mysql.connector.connect(host='localhost',
                                             database='sdatabase',
                                             user='root',
                                             password='Ramadevi@69')
        cursor = connection.cursor()
        mySql_insert_query = "INSERT INTO sravya(distance, dt) VALUES ('"distance"','"+dt+"') "
        cursor.execute(mySql_insert_query)
        connection.commit()
        print("Record inserted successfully into sravya table")

    except mysql.connector.Error as error:
        print("Failed to insert into MySQL table {}".format(error))
    finally:
        if (connection.is_connected()):
            cursor.close()
            connection.close()
            print("MySQL connection is closed")




client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("3.218.230.74", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()

import paho.mqtt.client as mqtt
import serial
# El callback para cuando el cliente reciva un Connack de respuesta del servidor. 
def on_connect(client, userdata, flags, rc):
    print("Conectado con el siguiente codigo  "+str(rc))
    #Suscribirse dentro de On_connect signidica que si perdemos la conexion 
    #y nos reconectamos la suscripcion se renovara.
    client.subscribe(MQTT_topicSub)

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))


MQTT_broker = "m11.cloudmqtt.com"
MQTT_username = "miidnlzk"
MQTT_password = "zjUYiSYadzw4"
MQTT_topicSub = "jsonTopic"  #<---topico al que te vas a suscribir
MQTT_topicAlive = "Conection"#topico al que le vas a decir que ya estas conectad
MQTT_port = 10783
MQTT_clientID = "Hackerspace"#identificador de cliente
client = mqtt.Client(MQTT_clientID, clean_session=True, userdata=None, protocol="MQTTv311", transport="tcp")
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set(MQTT_username,MQTT_password )
client.connect(MQTT_broker, MQTT_port, 60)
client.publish(MQTT_topicAlive,MQTT_clientID, qos=0, retain=False)
client.publish("Data","Hola mundo :D", qos=0, retain=False)

puertoserial=0;
mensaje = 0;
def configurarSerial():
	puerto = raw_input("Hola /n Inserte el puerto a configurar: ")
	global puertoserial
	puertoserial = serial.Serial(puerto)
	print puertoserial

def leerSerial():
	global puertoserial
	global mensaje
	print "leerSerial"
	puertoserial.write('1')

	mensaje= puertoserial.readline()
	print mensaje
	mensaje= puertoserial.readline()
	print mensaje
	mensaje= puertoserial.readline()
	print mensaje
	mensaje= puertoserial.readline()
	print mensaje
	client.publish("/json",mensaje, qos=0, retain=False)
def escribirSerial():

	print "leerSerial"

configurarSerial()
leerSerial()
puertoserial.close()

#client.loop_forever()

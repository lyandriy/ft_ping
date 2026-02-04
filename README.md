# ft_ping

https://www.lowi.es/glosario/ping/
https://www.ionos.es/digitalguide/servidores/know-how/que-es-el-protocolo-icmp-y-como-funciona/
ICMP socket - https://ekman.cx/articles/icmp_sockets/
socket_raw - https://www.hackplayers.com/2017/05/en-crudo-y-sin-censura-raw-sockets-i-en-c.html
             https://www.cloudflare.com/es-es/learning/ddos/glossary/internet-control-message-protocol-icmp/
             https://aws.amazon.com/es/what-is/icmp/
             https://personales.upv.es/rmartin/TcpIp/cap02s04.html

MAPA
init()
resolve destination
create socket
while (running)
    build ICMP packet
    send packet
    wait for reply (or timeout)
    parse reply
    print result
cleanup()

ICMP:
1️⃣ El sobre del paquete

Un paquete ICMP siempre tiene información organizada en campos, como un sobre con etiquetas:

Campo	Qué significa
Tipo (Type)	Qué tipo de mensaje es. Ej.: “ping” o “respuesta al ping”.
Código (Code)	Subtipo del mensaje, normalmente 0 para ping.
Checksum	Número que ayuda a verificar que la carta no se dañó en el camino.
Identificador (ID)	Quién envía la carta, como tu nombre o número de cuenta.
Número de secuencia	Qué número de carta es, para no confundirse si envías varias.
Datos (Data)	Mensaje opcional, texto o cualquier información extra.
2️⃣ Tipos de mensajes importantes

Para ping usamos solo:

Echo Request (8) → “Hola, ¿estás ahí?”

Echo Reply (0) → “Sí, estoy aquí”

💡 Tipo = 8 → pregunta, Tipo = 0 → respuesta

3️⃣ Cómo se ve mentalmente

Imagina que tu programa envía esto:

Tipo: Echo Request (8)
Código: 0
ID: 1234  ← identifica TU programa
Seq: 1   ← primer ping
Datos: “Hola mundo”


Cuando el otro ordenador responde:

Tipo: Echo Reply (0)
Código: 0
ID: 1234
Seq: 1
Datos: “Hola mundo”  ← lo mismo que enviaste

4️⃣ Para qué sirve cada campo en práctica

Tipo/Code → saber si es pregunta o respuesta

ID → asegurarte de que es TU ping y no el de otro programa

Seq → saber qué número de ping corresponde

Datos → opcional, pero útil para llenar tamaño y probar velocidad

Checksum → verificar que el mensaje no se corrompió
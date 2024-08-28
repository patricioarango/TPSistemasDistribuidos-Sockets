
# Trabajo Práctico TCP/IP Socket  - Sistemas Distribuidos

## Patricio Arango DNI 29119499

### Funcionamiento general:
El servidor codificado en C genera el socket en el puerto 5000, definido a priori, dónde se conectan tanto el cliente en C como el de Python. El servidor recibe una serie de comandos y en base a eso, ejecuta diferentes acciones:

- ”usua”: Que dispara la generación de un nombre de usuario.
- ”cont”: Que inicia la producción de una contraseña.
- ”clos”: Cierra la conexión del socket.  

Una vez recibido el comando, el servidor analiza la longitud pedida en cada caso y valida si coincide con las especificaciones, caso contrario envía un mensaje al cliente con el aviso de error correspondiente.

### Generación de Usuario Aleatorio:
El requisito indica que al generar un usuario, si el primer carácter del es vocal, el siguiente será consonante y así de forma sucesiva hasta alcanzar la extensión pedida por el cliente. Entonces, el servidor, junto con el comando “usua” recibe la longitud y valida que no sea menor a 5 ni mayor a 15. El primer paso es generar un número aleatorio que, de ser par, el string comienza con vocal. Caso contrario, con consonante. El servidor cuenta con un arreglo de vocales y otro de consonantes y a medida que se genera la cadena, se toma el carácter desde el arreglo correspondiente con un índice random.

### Generación de Password Aleatorio:
El servidor, en este caso, cuenta con un arreglo alfanumérico que incluye las minúsculas y mayúsculas de cada letra y los números. Una vez que se recibe el comando “cont”, se valida que la extensión sea de al menos 8 caracteres de mínimo y 49 de máximo. Si la extensión es válida se genera un índice random para cada posición de la cadena. 

### Los Clientes:
Tanto el cliente en C como el de Python cuentan con un menú, que le permite al usuario elegir qué función quiere ejecutar: generar un usuario, fabricar una contraseña o desconectar el cliente. Ambos clientes validan que el dato ingresado sea un número y se envía el comando al servidor para esperar la respuesta. 



# Test VGA ESP32
Test de los modos de vídeo en ESP32 desde el menú utilizando el teclado PS/2 o teclado remoto a través de terminal, como Putty.
<br>
Seleccione el modo de vídeo con las teclas de flecha arriba y abajo y pulse ENTER para aceptar.
<ul>
 <li>360x200x70hz bitluni</li>
 <li>320x240x60hz bitluni</li>
 <li>320x240x60hz fabgl</li>
 <li>QVGA 320x240x60hz fabgl</li>
 <li>320x200x70hz bitluni</li>
 <li>320x200x70hz fabgl</li>
 <li>360x400x70.4hz bitluni</li>
 <li>400x300x56.2hz bitluni</li>
 <li>320x350x70hz bitluni</li>
 <li>320x400x70hz bitluni</li>
 <li>640x400x70hz bitluni</li>
 <li>TTGOVGA32 PAL CVBS 5V (PAL CVBS GPIO 26)</li>
 <li>TTGOVGA32 PAL CVBS 5V+ (PAL CVBS GPIO 26)</li>
 <li>ESP32 PAL CVBS 3V (PAL CVBS GPIO 26)</li>
 <li>TTGOVGA32 NTSC CVBS 5V (NTSC CVBS GPIO 26)</li>
 <li>TTGOVGA32 NTSC CVBS 5V+ (NTSC CVBS GPIO 26)</li>
 <li>ESP32 NTSC CVBS 3V (NTSC CVBS GPIO 26)</li>
</ul>


<br><br>
<h1>Versión precompilada</h1>
En la carpeta precompile se encuentra la versión compilada para poder ser grabada con el flash download tool 3.9.2.<br><br>
<a href='https://github.com/rpsubc8/ESP32TestVGA/tree/main/ESP32/precompile'>https://github.com/rpsubc8/ESP32TestVGA/tree/main/ESP32/precompile</a>
<br><br>
Debemos de elegir el tipo ESP32:
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TestVGA/main/preview/flash00.gif'></center>
Posteriormente, seleccionaremos los archivos tal y como la captura adjunta, con los mismos valores de offset:
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TestVGA/main/preview/flash01.gif'></center>
Y le daremos a start. Si todo ha sido correcto, sólo tendremos que reiniciar el ESP32.


<br><br>
<h1>Requerimientos</h1>
Se requiere:
 <ul>
  <li>TTGO VGA32 v1.x (1.0, 1.1, 1.2, 1.4)</li>
  <li>Visual Studio 1.66.1 PLATFORMIO 2.5.0 Espressif32 v3.5.0</li>
  <li>Arduino IDE 1.8.11 Espressif System 1.0.6</li>  
  <li>Librería reducida Arduino bitluni 0.3.3 (incluida en proyecto)</li>
 </ul>
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TestVGA/main/preview/ttgovga32v12.jpg'></center> 
<br>


<br><br>
<h1>ArduinoDroid</h1>
Se debe ejecutar, sólo una vez, el script makeandroid.bat, que nos deja toda la estructura de datos del directorio dataFlash en el directorio raiz, así como reemplazando las archivos principales con llamadas de los .h sin usar el directorio de datos dataFlash.<br>
Al finalizar, el propio script, termina borrando el directorio dataFlash.<br>
El script utiliza el fart.exe (find and replace text).<br>
Una vez, se haya ejecutado con éxito, se puede usar con el ArduinoDroid.


<br><br>
<h1>Arduino Web Editor</h1>
Se debe ejecutar, sólo una vez, el script makeandroid.bat. Una vez finalizado, se sube a la nube como un proyecto cualquiera, ya sea comprimido en zip o por archivos.


 
<h1>PlatformIO</h1>
Se debe instalar el PLATFORMIO 2.5.0 desde las extensiones del Visual Studio. Se requiere también Espressif32 v3.5.0. 
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TestVGA/main/preview/previewPlatformIOinstall.gif'></center>
Luego se seleccionará el directorio de trabajo <b>ESP32TestVGA</b>.
Debemos modificar el fichero <b>platformio.ini</b> la opción <b>upload_port</b> para seleccionar el puerto COM donde tenemos nuestra placa TTGO VGA32, si por algun motivo no nos lo detecta.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TestVGA/main/preview/previewPlatformIO.gif'></center>
Luego procederemos a compilar y subir a la placa. No se usa particiones, así que debemos subir todo el binario compilado.
Está todo preparado para no tener que instalar las librerias de bitluni ni fabgl.


<br><br>
<h1>Arduino IDE</h1>
Todo el proyecto es compatible con la estructura de Arduino 1.8.11.
Tan sólo tenemos que abrir el <b>testvga.ino</b> del directorio <b>testvga</b>.
<center><img src='https://raw.githubusercontent.com/rpsubc8/ESP32TestVGA/main/preview/previewArduinoIDEpreferences.gif'></center>
Debemos instalar las extensiones de spressif en el gestor de urls adicionales de tarjetas <b>https://dl.espressif.com/dl/package_esp32_index.json</b>
<br>
Ya está preparado el proyecto, de forma que no se necesita ninguna librería de bitluni ni fabgl.
Debemos desactivar la opción de PSRAM, y en caso de superar 1 MB de binario, seleccionar 4 MB de partición a la hora de subir. Aunque el código no use PSRAM, si la opción está activa y nuestro ESP32 no dispone de ella, se generará una excepción y reinicio del mismo en modo bucle.




<br><br>
<h1>Teclado UART</h1>
Se se activa la opción <b>use_lib_keyboard_uart</b>, se permite usar el teclado del PC desde el monitor VStudio o desde el putty (115200 bauds), de manera simple, dado que no es mediante lectura SCANCODE down, up:
<ul>
 <li><b>Tecla TAB o tecla F2:</b> Muestra OSD</li>  
 <li><b>ENTER</b>: Envía ENTER (aceptar)</li>
 <li><b>Arriba:</b> Arriba (incremento 1)</li>
 <li><b>Abajo:</b> Abajo (decremento 1)</li>
 <li><b>Derecha:</b> Derecha (incremento 10)</li>
 <li><b>Izquierda:</b> Izquierda (decremento 10)</li>
</ul>
Desde el Arduino IDE, no se permite dicha funcionalidad, dado que el monitor serie requiere el envio del ENTER por cada acción.


<br><br>
<h1>Opciones</h1>
El archivo <b>gbConfig.h</b> se seleccionan las opciones:
<ul> 
 <li><b>use_lib_log_serial:</b> Se envian logs por puerto serie usb</li> 
 <li><b>use_lib_keyboard_poll_milis:</b> Se debe especificar el número de milisegundos de polling para el teclado.</li> 
 <li><b>FIX_PERIBOARD_NOT_INITING:</b> Solución realizada por <b>dcrespo3D</b> para la inicialización en algunos teclados.</li>
 <li><b>use_lib_fix_double_precision</b> No usa el cálculo de frecuencia de video VGA con el propio ESP32, evitando posibles problemas de precisión con la mantisa. Es útil para ESP32's que calculen mal la frecuencia.</li>
 <li><b>use_lib_debug_i2s</b> Traza con los cálculos del modo de video.</li>
 <li><b>use_lib_vga360x200x70hz_bitluni</b> Modo de video 360x200 con los parámetros de bitluni.</li>
 <li><b>use_lib_vga320x200x70hz_bitluni</b> Modo de video 320x200 con los parámetros de bitluni.</li>
 <li><b>use_lib_vga320x200x70hz_fabgl</b> Modo 320x200 con los parámetros de fabgl.</li>
 <li><b>use_lib_keyboard_uart:</b> Permite usar el teclado del PC desde el PC por terminal monitor VStudio o desde el putty, sin falta de tener teclado. Útil para no tener que usar el teclado físico PS/2 y para desarrollo.</li>
 </ul>


<br><br>
<h1>DIY circuito</h1>
Si no queremos usar una placa TTGO VGA32 v1.x, podemos construirla siguiendo el esquema de <b>fabgl</b>:
<center><img src='preview/fabglcircuit.gif'></center>
Para el caso de querer salida de video cvbs, en lugar de VGA, debemos de sacar un cable directo del pin 26 del conector PS/2 del ratón, activando la opción <b>use_lib_cvbs_pal</b>, así como <b>use_lib_cvbs_ttgo_vga32</b> del <b>gbConfig.h</b>. Si no activamos dicha opción, la salida será de más de 1 voltio, teniendo que ser reducida con un reductor de voltaje (potenciómetro).
<center><img src='preview/ps2.gif'></center>
El conector de PS/2 es visto desde el propio jack de la placa, es decir, jack hembra. El pin en PS/2 es el CLK, es decir, el 5.
<center><img src='preview/ttgops2cvbs.gif'></center>
En esta imagen se puede ver el mosfet SOT23 interno de la placa TTGO VGA32, de manera, que la salida en CLK (pin 5) es 5 voltios.


<br><br>
<h1>Test DAC cvbs</h1>
Para TTGO VGA32 como la salida es 5v, o hacemos reducción de voltaje o podemos reducir la escala del DAC. En 3.3v de salida, con máximo el valor de 77, ya nos daría 0.99v, que sería 1v. Si tenemos 5v de salida, con 50, ya tenemos 0.97v, que sería 1v. De esta forma, ya no necesitamos resistencias reductoras, es el cable directo. Mientras no nos pasemos de 77 en 3.3v o 50 en 5v, no tendremos problema, sobre todo si sólo necesitamos 2 colores (blanco y negro).
Podemos hacer pruebas con un multímetro, sobre todo en la TTGO VGA32 v1.x:
<pre>
//ESP32 Pin 26
//DAC - Voltaje
//  0 - 0.06
// 38 - 0.52
// 77 - 1
//255 - 3.17

#include <Arduino.h>
#include <driver/dac.h>

const int arrayValue[4]={0,38,77,255};
unsigned char cont=0;

void setup() {
 Serial.begin(115200);
 dac_output_enable(DAC_CHANNEL_2);
}

void loop() {
 dac_output_voltage(DAC_CHANNEL_2, arrayValue[cont]);
 Serial.printf("%d\n",arrayValue[cont]);
 delay(4000);
 cont++;
 cont &= 0x03;
}
</pre>
Los valores máximos al escribir en el buffer de video en una placa ESP32 es de 54, mientras que para TTGO VGA32 v1.x sería de 35.


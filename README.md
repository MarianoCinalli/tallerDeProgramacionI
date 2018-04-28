# Trabajos practicos de Taller de Programación I FIUBA

- Facultad de Ingenieria
- Universidad de Buenos Aires

## Integrantes:

- Mariano Cinalli [@MarianoCinalli](https://github.com/MarianoCinalli)
- Joaquin Carbon [@joakoracing](https://github.com/joakoracing)
- Claudio Smith [@claucapo](https://github.com/claucapo)
- Nicolas Cian [@nicolascian](https://github.com/nicolascian)
- Juan D'Ambra [@juandambra](https://github.com/juandambra)

### Para compilar:
Se puede usar el script `compile.sh`:  
    compile.sh [-hcsr]  
        Help:  
        `-c` Compila solo el cliente.  
        `-s` Compila solo el server.  
        `-r` Corre las applicaciones luego de compilar. Corren en background.  
        Si no se especifica, server o cliente, compila ambos.  

O, directamente, los makefiles dentro de los directorios `client` y/o `server`:  
    `sudo make dependencies` - Instala las librerias necesarias.  
    `make` - Borra los archivos existentes de la compilacion anterior y compila.  
    `make` clean - Borra los archivos existentes de la compilacion anterior (main y .obj)  

### Instrucciones del juego:

#### Archivo de configuracion:
* `equipo`  
  * formacion: '3-3', '3-1-2', '3-2-1',    
  * casaca: 'alternativa', 'principal', 'alternativa'  
* `debug`  
  * level: 'debug', "info", "error"  
* `assets`    
  * cancha: 'images/cancha.png'  
  * sprites: 'images/sprites.png' 
* `performance`    
  * framerate: 2  
  * margenes: "80"  

#### Flags:
`-V, --version`      Imprimir version y salir.  
`-h, --help`         Imprimir esta ayuda.  
`-i, --input`        Path del archivo de configuracion YAML.  
`-l, --logLevel`     El nivel de log para la aplicacion.  

#### Controles:
* `flechas` trotar  
* `w` correr  
* `a` barrida  
* `s` patear 
* `barra` cambiar de jugador  

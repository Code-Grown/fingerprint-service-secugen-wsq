# FingerPrint Service WSQ

## Requerimientos para la instalación:

- [x] Distribución Linux (Probado en Debian 9, 10, 11)
- [x] Secugen Hamster Plus FingerPrint Device
- [x] Docker
- [x] Docker Compose


## Manual de Instalación con Docker:

- Conectar el dispositivo SecuGen Hamster Plus.
- Todos los requerimientos a nivel de software, configuraciones, herramientas, etc. están cubiertas en el fichero dockerfile.
- Toda la instalacion queda embebida dentro del contenedor, lo cual no es necesario instalarlo localmente.

- Configurar variables de entorno:

```
FLASK_SECRET_KEY=tokenaleatorio
DEBUG=False
```

Deploy Local con Docker Compose
------------------------------------
En el directorio del proyecto, en donde se encuentra el archivo docker-compose.yml, está el servicio creado para una versión productiva.

### Ojo: Debes tener instalado docker y docker-compose en tu maquina local

Puedes crear los siguentes aliases en tu archivo ~/.bashrc o ~/.zshrc para simplificar la ejecución:

```
alias dc='docker-compose'
alias dce='docker-compose exec'
alias dcl='docker-compose logs'
alias dclf='docker-compose logs -f'
alias dcup='docker-compose up'
alias dcdown='docker-compose down'
alias dcstop='docker-compose stop'
```


Para poder levantar el ambiente local solo debes ejecutar la siguiente instrucción y esperar que se instalen las dependencias configuradas en el dockerfile que tiene asociado dicho ambiente.

Ejecutando docker compose para enviarlo a background directamente y ver los logs después de su ejecución:

```
docker-compose up --build fingerprint_service
```
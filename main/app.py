#!/usr/local/bin/python
# -*- coding: utf-8 -*-
from flask import Flask
from flask_sock import Sock
from os import environ as env

# Gunicorn
from utils.log import Log
from utils.profiling import Profiling

# Flask Run
# from .utils.log import Log
# from .utils.profiling import Profiling

import json, subprocess, base64

app = Flask(__name__)
app.config['SECRET_KEY'] = env.get('FLASK_SECRET_KEY') or 'secret!'
app.debug = env.get('DEBUG') or False

sock = Sock(app)

@app.route('/')
def index():
    return "Index # Biometrics API"

@sock.route('/api/canal')
def reverse(sock):
    while True:

        ENCODING = 'utf-8'
        FINGERPATH = '/main'
        SYSTEMCALLACTION = 'sudo_wsq_direct_capture_fdu03'
        SYSTEMCHECKDEVISE = 'sudo_wsq_check_devise_fdu03'
        SYSTEMCLOSE = "killdevice"

        mensaje = json.loads( sock.receive() )
        comando = mensaje['comando']
        rsdata = {}
        p = None

        def make_call_device(action_command):
            profiling = Profiling()
            profiling.init_profiling()
            Log.debug(f'Realizando profiling: {action_command}')
            Log.warning(f'Realizando llamada al sistema: {action_command}')
            p = subprocess.run(
                action_command,
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                check=True
            )
            exec_time = profiling.end_profiling()
            Log.warning(f'Llamada al sistema finalizada: {action_command}')
            Log.debug(f'Profiling finalizado, tiempo de ejecución -> {exec_time} segundos: {action_command}')
            return p

        Log.warning(f'Procesando mensaje: {comando}')

        if comando == 'chequear_dispositivo':
            try:
                Log.info(f'Comprobando dispositivo')
                make_call_device(SYSTEMCHECKDEVISE)
                Log.info(f'Dispositivo conectado')
            except subprocess.CalledProcessError as err:
                Log.critical(f'Dispositivo NO conectado')
                rsdata = {
                    "success": "false",
                    "status": "500",
                    "return_code": "255",
                    "message": "Error con el dispositivo de biometría",
                    "tipo": "chequeo_dispositivo"
                }
        elif comando == 'iniciar_transaccion':
            try:
                Log.info(f'Iniciando transaccion de lectura biométrica')
                result = make_call_device(SYSTEMCALLACTION)
                Log.info(f'Transacción iniciada')
            except:
                rsdata = {
                    "success": "false",
                    "status": "500",
                    "return_code": "255",
                    "message": "Error con el dispositivo de biometría",
                    "tipo": "chequeo_dispositivo"
                }
                sock.send(rsdata)
                return
            if (result.returncode == 0):
                Log.info(f'Transacción de lectura biométrica completada')
                with open(f"{FINGERPATH}/finger.wsq", "rb") as finger_image:
                    Log.info(f'Codificando imagen biométrica')
                    b64_encode = base64.b64encode(finger_image.read())
                    b64_str = b64_encode.decode(ENCODING)
                    Log.info(f'Imagen biométrica codificada')
                rsdata = {
                    "success": "true",
                    "status": "200",
                    "return_code": f"{result.returncode}",
                    "encoded_image": f"{b64_str}",
                    "tipo": "huella_leida"
                }
            else:
                Log.critical(f'Error al realizar la lectura biométrica, finalizando transacción')
                rsdata = {
                    "success": "false",
                    "status": "500",
                    "return_code": f"{result.returncode}",
                    "message": "Error de lectura",
                    "tipo": "chequeo_dispositivo"
                }
        elif comando == 'finalizar_transaccion':
            try:
                Log.info(f'Finalizando transacción')
                make_call_device(SYSTEMCLOSE)
                Log.info(f'Transacción finalizada correctamente')
            except subprocess.CalledProcessError as err:
                pass

            rsdata = {
                "success": "true",
                "status": "200",
                "tipo": "cerrar_dispositivo"
            }
        Log.info(f'Retornando información al websocket')
        sock.send(rsdata)
        Log.info(f'Finalizado.')

FROM python:3.9.2-slim-buster

USER root

RUN mkdir /main
RUN adduser --disabled-password secugen
RUN chown secugen:secugen /main

# set environment variables for build time
ENV DEBIAN_FRONTEND=noninteractive
ENV LC_ALL=C.UTF-8
ENV LANG=C.UTF-8
ENV PATH="/home/secugen/.local/bin:${PATH}"



WORKDIR /main


RUN apt update && apt upgrade -y && apt install -y \
  # devise deps
  libgtk2.0-dev g++ gcc libusb-dev \
  # debud deps
  sudo curl nmap gcc git bash nano tzdata htop net-tools iputils-ping cron systemd build-essential procps \
  # build deps
  build-essential software-properties-common software-properties-gtk python3 python3-dev python3-pip openssl \
  git bash nano sudo tzdata curl htop usbutils



RUN echo 'alias ll="ls -la"' >> ~/.bashrc
RUN echo 'alias runapp="flask run -p 5000 -h 0.0.0.0"' >> ~/.bashrc

# Usando Threads
#RUN echo 'alias guniapp="gunicorn --bind 0.0.0.0:5000 app:app --reload --workers=2 --threads=4 --log-file=- --worker-tmp-dir /dev/shm --worker-class=gthread"' >> ~/.bashrc

# Usando Eventlet
RUN echo 'alias guniapp="gunicorn --bind 0.0.0.0:5000 app:app --reload --workers=2 --threads=4 --log-file=- --worker-tmp-dir /dev/shm --worker-class=eventlet"' >> ~/.bashrc

#COPY --chown=secugen:secugen . .

COPY --chown=secugen:secugen ./main/storage/logs/app.log /main/storage/logs/app.log
RUN ln -sf /proc/1/fd/1 /main/storage/logs/app.log

COPY --chown=secugen:secugen ./main/requirements.txt /main
COPY --chown=secugen:secugen ./main /main


USER secugen
# WORKDIR /home/secugen


RUN pip install --upgrade pip
RUN pip install --user -r requirements.txt

# Configuración de usuario
USER root
RUN groupadd SecuGen
RUN gpasswd -a root SecuGen
RUN gpasswd -a secugen SecuGen

# Copia reglas de configuración de huellero
RUN mkdir -p /etc/udev/rules.d/
COPY ./config/99SecuGen.rules /etc/udev/rules.d/

# Copia sdk a directorio raiz
RUN mkdir -p /fdxsdkpro
COPY --chown=secugen:secugen ./config/fdxsdkpro/ /fdxsdkpro

# Instalación del sdk
WORKDIR /fdxsdkpro/lib/linux3
RUN make install

# Accesos directos a los ejecutables que interactúan con el huellero {check, capture, close}
RUN ln -s /fdxsdkpro/bin/linux3/wsq_check_devise_fdu03 /usr/local/bin/wsq_check_devise_fdu03
RUN ln -s /fdxsdkpro/bin/linux3/wsq_direct_capture_fdu03 /usr/local/bin/wsq_direct_capture_fdu03
COPY ./bin/killdevice /usr/local/bin/killdevice
RUN chmod 777 /usr/local/bin/killdevice

# Correccion de permisos a los archivos
RUN find /main -type f -exec chmod 775 $i {} \;
RUN find /main -type d -exec chmod 644 $i {} \;

# Triggers de los dispositivos
#RUN /etc/init.d/udev restart  && udevadm trigger
#RUN sbin/ldconfig /usr/local/lib

USER secugen
WORKDIR /main

EXPOSE 5000

# Modo dev usando flask run
#CMD ["flask", "run", "-p", "5000", "-h", "0.0.0.0"]

# Aca usar en desarrollo, --reload sirve para detectar cambios y recargar el servidor
#CMD ["gunicorn", "--bind", "0.0.0.0:5000", "app:app", "--reload", "--workers=2", "--threads=4", "--log-file=-", "--worker-tmp-dir", "/dev/shm" ,"--worker-class=gthread"]
# gunicorn --bind 0.0.0.0:5000 app:app --reload --workers=2 --threads=4 --log-file=- --worker-tmp-dir /dev/shm --worker-class=gthread
# gunicorn --bind 0.0.0.0:5000 app:app --reload --workers=1 --threads=1 --log-file=- --worker-tmp-dir /dev/shm --worker-class=gthread
# gunicorn --bind 0.0.0.0:5000 app:app --reload --workers=1 --threads=4 --log-file=- --worker-tmp-dir /dev/shm --worker-class=gthread

# Este de abajo usar en producción usando gunicorn y threads
#CMD ["gunicorn", "--bind", "0.0.0.0:5000", "app:app", "--log-file=-", "--worker-tmp-dir", "/dev/shm", "--workers=8", "--threads=8", "--worker-class=gthread"]

# Este de abajo usar en producción usando gunicorn y eventlet
CMD ["gunicorn", "--bind", "0.0.0.0:5000", "app:app", "--log-file=-", "--worker-tmp-dir", "/dev/shm", "--workers=8", "--threads=8", "--worker-class=eventlet"]

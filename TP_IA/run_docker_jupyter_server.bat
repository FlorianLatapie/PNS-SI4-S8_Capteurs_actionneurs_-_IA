@ECHO OFF
ECHO **Running jupyter notebook with docker 
PowerShell -NoProfile -ExecutionPolicy Bypass -Command "docker run -it --rm --name jupyter-tensorflow -p 8888:8888 -e JUPYTER_ENABLE_LAB=yes -v $((pwd).tostring()+\":/home/jovyan/work\")jupyter/tensorflow-notebook"

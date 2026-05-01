#!/usr/bin/env bash

project_name='prog3_programacion_concurrente_v2023_2'
source_code='
        P1.h
        P2.h
        P3.h
        P4.h
        P5.h
  '
rm -f ${project_name}.zip
zip -r -S ${project_name} ${source_code}
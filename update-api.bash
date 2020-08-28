#!/bin/bash

if [[ -z "$GTAD" || -z "$CSAPIDIR" ]]; then
    echo "You have to specify GTAD and CSAPIDIR variables."
    exit 1
fi

cd "$(dirname "$0")/src" &&
"$GTAD" --config ../gtad/gtad.yaml --out csapi "$CSAPIDIR" key_backup.yaml-

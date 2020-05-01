head -n 1100000 /dev/urandom | base64 | tr -cd [:lower:]\\n | head -n 1000000 | cut -c-13 > data.txt

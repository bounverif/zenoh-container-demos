FROM python:3.10

RUN git clone --depth=1 https://github.com/doganulus/timescales.git /tmp/timescales \
    && cd /tmp/timescales \
    && make large \
    && mkdir -p /app/timescales \
    && mv /tmp/timescales/largesuite /app/timescales/ \
    && rm -rf /tmp/timescales

COPY . /app/
WORKDIR /app/

RUN python3 -m pip install --no-cache-dir -r /app/requirements.txt

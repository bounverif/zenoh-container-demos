FROM python:3.10

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get install -y \        
        cyclonedds-tools \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

RUN python3 -m pip install --no-cache-dir cyclonedds

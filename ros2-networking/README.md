# Containerized Zenoh DDS bridge example for ROS 2

This example demonstrates a ROS 2 DDS network and Zenoh DDS bridge in containers. Both DDS and Zenoh networks are isolated from the `host` network. Zenoh router is exposed at `localhost:7474` and allows local Zenoh apps to read/write ROS 2 `/chatter` topic over the bridge.

## Prerequisites

- A container runtime engine such as `docker` or `podman`
- A container compose provider such as `docker-compose` or `podman-compose`
- Python `eclipse-zenoh` library

## Example run

```sh
# # Install system dependencies and create Zenoh virtual environment
# apt install python3 python3-pip python3-venv
# python3 -m venv ~/.venv/zenoh
#
# # Activate virtual environment
# source ~/.venv/zenoh/bin/activate

# # Install Zenoh python client
# python3 -m pip install eclipse-zenoh


# Launch a DDS network and Zenoh bridge in containers 
cd ./ros2-networking
docker compose up -d

# Execute the example app
python3 app.py

# Stop containers after test is finished
docker compose down
```

## Debugging

Check logs for the `bridge` service.
```sh
docker compose logs bridge
```

Enter into `bridge` container for further inspection:
```sh
docker compose exec bridge /bin/sh
```

Check ROS 2 network works as expected:
```sh
docker compose exec listener /bin/sh

# Inside container
source /opt/ros/humble/setup.sh
ros2 topic list
ros2 topic echo /chatter
```

Check open ports on the host:
```sh
sudo netstat -tulpn | grep LISTEN
```
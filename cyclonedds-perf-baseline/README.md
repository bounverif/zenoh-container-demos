
# Containerized CycloneDDS perfomance measurement

This example demonstrates a pure DDS network performance using `ddsperf` inside containers.

## Prerequisites

- A container runtime engine such as `docker` or `podman`
- A container compose provider such as `docker-compose` or `podman-compose`

## Example run

```sh
docker compose up
```

You will see throughput measurement (message rate and bitrate) at the program output.

```bash
...
talker-1    | [1] 31.000 31.9k/s  22u |                     .x@@x-___|   0% 305u
talker-1    | [1] 31.000  rss:11.7MB vcsw:2154 ivcsw:26 recvUC:1%+1% pub:9%+84%
listener-1  | [1] 31.000  size 16384 total 1047377 lost 0 delta 31930 lost 0 rate 31.93 kS/s 4185.09 Mb/s (35.09 kS/s 4599.92 Mb/s)
listener-1  | [1] 31.000  rss:13.5MB vcsw:56468 ivcsw:0 ddsperf:1%+0% tev:1%+3% recvUC:16%+35%
...
```

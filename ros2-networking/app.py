
import time
import argparse
import zenoh

# pycdr2 is the serializer of data in CDR format (required by ROS2)
from pycdr2 import IdlStruct
from dataclasses import dataclass

# Equivalent to std_msgs.msg.String class, but serializable by pycdr2
@dataclass
class String(IdlStruct, typename="String"):
    data: str


def chatter_callback(sample: zenoh.Sample):
    # Deserialize the message
    msg = String.deserialize(sample.payload)
    print('I heard: [%s]' % msg.data)

def main():
    parser = argparse.ArgumentParser(
        prog='listener',
        description='Zenoh/ROS2 listener example')
    parser.add_argument('--config', '-c', dest='config',
        metavar='FILE',
        type=str,
        help='A configuration file.')
    args = parser.parse_args()

    print(args.config)

    # Create Zenoh Config from file if provoded, or a default one otherwise
    conf = zenoh.Config.from_file(args.config) if args.config is not None else zenoh.Config()
    # Open Zenoh Session
    session = zenoh.open(conf)

    # Declare a subscriber
    sub = session.declare_subscriber('rt/chatter', chatter_callback)

    try:
        while True:
            time.sleep(1)
    except (KeyboardInterrupt):
        pass
    finally:
        sub.undeclare()
        session.close()


if __name__ == '__main__':
    main()
import time
import argparse
import zenoh


def listener_callback(sample: zenoh.Sample):
    print(f">> [Subscriber] Received {sample.kind} ('{sample.key_expr}': '{sample.payload.decode('utf-8')}')")

def main():
    parser = argparse.ArgumentParser(
        prog='listener',
        description='Zenoh/ROS2 listener example')
    parser.add_argument('--config', '-c', dest='config',
        metavar='FILE',
        type=str,
        help='A configuration file.')
    args = parser.parse_args()

    # Create Zenoh Config from file if provoded, or a default one otherwise
    conf = zenoh.Config.from_file(args.config) if args.config is not None else zenoh.Config()
    # Open Zenoh Session
    session = zenoh.open(conf)

    # Declare a subscriber
    sub = session.declare_subscriber('bounverif/timescales', listener_callback)

    try:
        while True:
            time.sleep(1)
    except (KeyboardInterrupt):
        pass
    

    sub.undeclare()
    session.close()


if __name__ == '__main__':
    main()

import time
import argparse
import zenoh


def main():

    # Command line argument parsing
    parser = argparse.ArgumentParser(
        prog='publisher',
        description='Timescales publisher example')
    parser.add_argument('--replay', '-r', dest='replay_file',
                        default='/app/timescales/largesuite/AbsentAQ/AbsentAQ10.jsonl',
                        metavar='FILE',
                        type=str,
                        help='The file to replay.')
    parser.add_argument('--period_us', '-d', dest='period_us',
                        default='10000',
                        type=int,
                        help='The period in microseconds.')
    parser.add_argument('--key', '-k', dest='key',
                        default='bounverif/timescales',
                        type=str,
                        help='The key expression to publish onto.')
    parser.add_argument('--config', '-c', dest='config',
                        metavar='FILE',
                        type=str,
                        help='A configuration file.')

    args = parser.parse_args()

    # Create Zenoh Config from file if provoded, or a default one otherwise
    conf = zenoh.Config.from_file(args.config) if args.config is not None else zenoh.Config()

    # Open Zenoh Session
    session = zenoh.open(conf)

    # Declare a publisher
    pub = session.declare_publisher(args.key)
    publish_period = args.period_us / 1e6

    with open(args.replay_file) as file:
        print(f"Replaying {args.replay_file} with period {publish_period} seconds")
        for line in file:
            buf = line.rstrip()
            pub.put(buf)
            time.sleep(publish_period)

    pub.undeclare()
    session.close()

if __name__ == '__main__':
    main()
